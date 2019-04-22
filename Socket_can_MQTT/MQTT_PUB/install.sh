#!/bin/bash 
sudo apt-get install git 

git clone https://github.com/eclipse/paho.mqtt.c.git

cd paho.mqtt.c

mkdir build && cd build
cmake ..

exit 0
