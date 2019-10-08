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

#include "tm.h"

#ifndef ADS_H
#include "ads.h"
#endif

#ifdef USEJPEG
extern "C" {
#include <jpeglib.h>
}
#endif

#ifdef USEPNG
extern "C" {
#include <png.h>
}
#endif

BEGIN_NAMESPACE_ADS

typedef char dptPixel;


#define PIXMAPGRID 32

#if (PIXMAPGRID == 64)
    typedef uint64 pixint;
    typedef unsigned int pixints;
    #define PIXFILL 0xffffffffffffffff
    #define PIXMAX 0x8000000000000000
    #define PIXADJUST 2
#elif (PIXMAPGRID == 32)
    typedef unsigned int pixint;
    typedef short unsigned int pixints;
    #define PIXFILL 0xffffffff
    #define PIXMAX 0x80000000
    #define PIXADJUST 4
#endif

/* Values for the member variable _init
 * INIT = created, 
 * CONFIGURED = has reasonable values for width, height, slices, etc
 * ALLOCATED = memory has been allocated
 */
#define INIT 0
#define WIDTH 1
#define SLICES 2
#define SCALING 4
#define ALLOCATED 8

#define ALL (WIDTH | SLICES | SCALING | ALLOCATED)


#define DPT_PPM 1
#define DPT_JPEG 2
#define DPT_PNG 3

#define DPT_FILE 1
#define DPT_STRING 2

#define DPT_JPEG_SLOW 1
#define DPT_JPEG_FAST 2
#define DPT_JPEG_FLOAT 3

#define MAXPRECOLOR 15


class dpt
{
  public:
    dpt();
    ~dpt();

    int configure(int width,int height, int x0, int y0, int x1, int y1,int nslices);

	// render a rectangle
	int box(int x0, int y0, int x1, int y1, int slice);

	// write slices as a pbm file
	int write_pbm(FILE*fp,int sl=0);

	// colorize a slice
	int colorize(int slice,int rgb);

    // call to get the output
	int create_image(FILE*fp, char **out, int output, int encoding, int ftype);


    // allocate (re-allocate) memory
    int alloc_mem(void);

    // set the scaling parameters
    int scaling(int _x0, int _y0, int _x1, int _y1);

    // set the size parameters
    int setSize(int width, int height);

    // set the number of slices
    int setSlices(int _nslices);

    int get_max_slice();

    char *get_color_value(int slice);

    int get_boxcount();

#ifdef USEJPEG
    // set JPEG parameters
    void setJpegAttributes(int quality, int speed);
#endif


  protected:
    int    width;
    int    height;
    int    nslices;              // max number of slices
    int    maxslice;            // maximum used slice
	int x0,x1,y0,y1;          // bounding box
	double sx,sy;                // scale factors
    int     csize;              // size of the color table
    char    ppmheader[128];
    char    ppmheader64[128];

    char    pixbuff[4];
    char    pixbuff64[5];

    int _init;

    int _boxcount;

    struct rgb
    {
        char out[5];
    };

    struct rgb * pixcol;
    struct rgb * pixcol64;

    typedef union 
    {
        pixint lword;
        pixints word[2];
    } pixmap;

    pixmap **plptr;
    pixmap **planes;
    pixmap **plalloc;
    int pixwidth; // how many pixels pixmap is wide, upped to multiple of 
                   // PIXMAPGRID
    int pixwrem;  // how many pixels are used in the last block of a row
    int pixstride;    // how many uint64_t blocks per row
    int pixfullblox;  // how many "full" blocks per row 
                      // (equal to stride, or one less if pixhrem > 0)
    int maxplane;

    pixint start[PIXMAPGRID];
    pixint middle[PIXMAPGRID];
    pixint end[PIXMAPGRID];

    int precolor[MAXPRECOLOR+1];
    

#ifdef USEJPEG
    int jpegquality;
    J_DCT_METHOD jpegspeed;
#endif

#ifdef USEPNG
    png_color *png_palette;
#endif
    
  protected:
	inline int R_COLOR(unsigned int p) { return 0xff & ((p & 0xff0000)>>16); }
	inline int G_COLOR(unsigned int p) { return 0xff & ((p & 0x00ff00)>>8); }
	inline int B_COLOR(unsigned int p) { return 0xff & ((p & 0x0000ff)   ); }

	// find the highest slice of each pixel
    void tobase64(char *in, char *out, int len);

    void dump_row(int row, int plane);
    void dump_bytes(char *s);

    int free_mem(void);

    // set up ppm headers
    void init_headers();

    void scale_factors();

	// write all slices as a series of PBM files
	int write_string(char **s, int encoding);

#ifdef USEJPEG
    int write_jpeg_file(FILE *fp, char *src);

    int write_jpeg_string(char *src, char *output, int &len);
#endif
#ifdef USEPNG
    int write_png_file(FILE *fp, char *src);

    int write_png_string(char *src, char *output, int &len);
#endif

	// write all slices as a series of PBM files
	int write_ppm_file(FILE*fp, int encoding);

	// write all slices as a series of PBM files
	int write_ppm_string(char **s, int encoding);

    // set up initial pixcolor stuff
    void init_pixcol();
    void init_pixbuff();
};

END_NAMESPACE_ADS

#endif
