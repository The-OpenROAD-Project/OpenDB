///////////////////////////////////////////////////////////////////////////////
// BSD 3-Clause License
//
// Copyright (c) 2019, Nefelus Inc
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// * Neither the name of the copyright holder nor the names of its
//   contributors may be used to endorse or promote products derived from
//   this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

    // Window Specific API

    // embed an area into a new parent 
    void newParent(int ranum, long unsigned int wid);

    // bind an area to a new window
    void newWindow(int ranum, long unsigned int wid);

    // change working size of window to x by y
    // note: might be used to zoom in/out, although bbox also works for that
    void resize(int ranum, int x, int y);

    // sets the "bbox" of the associated window in terms of x,y (lower left)
    // and width and height - all in WORLD coordinates
    void setBBox(int ranum, int x, int y, int width, int height);
    void setChipBBox(int ranum, int x, int y, int width, int height);
    // convert co-ordinates from/to world values for a GIVEN WINDOW
    void world2Screen(int ranum, int x, int y);
    void screen2World(int ranum, int x, int y);

    // clear window
    void clear(int ranum);

    // copy bitmap to window
    void paint(int ranum);

    // re-create bitmap based on current display list and settings
    int refresh(int ranum);
    void refreshSelection(int ranum);

    // get a handle for a new window
    int newArea();

    // set the display list for a given window
    void setDataset(int ranum, char *tag, int x, int y);
    // set the image list for a given window
    void setImage(int ranum, char *tag, int x, int y);
    // set the selection display list for a given window
    void setSelection(int ranum, char *tag, int x, int y);

    void setScaleFactor(int ranum, double xsf, double ysf);
    void setOverscanColor(int ranum, char *color);

    void scroll(int ranum, int dx, int dy);
    void resetScroll(int ranum);

    // set brush and pen values for a given layer
    void setBrush(int ranum, int layer, const char * color);
    void setPen(int ranum, int layer, const char * color);

    // set selection brush and pen values 
    void setSelBrush(int ranum, const char * color);
    void setSelPen(int ranum, int width, const char * color);

    // turn layers on an off - PER window
    void setLayerStatus(int ranum, int layer, int status);

    // get Rendered/Selection Rendered stats
    int getRenderStats(int ranum);
    int getSRenderStats(int ranum);

    void grabImage(int ranum, char *name);

    void setColormap(int ranum, char *tag);
    void setSpecialColormap(int ranum, char *tag);


    // max of ALL layers - kept globally
    int getMaxLayers();

    void selectColormap(char *tag);

    // DataSet Specific API

    void getDataBBox(char *tag);
    void getDataSetData(char *tag);
    void getDataSetRange(char *tag, int start, int end);
    void getImageBBox(char *tag);

    // get Object count from dataset
    int getDatasetStats(char *tag);

    // erase a data set completely
    void eraseDataset(char *tag);

    void eraseImage(char *tag);
    void addImage64(char *tag, char *data, int len, int x0, int y0, int x1, int y1, char *format = NULL);
    void addImage(char *tag, unsigned char *bytearray, int len, int x0, int y0, int x1, int y1, char *format = NULL);
    void addImageChannel(char *tag, char *channel, int x0, int y0, int x1, int y1);
    void setImageBBox(char *tag, int x0, int y0, int x1, int y1);

    // get information about tags
    int getTagSize(char *type);
    int getTagCount(char *type, int idx);
    char *getTagName(char *type, int idx);

    // get the count of unique HType values
    int getSliceSize();
    // get the count of how many objects there are with a given HTYPE tag
    int getSliceCount(int slice);
    // get the NAME of a given HTYPE tag
    char *getSliceName(int);


    // set a given HTYPE tag's status
    void setVisibility(char *tag, int status);

    // set a given type's status (e.g. power)
    void setTypeStatus(char *type, int status);

    // set a given type/tag's status (e.g. power->signal)
    void setTagStatus(char *type, char *tag, int status);

    // show all tags in the system
    void dumpTags();
    void dumpDSets();

    void adjust(int ranum, int w, int l, int o, int y, int z);

    // add an Object to the dataset with name tag
    void addObject(char *tag, char *line);
    void addObjects(char *tag, char *list);

    // add Objects read from "channel" to the dataset with name tag
    // returns count of objects read
    int readFile(char *tag, char *channel);

    // called by swig initialization
    int QtInit(Tcl_Interp *interp);
