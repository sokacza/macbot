# Project Install

In 2023, the software and documentation was aggregated into the following GitHub repo:

{% embed url="https://github.com/sokacza/macbot/tree/main" %}
MacBot Project Repository
{% endembed %}

When setting up a MacBot, do the following:

* Create an Ubuntu 18.04 image to install on the Jetson Nano [https://developer.nvidia.com/embedded/learn/get-started-jetson-nano-devkit](https://developer.nvidia.com/embedded/learn/get-started-jetson-nano-devkit)
* Create a new user with the following credentials:
  * Username: jnano
  * Password: 9055259140
* Rename the computer to macbot##, e.g. macbot01
* Clone the macbot repo into the /home/jnano user directory
* Install ROS melodic using **melodic\_install.bash**
* If the MacBot has access to the internet, run **install.bash**
* If the MacBot does not have access to the internet, run **offline\_install.bash**
* A ROS workspace should be created called **macbot\_ws**
