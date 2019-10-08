#!/bin/bash

RELEASE=/home/centos/C/Athena-OpenROAD/zrouter/

PATH=$RELEASE/bin:$RELEASE/x86-64-linux/zrouter/bin:$RELEASE/msh/dist/i386-linux/bin/:$PATH

LD_LIBRARY_PATH=$RELEASE/msh/dist/i386-linux/lib:$LD_LIBRARY_PATH:/lib64:/usr/lib64:/lib:/usr/lib

TCL_LIBRARY=/usr/lib64/tcl8.5
TK_LIBRARY=/usr/lib64/tk8.5
BLT_LIBRARY=/usr/share/tcl8.5/blt2.4

export PATH TCL_LIBRARY LD_LIBRARY_PATH TK_LIBRARY BLT_LIBRARY

export MPT_ARCH=64
export ADS_LICENSE=ADS_2006
export NO_LICENSE_TOKEN=dimitris_friend
export ADS_NO_LICENSE=
export MSHCONFIG=$RELEASE/msh.conf

