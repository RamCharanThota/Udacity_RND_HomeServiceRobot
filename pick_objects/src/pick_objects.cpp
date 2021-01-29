#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>

// Define a client for to send goal requests to the move_base server through a SimpleActionClient
typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;
float pick_up_positon[3]={5,3,1};
float drop_off_positon[3]={2.5,2,1};

int main(int argc, char** argv){
  // Initialize the simple_navigation_goals node
  ros::init(argc, argv, "simple_navigation_goals");

  //tell the action client that we want to spin a thread by default
  MoveBaseClient ac("move_base", true);

  // Wait 5 sec for move_base action server to come up
  while(!ac.waitForServer(ros::Duration(5.0))){
    ROS_INFO("Waiting for the move_base action server to come up");
  }

  move_base_msgs::MoveBaseGoal goal;

  // set up the frame parameters
  goal.target_pose.header.frame_id = "map";
  goal.target_pose.header.stamp = ros::Time::now();

  // Define a position and orientation for the robot to reach
  goal.target_pose.pose.position.x =  pick_up_positon[0];
  goal.target_pose.pose.position.y =  pick_up_positon[1];
  goal.target_pose.pose.orientation.w =  pick_up_positon[2];

   // Send the goal position and orientation for the robot to reach
  ROS_INFO("Sending goal");
  ac.sendGoal(goal);

  // Wait an infinite time for the results
  ac.waitForResult();

  // Check if the robot reached its goal
  if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
{
    ROS_INFO("Hooray, the robot reached pickup goal");
    ros::Duration(5.0).sleep();

// Define a position and orientation for the robot to reach
  goal.target_pose.pose.position.x = drop_off_positon[0];
  goal.target_pose.pose.position.y = drop_off_positon[1];
  goal.target_pose.pose.orientation.w = drop_off_positon[2];

   // Send the goal position and orientation for the robot to reach
  ROS_INFO("Sending goal");
  ac.sendGoal(goal);

  // Wait an infinite time for the results
  ac.waitForResult();

  // Check if the robot reached its goal
  if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED){
    ROS_INFO("Hooray, the robot reached dropoff goal");}
  else
{
    ROS_INFO("The base failed to move forward 1 meter for some reason");
}
}
  else
{
    ROS_INFO("The base failed to move forward 1 meter for some reason");
 
}

   


  return 0;
}
