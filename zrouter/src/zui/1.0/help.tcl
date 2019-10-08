#
# BSD 3-Clause License
#
# Copyright (c) 2019, Nefelus Inc
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# * Redistributions of source code must retain the above copyright notice, this
#   list of conditions and the following disclaimer.
#
# * Redistributions in binary form must reproduce the above copyright notice,
#   this list of conditions and the following disclaimer in the documentation
#   and/or other materials provided with the distribution.
#
# * Neither the name of the copyright holder nor the names of its
#   contributors may be used to endorse or promote products derived from
#   this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

package provide ade::ui 1.0

#
#       Class: ZHelp
# Description: Zui Popup help
#      Author: Manolis M. Tsangaris
#
itcl::class ZHelp {
    inherit LWBrowser
    # -------------------------------------------------- Public Variables --
    # -------------------------------------------------- Public Methods ----
    public method Show {page} {
        zh_show $page
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    # -------------------------------------------------- Common Variables --
    common HELP
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        Show /
    }
    destructor { }
    # -------------------------------------------------- Private Methods ---
    # ----------------------------------------------- Interface Implementation
    private method zh_show {url} {
        if ![info exists HELP($url)] {
            set url /error
        }
        LWBrowser::ShowPage $HELP($url)
    }
    # ----------------------------------------------- CallBacks
    # -------------------------------------------------- Common Initialization

array set HELP {

/ { }

/gcmds {
<h2>ZUI commands</h2>

The following keyboard/mouse shortcuts are recognized:<br>

<PRE>
<b>        KEY         Description</b>
           SPACE: zoom in
  CONTROL  SPACE: zoom out
      Arrow Keys: pan left, right, up and down
LEFT MOUSE
          BUTTON: Select an object
SHIFT+LEFT
    MOUSE BUTTON: select an area to zoom in
RIGHT
    MOUSE BUTTON: Inspect an object
             ESC: Interrupt the drawing of large number of objects
</PRE>
}

/error {
    This page does not exist
}

/about {
<center>
<b>
<i>
ZUI: ZROUTE(tm) graphical user interface<br>
</i>
</b>
</center>
}

/zdb {
<h2>ZDB: ZROUTE debugger</h2>

<PRE>
It can be invoked from the zroute command prompt, and will
accept additional arguments corresponding to the tcl script to run:

% zdb db ztest

This particular command will invoke the ztest routine will simulate
the effect of debugging c++ code.

At the zdb prompt, you can use help to see the available commands.

ZDB will allow stepping through any zevent posted by the C++ code,
with additional logic to stop when a condition is true.

}


/ucmds {
<h2>ZUI user commands</h2>
<P>
Those commands are entered from the command line window, at the
bottom of the screen:
<PRE>
show net id ID  : to show a net by id
show id         : try to guess what is id, and then show the corresponding obj
gfactor  INT    : grid size in pixels to sample/aggregate
bbox x0 y0 x1 y1: shoe the bounding box specified in the current mode
</PRE>
}


}



}
