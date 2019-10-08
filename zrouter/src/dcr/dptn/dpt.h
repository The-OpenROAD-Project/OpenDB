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

#ifndef ADS_DPT_H
#define ADS_DPT_H

#ifndef ADS_H
#include "ads.h"
#endif

BEGIN_NAMESPACE_ADS

typedef char dptPixel;

#define MAXCOLORSLICES 20

class dpt
{
  public:
    dpt();
    ~dpt();

    int configure(int width,int height,
		int x0, int y0, int x1, int y1,int nslices);

	// render a rectangle
	int box(int x0, int y0, int x1, int y1,int slice);

	// write slices as a pbm file
	int write_pbm(FILE*fp,int sl=0);

	// write all slices as a series of PBM files
	int write(FILE*fp);

	// colorize a slice
	int colorize(int slice,int rgb);

	// write all slices as a series of PBM files
	int write_image(FILE*fp);

    void dump_color();
	
  protected:
    dptPixel** slice;            // pixmaps
    int    width;
    int    height;
    int    nslices;              // max number of slices
    int    maxsll;
	int x0,x1,y0,y1;          // bounding box
	double sx,sy;                // scale factors
	int    *color;               // color table
    int     csize;              // size of the color table

    struct rgb
    {
        char out[4];
    };

    struct rgb * cl2;

  protected:
	inline int R_COLOR(unsigned int p) { return 0xff & ((p & 0xff0000)>>16); }
	inline int G_COLOR(unsigned int p) { return 0xff & ((p & 0x00ff00)>>8); }
	inline int B_COLOR(unsigned int p) { return 0xff & ((p & 0x0000ff)   ); }

	// find the highest slice of each pixel
	int top_slice(int x, int y); 
};

END_NAMESPACE_ADS

#endif
