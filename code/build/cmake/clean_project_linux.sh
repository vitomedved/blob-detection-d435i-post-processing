#!/bin/bash

CURRENT_DIR=$PWD
LINUX_BUILD_DIR=$PWD"/linux_build"
LINUX_OUT_DIR=$PWD"/linux_out"

if [ -d $LINUX_BUILD_DIR ]; then
    rm -rf $LINUX_BUILD_DIR
fi

if [ -d $LINUX_OUT_DIR ]; then
    rm -rf $LINUX_OUT_DIR
fi
