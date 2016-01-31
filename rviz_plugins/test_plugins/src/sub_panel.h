#ifndef BARE_PANEL_H
#define BARE_PANEL_H

#include <ros/ros.h>
#include <QLabel>
#include <std_msgs/UInt8.h>

#include <rviz/panel.h>

class QLineEdit;

namespace test_plugins
{

  class SubPanel: public rviz::Panel
  {
    Q_OBJECT
    public:
      SubPanel( QWidget* parent = 0 );

    private:
      void subCallback(const std_msgs::UInt8 &msg);

    public Q_SLOTS:

    protected Q_SLOTS:

    protected:
      ros::Subscriber sub;
      ros::NodeHandle nh_;
      QLabel* output_topic_editor_;

  };

}

#endif // BARE_PANEL_H