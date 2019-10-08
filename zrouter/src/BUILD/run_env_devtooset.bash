#!/bin/bash
which scl > /dev/null
if [ $? -ne 0 ]; then
  echo "Please install scl first"
  exit
fi

if [ -n "$1" ]; then
  toolset="$1"
  yum list installed "$toolset" > /dev/null 2>&1
  if [ $? -eq 1 ]; then
    echo "$toolset has not been installed"
  fi
else
 toolset=`scl --list | sort -n -r -k 2 -t '-' | head -1`
fi

PATH=$(echo :$PATH: | sed  -e ':loop' -e "s,:/opt/rh/devtoolset[^:]*:,:,g" -e 't loop' -e 's/^://' -e 's/:$//')
LD_LIBRARY_PATH=$(echo :$LD_LIBRARY_PATH: | sed  -e ':loop' -e "s,:/opt/rh/devtoolset[^:]*:,:,g" -e 't loop' -e 's/^://' -e 's/:$//')

export CC=/opt/rh/$toolset/root/usr/bin/gcc
export CPP=/opt/rh/$toolset/root/usr/bin/cpp
export CXX=/opt/rh/$toolset/root/usr/bin/g++
export PATH=/opt/rh/$toolset/root/usr/bin:$PATH
export LD_LIBRARY_PATH=/opt/rh/$toolset/root/usr/lib64:/opt/rh/$toolset/root/usr/lib:/opt/rh/$toolset/root/usr/lib64/dyninst:/opt/rh/$toolset/root/usr/lib/dyninst:/opt/rh/$toolset/root/usr/lib64:/opt/rh/$toolset/root/usr/lib:$LD_LIBRARY_PATH
