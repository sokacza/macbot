#!/bin/bash

echo "Installing Git and Perl"
sudo apt -y install git perl wget

echo "Installing TurtleSim"
sudo apt install ros-melodic-turtlesim

echo "Cloning motor driver firmware to ~/Documents/MotorDriver"
mkdir -p ~/Documents/MotorDriver/{Lib, Formware, DistributionBoard}
git clone https://code.roboteurs.com/maciot-libs/golink-env.git ~/Documents/MotorDriver/CAN
git clone  https://code.roboteurs.com/maciot-libs/arduino-esp32-encoder ~/Documents/MotorDriver/Firmware
wget -P ~/Documents/MotorDriver/MotorDriver https://code.roboteurs.com/snippets/11/raw
wget -P ~/Documents/MotorDriver/DistributionBoard https://code.roboteurs.com/snippets/10

echo "Setting up ROS workspace"
mkdir -p ~/macbot_ws/src
cd ~/macbot_ws/src
catkin_init_workspace
cd ..
catkin_make
sudo echo "echo \"macbot_ws sourced!\"" >> ~/.bashrc
sudo echo "source ~/macbot_ws/devel/setup.bash" >> ~/.bashrc
cd src
git clone https://github.com/merose/diff_drive.git
cd ..
echo "\n\n----------\nBuilding diff_drive: \n----------\n\n"
catkin_make
cd src
git clone https://github.com/ros-planning/navigation
cd ..
echo "\n\n----------\nBuilding dwa_local_planner: \n----------\n\n"
catkin_make
git clone https://github.com/YDLIDAR/ydlidar_ros_driver.git
cd ..
echo "\n\n----------\nBuilding ydlidar_ros_driver: \n----------\n\n"
catkin_make
echo "\n\n----------\nPLEASE ENSURE THAT YDLIDAR SDK IS INSTALLED FROM:\nhttps://github.com/YDLIDAR/YDLidar-SDK = https://github.com/YDLIDAR/YDLidar-SDK.git \n----------\n\n"

echo "\n\n\n\n\nDONE :)\n"
