#!/bin/sh

xterm  -e  " roslaunch turtlebot_gazebo turtlebot_world.launch world_file:=$(rospack find gmapping)/../../world/final_updated_home.world"&
sleep 5
xterm  -e  " roslaunch turtlebot_gazebo amcl_demo.launch" & 
sleep 5
xterm  -e  " roslaunch turtlebot_rviz_launchers view_navigation.launch" &
sleep 5
xterm  -e  " rosrun pick_objects pick_objects"





