#!/bin/bash


# git clone https://github.com/edrosten/TooN.git
# git clone https://github.com/edrosten/gvars.git
# git clone https://github.com/edrosten/libcvd.git

sudo apt-get install -y build-essential pkg-config #build tools
sudo apt-get install -y freeglut3-dev #opengl
sudo apt-get install -y libblas-dev liblapack-dev libfortran3 ncurses-dev libreadline-dev libdc1394-22-dev #ptam specific requrements
sudo apt-get install -y libtiff-dev libjpeg-dev libpng-dev #image libraries
sudo apt-get install -y libglib2.0-0 libglib2.0-dev libgstreamer0.10-0 libgstreamer0.10-dev libgtk2.0-dev #gstreamer


#--- TooN installation
cd TooN/
./configure
make
sudo make install
cd ..

#--- libcvd installation
export CXXFLAGS=-D_REENTRANT
cd libcvd/
./configure --without-ffmpeg
make
sudo make install
cd ..

#--- gvars installation
cd gvars/
./configure --disable-widgets
make
sudo make install
cd ..





