
#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include "nav_msgs/Odometry.h"

float pick_up_position[3]={5,3,1};
float drop_off_position[3]={2.5,2,1};

bool at_pick_up_position= false;
bool at_drop_off_position=false ;


void CallbackFunction(const nav_msgs::Odometry::ConstPtr& msgs)
{ 

if (std::abs(msgs->pose.pose.position.x-pick_up_position[0])<0.20 && std::abs(msgs->pose.pose.position.y-pick_up_position[1])<0.20  ){

at_pick_up_position= true;

}

if (std::abs(msgs->pose.pose.position.x-drop_off_position[0])<0.20 && std::abs(msgs->pose.pose.position.y-drop_off_position[1])<0.20){

at_drop_off_position=true;

}


 


}






int main( int argc, char** argv )
{
  ros::init(argc, argv, "basic_shapes");
  ros::NodeHandle n;
  ros::Rate r(1);
  ros::Publisher marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 1);
ros::Subscriber odometry_sub = n.subscribe("odom", 100,CallbackFunction);



  uint32_t shape = visualization_msgs::Marker::CUBE;

  while (ros::ok())
  {
    visualization_msgs::Marker marker;
    // Set the frame ID and timestamp.  See the TF tutorials for information on these.
    marker.header.frame_id = "map";
    marker.header.stamp = ros::Time::now();

    // Set the namespace and id for this marker.  This serves to create a unique ID
    // Any marker sent with the same namespace and id will overwrite the old one

    marker.ns = "basic_shapes";
    marker.id = 0;

    // Set the marker type.  Initially this is CUBE, and cycles between that and SPHERE, ARROW, and CYLINDER

    marker.type = shape;


    // Set the marker action.  Options are ADD, DELETE, and new in ROS Indigo: 3 (DELETEALL)

    marker.action = visualization_msgs::Marker::ADD;

    // Set the pose of the marker.  This is a full 6DOF pose relative to the frame/time specified in the header
    marker.pose.position.x = pick_up_position[0];
    marker.pose.position.y = pick_up_position[1];
    marker.pose.orientation.w = pick_up_position[2];
// %EndTag(POSE)%

    // Set the scale of the marker -- 1x1x1 here means 1m on a side
// %Tag(SCALE)%
    marker.scale.x = 0.5;
    marker.scale.y = 0.5;
    marker.scale.z = 0.5;
// %EndTag(SCALE)%

    // Set the color -- be sure to set alpha to something non-zero!
// %Tag(COLOR)%
    marker.color.r = 1.0f;
    marker.color.g = 0.0f;
    marker.color.b = 0.0f;
    marker.color.a = 1.0;
// %EndTag(COLOR)%

// %Tag(LIFETIME)%
    marker.lifetime = ros::Duration();
// %EndTag(LIFETIME)%

    // Publish the marker
// %Tag(PUBLISH)%
    while (marker_pub.getNumSubscribers() < 1)
    {
      if (!ros::ok())
      {
        return 0;
      }
      ROS_WARN_ONCE("Please create a subscriber to the marker");
      sleep(1);
    }
    marker_pub.publish(marker);

while(!at_pick_up_position)
   {
    ros::spinOnce();
   }

bool picked_up_marker=false;

if (at_pick_up_position && !picked_up_marker){
marker.action = visualization_msgs::Marker::DELETE;// hide the marker
marker_pub.publish(marker);
picked_up_marker= true;
}

while(!at_drop_off_position)
   {
    ros::spinOnce();
   }

bool drop_off_marker=false;
 
if(at_drop_off_position && !drop_off_marker){
marker.action = visualization_msgs::Marker::ADD;// add the marker
marker.pose.position.x = drop_off_position[0];
marker.pose.position.y =drop_off_position[1];
marker.pose.orientation.w = drop_off_position[2];
marker_pub.publish(marker);
drop_off_marker= true;
ros::Duration(10.0).sleep(); // Sleep for 10 second
}

}

}

