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
#include <string.h>
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
    free((int*)color);
    free((struct rgb*)cl2);
}


int dpt::configure(int _width,int _height,
        int _x0, int _y0, int _x1, int _y1,int _nslices) {

	assert(_nslices>0);

	width   = _width;
	height  = _height;
	nslices = _nslices;
	slice   = (dptPixel**) calloc(nslices,sizeof(nslices));
    maxsll = nslices % (sizeof(dptPixel) * 8);
    csize = (1<<nslices);
    if ( nslices > MAXCOLORSLICES )
    {
        color = (int*) calloc(nslices,sizeof(int));
        cl2 = (struct rgb*)calloc(nslices, sizeof(struct rgb));
    }
    else
    {
        color   = (int*) calloc(csize,sizeof(int));
        cl2 = (struct rgb*)calloc(csize, sizeof(struct rgb));
        cl2[0].out[0]=0xff;
        cl2[0].out[1]=0xff;
        cl2[0].out[2]=0xff;
        cl2[0].out[3]='\0';
    }

	for(int s=0; s<nslices ; s++) slice[s] = 0;

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

int dpt::box(int px0, int py0, int px1, int py1,int sl) {
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
	long a;
	if (px0 > px1)  {
		a = px0 ; px0 = px1 ; px1 = a ; 
	}
	if (py0 > py1) {
		a = py0 ; py0 = py1 ; py1 = a ; 
	}
	// render a rectangle on the selected slice. Paint all pixels
	CLIP(px0,x0,x1);
	CLIP(px1,x0,x1);
	CLIP(py0,y0,y1);
	CLIP(py1,y0,y1);
	
	// is it a nil (1 pixel object?) skip.
	if ((px0==px1)&&(py0==py1)) {
		return -1;
	}

	// convert to pixel space
	int cx0 = int((px0 - x0) * sx);
	int cx1 = int((px1 - x0) * sx);
	int cy0 = int((py0 - y0) * sy);
	int cy1 = int((py1 - y0) * sy);

    int xdiff = cx1 - cx0;
    int ydiff = cy1 - cy0;

	//fprintf(stderr,"clipped %3d %3d %3d %3d\n",cx0,cy0,cx1,cy1);
	int mask = 1 << msl;
	//fprintf(stderr,"# mask=%d -> %x\n",sl,mask);

	int out;
    int o2;
    int px;

    if( ydiff > xdiff )
    {
        for (int py=cy0 ; py <= cy1;py++) {
            out = py*width;
            o2 = cx1+out;
            for (px=cx0+out ; px <= o2;px++) {
                //int p = py*width+px;
                //assert(p<width*height);
                //printf("Setting pixel 1: %d\n", px);
                // Add pointer reference!!
                slice[ssl][px] |= mask;
            }
        }
    } 
    else 
    {
        int ys = cy0 * width;
        int xs = ys + cx0;
        int xe = ys + cx1;

        int ye2;
        int yoff = ydiff * width;
        //fprintf (stdout,"%d,%d to %d,%d\n",cx0,cy0,cx1,cy1);
        //fprintf (stderr,"%d %d %d %d\n",xs, xe, yoff, width);

        for (px=xs ; px <= xe ;px++) {
            ye2 = px + yoff;
            for (int py=px ; py <= ye2;py+=width) {
                slice[ssl][py] |= mask;
            }
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

    //dump_color();
	//fprintf(fp,"P3\n");
	fprintf(fp,"P6\n");
	fprintf(fp,"%d %d\n",width,height);
	fprintf(fp,"255\n");
    int offset;
    unsigned int out;
    unsigned int xp = 0;
    int p = 0;
    struct rgb * rp;

    int x,y;
    if (nslices > MAXCOLORSLICES )
    {
        for(y=height-1 ; y >= 0 ; y--) {
            offset = y*width;
            for(x=0 ; x < width ; x++) {
                for(int sl=maxsll; sl>=0; sl--)
                {
                    if ( xp = (slice[sl][offset+x]) & 0x000000FF )
                    {
                        __asm__ __volatile__(   "bsrl %1,%0"
                                          : "=r"(p)
                                          : "Ir"(xp)
                                          : "cc"
                                          );

                        break;
                    }
                }

                fwrite(cl2[p].out,1,3,fp);
            }
        }

    }
    else
    {
        for(y=height-1 ; y >= 0 ; y--) {
            offset = y*width;
            for(x=0 ; x < width ; x++) {
                for(int sl=maxsll; sl>=0; sl--)
                {
                    if( (xp = (unsigned char)(slice[sl][offset+x]))) 
                    {
                        break;
                    }
                }
                fwrite(cl2[xp].out,1,3,fp);
            }
        }
    }
	//fprintf(fp,"\n");
}

int dpt::colorize(int slice,int rgb) {
	assert(slice < nslices);
	assert(slice >= 0);
    char buff[4];
    sprintf(buff,"%c%c%c",R_COLOR(rgb),G_COLOR(rgb),B_COLOR(rgb));
    if ( nslices > MAXCOLORSLICES) 
    {
        //color[slice] = rgb;
        cl2[slice].out[0]=buff[0];
        cl2[slice].out[1]=buff[1];
        cl2[slice].out[2]=buff[2];
    }
    else
    {
        if ( slice == 0 ) 
        {
            //color[1]=rgb;
            cl2[1].out[0]=buff[0];
            cl2[1].out[1]=buff[1];
            cl2[1].out[2]=buff[2];
        }
        else
        {
            int min =(1<<slice);
            int max = (1<<(slice+1));

            for ( int i=min; i<max; i++)
            {
                //color[i] = rgb;
                cl2[i].out[0]=buff[0];
                cl2[i].out[1]=buff[1];
                cl2[i].out[2]=buff[2];
            }
        }
    }

}

void dpt::dump_color()
{
    fprintf(stderr,"Color Table Dump\n");
    int max = (1<<(nslices+1));
    for (int i=0; i<max; i++)
    {
        fprintf(stderr,"cl2[%d]  %x, g %x, b %x\n",i,cl2[i].out[0],cl2[i].out[1],cl2[i].out[2]);
    }
}

END_NAMESPACE_ADS
