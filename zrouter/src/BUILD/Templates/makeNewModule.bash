#!/bin/bash -f

if [ $# -lt 2 ]
then
	echo " "
	echo "makeNewModule -- creates all the basic initial objects. APIs and Makefile changes"
	echo "                 to add a new module in Nefelus Build Infrastructure"
        echo "                 and to create a Tcl Object Name which will be available to running Tcl Scripts"
        echo "                 "
	echo "Usage: <Cpp_Module_Class> <Tcl_Object>"
	echo "                 Cpp_Module_Class is the C++ Class that provides the APIs between Tcl and the main module Classes"
	echo "                 Tcl_Object is the Tcl Module Name to be used in the run Tcl scripts"
	echo " "
	echo "REQUIREMENT: makeNewModule should run under zrouter/src"
	echo " "
	exit
fi

Templates=ModuleTemplate
mkdir $1
tcl_dir=$1/$2_tcl
mkdir $tcl_dir

rm -f Makefile.before.$1
cp Makefile Makefile.before.$1
sed "/LIBDIRS=/a\ \t$1 \\\\" Makefile.before.$1 > Makefile

cd $1
pwd

# Create the files for the new Modules`
sed s/TEMPLATE/$1/g ../$Templates/Makefile | sed s/TMP/$2/g > Makefile
sed s/TEMPLATE/$1/g ../$Templates/TEMPLATE.cpp > $1.cpp
sed s/TEMPLATE/$1/g ../$Templates/db_TEMPLATE.cpp > db_$1.cpp
sed s/TEMPLATE/$1/g ../$Templates/TEMPLATE.h > $1.h

cd $2_tcl
pwd

sed s/TMP/$2/g ../../$Templates/TMP_tcl/Makefile > Makefile
sed s/TMP/$2/g ../../$Templates/TMP_tcl/TMP.ti   > $2.ti

sed s/TMP/$2/g ../../$Templates/TMP_tcl/TMP.cpp  | sed s/TEMPLATE/$1/g  > $2.cpp
sed s/TMP/$2/g ../../$Templates/TMP_tcl/TMP.h    | sed s/TEMPLATE/$1/g  > $2.h

cd ../../
pwd

cd main

Module_lib=lib$1.a
Module_tcl_lib=lib$2_tcl.a
cp Makefile Makefile.before.$1
sed "/ZLIB/a\ \t../$1/$Module_lib \\\\"  Makefile.before.$1 >  Makefile.$1
sed "/ZLIB/a\ \t../$tcl_dir/$Module_tcl_lib \\\\" Makefile.$1 > Makefile.$2
cp Makefile.$2 Makefile 

cp modules.cpp modules.cpp.before.$2
sed "/ZDB/a\ \tZTCL_REGISTER\($2\)"  modules.cpp.before.$2 >  modules.cpp

cd ../
pwd

