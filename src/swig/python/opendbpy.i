%module opendbpy

%{
#define SWIG_FILE_WITH_INIT
#include "db.h"
using namespace odb;
%}

%include <stl.i>
%include <typemaps.i>
%include <std_string.i>
%include <std_vector.i>
%include <std_pair.i>

%include "db.h"