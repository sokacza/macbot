#!/bin/bash

touch ./install_log.txt
mkdir ./temporary


git clone --branch master https://github.com/YDLIDAR/YDLidar-SDK.git temporary/YDLidar-SDK
echo "Cloned YDLiDAR SDK" >> ./install_log.txt 
mkdir ./temporary/build
cd temporary/YDLidar-SDK/build
cmake ..
make
sudo make install
cd ../..
echo "Successfully installed YDLiDAR SDK"


/bin/bash ./ros/diff_drive/old_install.bash
sudo echo "Installed old diff drive" >> install_log.txt
 
/bin/bash ./ros/dwa_local_planner/old_install.bash
sudo echo "Installed old dwa local planner" >> install_log.txt

/bin/bash ./ros/ydlidar_ros_driver/old_install.bash
sudo echo "Installed cold ydlidar_ros_driver" >> install_log.txt

/bin/bash ./ros/macbot/old_install.bash
sudo echo "Installed old macbot" >> install_log.txt

mkdir -p macbot_ws/src
cd macbot_ws/src
catkin_init_workspace
cd ..
catkin_make

echo "Created ROS workspace" >> ../install_log.txt

mv ../temporary/diff_drive src
catkin_make
sudo echo "Built diff_drive" >> ../install_log.txt
mv ../temporary/dwa_local_planner src
catkin_make
sudo echo "Built dwa_local_planner" >> ../install_log.txt
mv ../temporary/ydlidar_ros_driver src
catkin_make
sudo echo "Built ydlidar_ros_driver" >> ../install_log.txt
mv ../temporary/macbot src
catkin_make
echo "Built macbot" >> ../install_log.txt

cd ..
rm -Rf temporary
echo "Cleaning temp files" >> ./install_log.txt

echo "DONE!" >> ./install_log.txt





