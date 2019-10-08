%typemap(in) unsigned char* bytearray
  "$1 = Tcl_GetByteArrayFromObj($input, 0);";

%module tkqt
%{
    #include <qapplication.h>
    #include "tkqt_swig.h"
%}
%init %{
    QtInit(interp);
%}

%include "tkqt_swig.h"

