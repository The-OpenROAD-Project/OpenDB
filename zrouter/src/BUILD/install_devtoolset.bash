#!/bin/bash

if [ -n "$1" ]; then
  echo "A devtoolset need to be specified."
  exit
fi

toolset="$1"

sudo yum -y install centos-release-scl
sudo yum -y install $toolset $toolset-libatomic-devel

