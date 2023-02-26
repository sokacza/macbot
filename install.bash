#!/bin/bash

touch ./install_log.txt
mkdir ./temporary


sudo apt -y install git perl wget
echo "Installed packages" >> ./install_log.txt


 
sudo apt install ros-melodic-turtlesim 
echo "Installed ROS TurtleSim" >> ./install_log.txt 



git clone --branch master https://github.com/YDLIDAR/YDLidar-SDK.git ./temporary
echo "Cloned YDLiDAR SDK" >> ./install_log.txt 
mkdir ./temporary/build
cd temporary/YDLidar-SDK/build
cmake ..
make
sudo make install
cd ../..
echo "Successfully installed YDLiDAR SDK"


/bin/bash ./ros/diff_drive/clone_install.bash
echo "Installed clone diff drive" >> ./install_log.txt
 
/bin/bash ./ros/dwa_local_planner/clone_install.bash
echo "Installed clone dwa local planner" >> ./install_log.txt

/bin/bash ./ros/ydlidar_ros_driver/clone_install.bash
echo "Installed clone ydlidar_ros_driver" >> ./install_log.txt

/bin/bash ./ros/macbot/clone_install.bash
echo "Installed clone macbot" >> ./install_log.txt

mkdir -p ./macbot_ws/src
cd macbot_ws/src
catkin_init_workspace
cd ..
catkin_make

echo "Created ROS workspace" >> ../install_log.txt

mv ../temporary/diff_drive src
catkin_make
echo "Built diff_drive" >> ../install_log.txt
mv ../temporary/dwa_local_planner src
catkin_make
echo "Built dwa_local_planner" >> ../install_log.txt
mv ../temporary/ydlidar_ros_driver src
catkin_make
echo "Built ydlidar_ros_driver" >> ../install_log.txt
mv ../temporary/macbot src
catkin_make
echo "Built macbot" >> ../install_log.txt

cd ..
rm -Rf temporary
echo "Cleaning temp files" >> ./install_log.txt

echo "DONE!" >> ./install_log.txt






