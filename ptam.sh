#!/bin/bash


# git clone https://github.com/edrosten/TooN.git
# git clone https://github.com/edrosten/gvars.git
# git clone https://github.com/edrosten/libcvd.git

sudo apt-get install build-essential
sudo apt-get install -y libblas-dev liblapack-dev libfortran3 ncurses-dev libreadline-dev libdc1394-22-dev libtiff-dev libjpeg-dev libpng-dev


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



