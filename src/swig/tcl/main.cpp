#include <tcl.h>

using namespace std;

extern "C"
{
    extern int Opendbtcl_Init(Tcl_Interp* interp);
}

int
opendbTclAppInit(Tcl_Interp* interp)
{
    if (Opendbtcl_Init(interp) == TCL_ERROR)
    {
        return TCL_ERROR;
    }
    const char *tcl_helper_cmds =
#include "helpers.tcl"
    ;
    
    if (Tcl_Eval(interp, tcl_helper_cmds) == TCL_OK && 
        Tcl_Eval(interp, "namespace import odb::*") == TCL_OK) {
        return TCL_OK;
    }
    return  TCL_ERROR;
}

int
main(int argc, char* argv[])
{
    Tcl_Main(argc, argv, opendbTclAppInit);
}