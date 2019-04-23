# SocketCAN_MQTT
SocketCAN MQTT sending and receiving data 

## Eclipse Paho MQTT C client
# Build requirements / compilation using CMake


### On Debian based systems this would mean that the following packages have to be installed:
```
apt-get install build-essential gcc make cmake cmake-gui cmake-curses-gui
```
### Also, in order to build a debian package from the source code, the following packages have to be installed
```
apt-get install fakeroot fakeroot devscripts dh-make lsb-release
```
### Ninja can be downloaded from its github project page in the "releases" section. Optionally it is possible to build binaries with SSL support. This requires the OpenSSL libraries and includes to be available. E. g. on Debian:
```
apt-get install libssl-dev
```
### The documentation requires doxygen and optionally graphviz:
```
apt-get install doxygen graphviz
```
## Problem statement 
### After Sending data from CAN to MQTT, MQTT sends the message back to the CAN network. Translating the data from  can_send, results fault values. I think changing data to binaries and send data in binaries will solve this issue. 
