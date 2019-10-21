#include <tcl.h>

using namespace std;

Tcl_Interp* _interp;

int 
opendbTclAppInit(Tcl_Interp *interp) 
{
    _interp = interp;

    if (Tcl_Init(interp) == TCL_ERROR) {
        return TCL_ERROR;
    }

    
    return TCL_OK;
}

int 
main(int argc, char *argv[]) 
{
    Tcl_Main(1, argv, opendbTclAppInit);
}