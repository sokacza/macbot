#!/bin/bash
echo "Cloning dwa_planner for ROS Melodic..."

git clone --branch melodic-devel https://github.com/ros-planning/navigation.git temporary/dwa_local_planner
echo "Done!"
