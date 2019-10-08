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

#include <stdlib.h>
#include "dpt.h"

BEGIN_NAMESPACE_ADS

// ----------------------------------------------------------------------------
//
// dpt - Methods
//
// ----------------------------------------------------------------------------
dpt::dpt()
{

}

dpt::~dpt()
{
	for(int s=0; s<nslices ; s++) {
		if (slice[s]) {
			free((char*)slice[s]);
		}
	}
	free((char*)slice);
}


int dpt::configure(int _width,int _height,
        double _x0, double _y0, double _x1, double _y1,int _nslices) {

	assert(_nslices>0);

	width   = _width;
	height  = _height;
	slice   = (dptPixel**) calloc(nslices,sizeof(nslices));
	nslices = _nslices;
	color   = (int*) calloc(nslices,sizeof(int));

	for(int s=0; s<nslices ; s++) slice[s] = 0;
	for(int s=0; s<nslices ; s++) color[s] = 1 << s;

	// bbox
	x0      = _x0;
	y0      = _y0;
	x1      = _x1;
	y1      = _y1;

	// scale factors
	sx      = ((double)width)  / (x1 - x0);
	sy      = ((double)height) / (y1 - y0);
}
// CLIP MACRO
#define CLIP(p,min,max) p = (p<min) ? min : (p>=max)? (max -1) : p

int dpt::box(double px0, double py0, double px1, double py1,int sl) {
	assert(sl>=0);
	int ssl = sl / (sizeof(dptPixel) * 8);
	int msl = sl % (sizeof(dptPixel) * 8);
	//fprintf(stderr,"# msl/ssl = %x %x\n",ssl,msl);
	assert(ssl<nslices);

	if (slice[ssl] == 0) {
		slice[ssl] = (dptPixel*)calloc(width*height,sizeof(dptPixel));
		assert(slice[ssl]);
	}

	// normalize bbox
	double a;
	if (px0 > px1)  {
		a = px0 ; px0 = px1 ; px1 = a ; 
	}
	if (py0 > py1) {
		a = py0 ; py0 = py1 ; py1 = a ; 
	}
	// render a rectangle on the selected slice. Paint all pixels

	// convert to pixel space
	int cx0 = int((px0 - x0) * sx);
	int cx1 = int((px1 - x0) * sx);
	int cy0 = int((py0 - y0) * sy);
	int cy1 = int((py1 - y0) * sy);

	// clip
	CLIP(cx0,0,width);
	CLIP(cx1,0,width);
	CLIP(cy0,0,height);
	CLIP(cy1,0,height);
	
	// is it a nil (1 pixel object?) skip.
	if ((cx0==cx1)&&(cy0==cy1)) {
		return -1;
	}

	//fprintf(stderr,"clipped %3d %3d %3d %3d\n",cx0,cy0,cx1,cy1);

	int mask = 1 << msl;
	//fprintf(stderr,"# mask=%d -> %x\n",sl,mask);

	int out ;
	for (int py=cy0 ; py <= cy1;py++) {
		for (int px=cx0 ; px <= cx1;px++) {
			int p = py*width+px;
			assert(p<width*height);
			slice[ssl][p] |= mask;
		}
	}

	return 0;
}

// write one slice or all slices packed in a dptPixel
int dpt::write_pbm(FILE *fp,int sl) {
	int mask;
	int ssl;
	ssl = sl / (sizeof(dptPixel) * 8);
	int msl = sl % (sizeof(dptPixel) * 8);
	assert(ssl<nslices);
	assert(slice[ssl]);
	mask=1 << msl;

	fprintf(fp,"P1\n");
	fprintf(fp,"# - slice %d (pixmap %d bit %d)\n",sl,ssl,msl);
	fprintf(fp,"%d %d\n",width,height);
	int n = 0;
	for(int y=0 ; y < height ; y++) {
		int k;
		int p;
		k = 0;
		for(int x=0 ; x < width ; x++) {
			fprintf(fp,"%s",(x % 20) ? " " : "\n");
			//p = 0xff & (slice[ssl][n++] & mask); //pixel
			p =slice[ssl][n++] & mask;
			fprintf(fp,"%d",p? 1: 0);
		}
	}
	fprintf(fp,"\n");
}

// generate an RGB image based on the colors
int dpt::write_image(FILE *fp) {

	//fprintf(fp,"P3\n");
	fprintf(fp,"P6\n");
	fprintf(fp,"%d %d\n",width,height);
	fprintf(fp,"255\n");
	for(int y=0 ; y < height ; y++) {
		for(int x=0 ; x < width ; x++) {
			int p = top_slice(x,y);
			unsigned int out;
			// any pixel?
			if (p != -1 ) {
			//fprintf(stderr,"%d %d %d\n", x, y, p);
				out = color[p];
			} else {
				out = 0xffffff;
			}
			//fprintf(fp,"%d %d %d ", R_COLOR(out),G_COLOR(out),B_COLOR(out));
			fprintf(fp,"%c%c%c", R_COLOR(out),G_COLOR(out),B_COLOR(out));
		}
	}
	//fprintf(fp,"\n");
}

int dpt::colorize(int slice,int rgb) {
	assert(slice < nslices);
	assert(slice >= 0);
	color[slice] = rgb;
}

// locate the top most slice at x,y
// return -1 if none
int dpt::top_slice(int x,int y) {
	// iterate through all slices
	for (int sl = nslices -1 ; sl >=0 ; sl--) {
		int ssl = sl / (sizeof(dptPixel) * 8);
		int msl = sl % (sizeof(dptPixel) * 8);
		unsigned char p = 1 & (slice[ssl][y*width+x] >> msl);
		if (p) { return sl ; }
	}
	return -1;
}

END_NAMESPACE_ADS
