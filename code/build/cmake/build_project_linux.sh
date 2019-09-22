#!/bin/bash

CURRENT_DIR=$PWD
LINUX_BUILD_DIR=$PWD"/linux_build"
BASE_CMAKE_DIR=$CURRENT_DIR"/../../components"
LINUX_OUT_DIR=$PWD"/linux_out"

if [ ! -d $LINUX_BUILD_DIR ]; then
    mkdir $LINUX_BUILD_DIR
else
    rm -rf $LINUX_BUILD_DIR"/*"
fi

cd $LINUX_BUILD_DIR

cmake -DOUTPUT_DIR=$LINUX_OUT_DIR $BASE_CMAKE_DIR

make -j5


