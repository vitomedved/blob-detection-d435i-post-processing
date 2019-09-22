#!/bin/bash

CURRENT_DIR=$PWD
OPENCV_DIR=$CURRENT_DIR"/../../third-party/opencv"
OPENCV_SRC_DIR=$OPENCV_DIR"/opencv-4.1.1"
OPENCV_BUILD_DIR=$OPENCV_SRC_DIR"/build"
OPENCV_INSTALL_DIR=$OPENCV_DIR"/build_linux"

cd $OPENCV_DIR

if [ -d $OPENCV_SRC_DIR ]; then
    rm -rf $OPENCV_SRC_DIR"/*";
fi

tar -xvzf opencv-4.1.1.tar.gz

cd $OPENCV_SRC_DIR

if [ ! -d $OPENCV_BUILD_DIR ]; then
    mkdir $OPENCV_BUILD_DIR
else
    rm -rf $OPENCV_BUILD_DIR"/*";
fi

cd $OPENCV_BUILD_DIR

if [ ! -d $OPENCV_INSTALL_DIR ]; then
    mkdir $OPENCV_INSTALL_DIR
else
    rm -rf $OPENCV_INSTALL_DIR"/*";
fi

cmake -DBUILD_LIST=core,imgcodecs,improc,videoio,highgui,video -DCMAKE_INSTALL_PREFIX=$OPENCV_INSTALL_DIR $OPENCV_SRC_DIR

make -j15

make install

rm -rf $OPENCV_SRC_DIR
