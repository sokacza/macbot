#!/bin/bash
echo "Cloning dwa_planner for ROS Melodic..."
mkdir ../../temporary
git clone --branch melodic-devel https://github.com/ros-planning/navigation.git ../../temporary
echo "Done!"
