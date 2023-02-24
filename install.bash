#!/bin/bash

touch ./install_log.txt
mkdir ./temporary

{
    sudo apt -y install git perl wget
    && echo "Installed packages" >> ./install_log.txt
} || echo "Failed to install some packages" >> ./install_log.txt

{ 
    sudo apt install ros-melodic-turtlesim 
    && echo "Installed ROS TurtleSim" >> ./install_log.txt 
} || echo "Failed to install ROS TurtleSim" >> ./install_log.txt

{ 
    git clone --branch master https://github.com/YDLIDAR/YDLidar-SDK.git ./temporary
    && echo "Cloned YDLiDAR SDK" >> ./install_log.txt 
    && mkdir ./temporary/build
    && cd temporary/YDLidar-SDK/build
    && cmake ..
    && make
    && sudo make install
    && cd ../..
    && echo "Successfully installed YDLiDAR SDK"
} || echo "Failed to install YDLiDAR SDK" >> ./install_log.txt


{
    /bin/bash ./ros/diff_drive/clone_install.bash
    && echo "Installed clone diff drive" >> ./install_log.txt
} || { 
    {
    /bin/bash ./ros/diff_drive/old_install.bash
    && echo "Installed copy diff drive" >> ./install_log.txt 
    } || echo "Failed to install diff drive" >> ./install_log.txt 
}











