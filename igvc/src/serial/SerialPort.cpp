#include <igvc/SerialPort.h>
#include <iostream>

using namespace std;
using namespace boost::asio;

SerialPort::SerialPort(string device, int baud)
    : port(ioservice),
      path(device)
{
    try
    {
        port.open(device);
    } catch(...){}
    
    if( !port.is_open() ) {
        throw runtime_error("Could not open serial port " + device);
    }
    
    try
    {
        port.set_option(boost::asio::serial_port_base::baud_rate(baud));
        port.set_option(boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none));
        port.set_option(boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one));
    } catch(...) {
        port.close();
        throw runtime_error("Could not open serial port " + device);
    }
}

void SerialPort::flush()
{
    ::tcflush(port.lowest_layer().native_handle(), TCIOFLUSH);
}

SerialPort::~SerialPort()
{
    port.close();
}

bool SerialPort::isOpen()
{
    return port.is_open();
}

void SerialPort::write(string msg)
{
    if(port.is_open())
        boost::asio::write(port, boost::asio::buffer(msg.c_str(),msg.length()));
}

void SerialPort::write(char *buffer, int length)
{
    boost::asio::write(port, boost::asio::buffer(buffer, length));
}

void SerialPort::write(unsigned char *buffer, int length)
{
    if(port.is_open())
        boost::asio::write(port, boost::asio::buffer(buffer, length));
}

char SerialPort::read()
{
    if(!port.is_open()) return -1;
    
    char in;
    try
    {
        boost::asio::read(port, buffer(&in, 1));
    } catch (boost::exception_detail::clone_impl<boost::exception_detail::error_info_injector<boost::system::system_error> >& err) {
        port.close();
        throw runtime_error("Error reading serial port");
    }
    return in;
}

char* SerialPort::read(int numBytes)
{
    if(!port.is_open())
        return (char*)"";
    char* bytes = new char[numBytes];
    for(int i = 0; i < numBytes; i++)
        bytes[i] = read();
    return bytes;
}

string SerialPort::readln()
{
    string line = "";
    while(true)
    {
        char in = read();	
        if(in == '\n')
            return line;
        if(in != '\r') // if using return char - new line will follow - do not terminate early
            line = line + in;
    }
}

string SerialPort::devicePath()
{
    return path;
}
