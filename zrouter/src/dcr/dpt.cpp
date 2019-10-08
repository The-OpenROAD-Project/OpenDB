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
#include "logger.h"


BEGIN_NAMESPACE_ADS

#ifdef USEJPEG
typedef struct
{
    struct jpeg_destination_mgr pub;
    int bufflen;
    char *buff;
    FILE *outfile;
} my_dest_mgr;

extern "C" void init_destbuff(jpeg_compress_struct *cinfo)
{
    my_dest_mgr *dest = (my_dest_mgr*)cinfo->dest;
    dest->pub.free_in_buffer = dest->bufflen;
    dest->pub.next_output_byte = (JOCTET*)dest->buff;
}

extern "C" void term_destination(jpeg_compress_struct *cinfo)
{
}

extern "C" boolean empty_buffer(jpeg_compress_struct *cinfo)
{
    //warning(stderr,"Empty buffer - should never get here!!\n");
    return TRUE;
}

my_dest_mgr mymgr;

#endif

#ifdef USEPNG
png_voidp png_error;
extern "C" void png_error_f(png_struct *p, const char *s) 
{
    fprintf(stderr,"png error: %s!\n",s);
}
extern "C" void png_warn_f(png_struct *p, const char *s)
{
    fprintf(stderr,"png warning: %s!\n",s);
}
png_structp png_ptr = NULL;

png_infop png_info_ptr = NULL;

char * png_dest = NULL;
int png_len = 0;

extern "C" void png_write_mydata(png_structp png_ptr, png_bytep data, png_size_t length)
{
    memcpy(png_dest, (char *)data, length);
    png_dest += length;
    png_len += length;
}

void png_flush_data(png_structp png_ptr)
{
}

#endif

// ----------------------------------------------------------------------------
//
// dpt - Methods
//
// ----------------------------------------------------------------------------
dpt::dpt()
{

    _init = INIT;

    pixint sum=PIXFILL;
	int i;
    for(i=0; i<PIXMAPGRID; i++)
    {
        start[i]=sum;
        sum = (sum>>1);
    }

    sum = PIXMAX;
    pixint s2 = sum;
    for(i=0;i<PIXMAPGRID;i++)
    {
        end[i] = sum;
        sum = (sum>>1) | PIXMAX;

        middle[i] = s2;
        s2 = (s2>>1);
    }

    nslices = -1;
    maxslice = -1;
    width = -1;
    height = -1;

    x0 = -1;
    y0 = -1;
    x1 = -1;
    y1 = -1;

#ifdef USEJPEG
    jpegquality = 75;
    jpegspeed = JDCT_IFAST;
#endif

    precolor[0] = 0xff0000;
	precolor[1] =  0x00ff00;
	precolor[2] =  0x20cfff;
	precolor[3] =  0xffff00;
	precolor[4] =  0x880000;
	precolor[5] =  0x007700;
	precolor[6] =  0x0000ff;
	precolor[7] =  0xff9000;
	precolor[8] =  0xff8080;
	precolor[9] =  0x607710;
	precolor[10] =  0x602090;
	precolor[11] =  0x808080;
	precolor[12] =  0xe040e0;
	precolor[13] =  0x88ff88;
	precolor[14] =  0x000080;
	precolor[15] =  0x805010;

    notice(0,"Initialized dpt 2!\n");
}


dpt::~dpt()
{
    free_mem();
}

void dpt::init_pixcol()
{

    for(int i=0;i<nslices;i++)
    {
        colorize(i,precolor[i]);
        if( i == MAXPRECOLOR)
            break;
    }
}

void dpt::init_pixbuff()
{
    pixbuff[0]=0x00;
    pixbuff[1]=0x00;
    pixbuff[2]=0x00;
    pixbuff[3]='\0';

    pixbuff64[0]='A';
    pixbuff64[1]='A';
    pixbuff64[2]='A';
    pixbuff64[3]='A';
    pixbuff64[4]='\0';
}

#ifdef USEJPEG
void dpt::setJpegAttributes(int quality, int speed)
{
    if( (quality > 0) && (quality <= 100) )
    {
        jpegquality = quality;
    }
    else
    {
        jpegquality = 75;
    }

    if( speed == DPT_JPEG_SLOW)
    {
        jpegspeed = JDCT_ISLOW;
    }
    else if( speed == DPT_JPEG_FAST) 
    {
        jpegspeed = JDCT_IFAST;
    }
    else if( speed == DPT_JPEG_FLOAT) 
    {
        jpegspeed = JDCT_FLOAT;
    }
    else
    {
        jpegspeed = JDCT_IFAST;
    }
}
#endif

int dpt::free_mem()
{
    if( (_init & ALLOCATED) )
    {
        notice(0,"Freeing DPT memory!\n");
        for(int s=0; s<nslices ; s++) 
        {
            free((pixmap**)plalloc[s]);
        }
        free((pixmap*)plalloc);
        free((pixmap*)planes);
        free((pixmap*)plptr);

        free( (struct rgb*) pixcol );
        free( (struct rgb*) pixcol64 );

#ifdef USEPNG
        free( (png_color *) png_palette);
#endif

        _init = (_init & (~ALLOCATED) );
        return 0;
    }
    else
    {
        return -1;
    }
}

int dpt::alloc_mem()
{
    if( (_init & ALLOCATED) )
    {
        free_mem();
    }

    if( (_init & WIDTH) && (_init & SLICES) )
    {
        notice(0,"Allocating DPT memory!\n");
        plptr  = (pixmap**) calloc(nslices,sizeof(pixmap*));
        planes  = (pixmap**) calloc(nslices,sizeof(pixmap*));
        plalloc  = (pixmap**) calloc(nslices,sizeof(pixmap*));


        pixcol = (struct rgb*)calloc(nslices,sizeof(struct rgb));
        pixcol64 = (struct rgb*)calloc(nslices,sizeof(struct rgb));

#ifdef USEPNG
        png_palette = (png_color *)calloc(nslices,sizeof(png_color));;
#endif

        for(int s=0; s<nslices ; s++) 
        {
            pixmap * pm = (pixmap*)calloc(height*pixstride+PIXADJUST,sizeof(pixmap));
            if ( pm == NULL )
            {
                fprintf(stderr,"Error: not enough memory for requested size/number of planes. Allocated %d planes out of %d.\n",s,nslices);
                fprintf(stderr,"Wanted (%d * %d + %d) * %d  = %d bytes\n",height,pixstride,PIXADJUST,(int)sizeof(pixmap),(int)((height*pixstride+PIXADJUST)*sizeof(pixmap)));
                exit(-1);
            }

            plalloc[s] = pm;

            // align on a 16-byte boundary
            pm = (pixmap*)(((char*)pm)+15);
            pm = (pixmap*) ((long)pm >> 4);
            pm = (pixmap*) ((long)pm << 4);
            planes[s] = pm;
        }

        _boxcount=0;
        _init |= ALLOCATED;
        init_pixbuff();
        init_pixcol();
        return 0;
    }
    else
    {
        return -1;
    }
}
void dpt::scale_factors()
{
	// scale factors
    if( x1 <= x0 )
    {
        // to avoid things like divide by 0, etc
        x1 = x0 + 1;
    }
	sx      = ((double)width)  / (x1 - x0);

    if (y1 <= y0 )
    {
        // to avoid things like divide by 0, etc
        y1 = y0 + 1;
    }
	sy      = ((double)height) / (y1 - y0);

    notice(0,"New scale factors: sx = %f, sy = %f\n",sx,sy);
}

int dpt::scaling(int _x0, int _y0, int _x1, int _y1)
{
	// bbox
	x0      = _x0;
	y0      = _y0;
	x1      = _x1;
	y1      = _y1;

    scale_factors();

    _init |= SCALING;

    return 0;

}

void dpt::init_headers()
{
    // Initialize headers - does NOT depend on memory allocation
    sprintf(ppmheader,"P6\n%d %d\n255\n",width,height);

    char hbuff[128];
    char hb2[4] = "";

    sprintf(hbuff,"P6");
    do 
    {
        hbuff[0]='\0';
        sprintf(hbuff,"P6%s\n%d %d\n255\n",hb2,width,height);
        strcat(hb2," ");
    } while ( (strlen(hbuff) % 3) != 0 );

    tobase64(hbuff,ppmheader64, strlen(hbuff));

}

int dpt::setSlices(int _nslices)
{
    if( nslices != _nslices )
    {
        free_mem();
        nslices = _nslices;
        maxplane = nslices-1;
        _init |= SLICES;
        alloc_mem();
    }
    return 0;
}

int dpt::setSize(int _width, int _height)
{
	width   = _width;
	height  = _height;

    pixwrem = width % PIXMAPGRID;
    pixwidth = width;
    if ( pixwrem != 0 )
    {
        // round off to next multiple of PIXMAPGRID
        pixwidth +=  (PIXMAPGRID - (width % PIXMAPGRID));
    } 

    pixstride = pixwidth / PIXMAPGRID;

    pixfullblox = pixstride;
    if( pixwrem != 0 )
    {
        pixfullblox--;
    }

    _init |= WIDTH;

    scale_factors();
    alloc_mem();
    init_headers();

    return 0;
}

int dpt::configure(int _width,int _height,
        int _x0, int _y0, int _x1, int _y1,int _nslices) {

	assert(_nslices>0);
    
    if( _init & ALLOCATED )
    {
        free_mem();
    }

    setSize(_width, _height);
    setSlices(_nslices);

    scaling(_x0, _y0, _x1, _y1);

    return 0;
}

// CLIP MACRO
#define CLIP(p,min,max) p = (p<min) ? min : (p>=max)? (max - 1) : p

int dpt::box(int px0, int py0, int px1, int py1,int sl) 
{

    if( (sl < 0 ) || (sl > nslices) )
    {
        fprintf(stderr,"Box in slice %d exceeds maximum configured slice count %d - ignored!\n",sl,nslices);
        return -1;
    }

    if( !(_init & ALL) )
    {
        return -1;
    }

    if( sl > maxslice )
    {
        maxslice = sl;
    }

    //fprintf(stderr,"Box from: %d,%d to %d,%d\n",px0,py0,px1,py1);

	// normalize bbox
	long a;
	if (px0 > px1)  {
		a = px0 ; px0 = px1 ; px1 = a ; 
	}
	if (py0 > py1) {
		a = py0 ; py0 = py1 ; py1 = a ; 
	}

    if( (px0 < x0) && (px1 < x0) )
        return -1;
    if( (px0 > x1) && (px1 > x1) )
        return -1;
    if( (py0 < y0) && (py1 < y0) )
        return -1;
    if( (py0 > y1) && (py1 > y1) )
        return -1;

	
    // convert to pixel space
	int cx0 = int((px0 - x0) * sx);
	int cx1 = int((px1 - x0) * sx);
	int cy0 = int((py0 - y0) * sy);
	int cy1 = int((py1 - y0) * sy);

	// is it a nil (1 pixel object?) skip.
	if ((cx0==cx1)&&(cy0==cy1)) {
		return -1;
	}
    // now fill in planes object
    //fprintf(stderr,"Starting new box: (%d,%d) to (%d,%d)!\n",cx0,cy0,cx1,cy1);
	// render a rectangle on the selected slice. Paint all pixels

	CLIP(cx0,0,width);
	CLIP(cx1,0,width);
	CLIP(cy0,0,height);
	CLIP(cy1,0,height);

    pixmap *pm;
    pixmap *pcb;
    int xbs = cx0 / PIXMAPGRID;
    int xbe = cx1 / PIXMAPGRID;

    //int xss = cx0 % PIXMAPGRID;
    int xee = cx1 % PIXMAPGRID;

    pixint smask  = start[cx0%PIXMAPGRID];
    pixint emask = end[xee];

    int mb;

    if ( xbe == xbs )
    {
        smask = smask & emask;
    }

    pm = planes[sl] + pixstride*cy0 + xbs;

    for( int yb = cy0; yb <= cy1; yb++)
    {
        // start block
        pcb = pm;
        
        // for next time through loop - allow compiler time for out-of-order
        pm += pixstride;

        // do "start" block
        pcb->lword = pcb->lword | smask;

        // do "middle" block
        for( mb=xbs + 1; mb< xbe;)
        {
            pcb++;
            // moved here to allow for out-of-order execution
            mb++;

            //if( pcb->lword != PIXFILL )
                pcb->lword = PIXFILL;

        }

        // do "end" block
        if ( xbe > xbs )
        {
            pcb++;
            pcb->lword = pcb->lword | emask;

        }

        //fprintf(stderr,"before: %x, after: %x (%d)\n",pm, pm+pixstride, pixstride);
    }

    _boxcount++;
    //notice(0,"Boxcount: %d\n",_boxcount);

	return 0;
}

int dpt::get_boxcount() 
{
    return _boxcount;
}

// write one slice or all slices packed in a dptPixel
int dpt::write_pbm(FILE *fp,int sl) {
#if 0
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
#endif
    return 0;
}


// generate an RGB image based on the colors
int dpt::create_image(FILE *fp, char **src, int output, int encoding, int ftype)
{

    int rc=0;
#if defined USEJPEG || defined USEPNG
    int ppmlen = strlen(ppmheader);
    char *s = NULL;
    char *o = NULL;
    char *o64;
    int len = width*height*3+128;
#endif
    // file-based output

    if( (output == DPT_FILE) )
    {
        if( ftype == DPT_JPEG )
        {
#ifdef USEJPEG
            // always needed
            rc += write_ppm_string(&s,255);
            if( encoding == 64)
            {
                o = (char*)malloc(len*sizeof(char));
                o64 = (char*)malloc(len + width*height);

                // convert to jpeg binary string
                rc += write_jpeg_string((s+ppmlen),o,len);
                free(s);
                    
                // convert to base64
                tobase64((char*)o,o64,len);
                free(o);

                int l2 = (len % 3);
                if( l2 != 0 )
                {
                    l2 = 4;
                }
                l2 = l2 + (len / 3 * 4);
                fwrite(o64,1,l2,fp);
                free(o64);
            }
            else
            {
                rc += write_jpeg_file(fp,(s+ppmlen));
                free(s);
            }
#else
            fprintf(stderr,"Jpeg Support not enabled - using ppm format!\n");
            rc += write_ppm_file(fp,encoding);
#endif
        }
        else if( ftype == DPT_PNG )
        {
#ifdef USEPNG
            // always needed
            rc += write_ppm_string(&s,255);
            if( encoding == 64)
            {
                o = (char*)malloc(len*sizeof(char));
                o64 = (char*)malloc(len + width*height);

                // convert to png binary string
                rc += write_png_string((s+ppmlen),o,len);
                free(s);
                    
                // convert to base64
                tobase64((char*)o,o64,len);
                free(o);

                int l2 = (len % 3);
                if( l2 != 0 )
                {
                    l2 = 4;
                }
                l2 = l2 + (len / 3 * 4);
                fwrite(o64,1,l2,fp);
                free(o64);
            }
            else
            {
                rc += write_png_file(fp,(s+ppmlen));
                free(s);
            }
#else
            fprintf(stderr,"PNG Support not enabled - using ppm format!\n");
            rc += write_ppm_file(fp,encoding);
#endif
        }
        else if (ftype == DPT_PPM )
        {
            rc += write_ppm_file(fp,encoding);
            notice(0,"Wrote ppm file to channel %x\n",fp);
        }
        else
        {
            fprintf(stderr,"Unknown file format!\n");
            return -1;
        }
        fflush(fp);
    }
    // string-based output
    else
    {
        if( ftype == DPT_JPEG )
        {
#ifdef USEJPEG
            // jpeg string in base-64 or 255
            o = (char*)malloc(len*sizeof(char));

            rc += write_ppm_string(&s,255);
            rc += write_jpeg_string((s+ppmlen),o,len);

            free(s);
            // have jpeg string

            if( encoding == 64 )
            {
                o64 = (char*)malloc(width*height*4+128);
                tobase64((char*)o,o64,len);
                free(o);
                int l2 = (len % 3);
                if( l2 != 0 )
                {
                    l2 = 4;
                }
                l2 = l2 + (len / 3 * 4);

                *src = o64;
            }
            else 
            {
                *src = (char*)o;
            }
            return rc;
#else
            fprintf(stderr,"Jpeg Support not enabled - using ppm format!\n");
            rc += write_ppm_string(src,encoding);
#endif
        }
        else if( ftype == DPT_PNG )
        {
#ifdef USEPNG
            // always needed
            rc += write_ppm_string(&s,255);
            if( encoding == 64)
            {
                o = (char*)malloc(len*sizeof(char));
                o64 = (char*)malloc(len + width*height);

                // convert to png binary string
                rc += write_png_string((s+ppmlen),o,len);
                free(s);
                    
                // convert to base64
                tobase64((char*)o,o64,len);
                free(o);

                int l2 = (len % 3);
                if( l2 != 0 )
                {
                    l2 = 4;
                }
                l2 = l2 + (len / 3 * 4);
                *src = o64;
            }
            else
            {
                *src = (char*)o;
            }
#else
            fprintf(stderr,"PNG Support not enabled - using ppm format!\n");
            rc += write_ppm_string(src,encoding);
#endif
        }
        else if (ftype == DPT_PPM )
        {
            rc += write_ppm_string(src,encoding);
        }
        else
        {
            fprintf(stderr,"Unknown file format!\n");
            return -1;
        }
    }

    return rc;
}

int dpt::write_ppm_file(FILE*fp, int encoding)
{

    if( !(_init & ALLOCATED) )
    {
        return -1;
    }

	//fprintf(fp,"P3\n");
    struct rgb * pixlookup;

    char * buffptr;
    int bufflen;

    if ( encoding == 255 )
    {
        pixlookup = pixcol;
        buffptr = pixbuff;
        bufflen = 3;
        fprintf(fp,"%s",ppmheader);
    } 
    else if (encoding == 64 )
    {
        pixlookup = pixcol64;
        buffptr = pixbuff64;
        bufflen = 4;
        fprintf(fp,"%s",ppmheader64);
    } 
    else 
    {
        fprintf(stderr,"Unsupported encoding style!\n");
        return -1;
    }

    // just for testing
    //memset(plalloc[3],129,height*(pixwidth+1)*sizeof(pixmap));

    pixint *bmask;
    int py; int st; int pln; int bit;

    char * outptr;
    long offset;

    //pixint *bend=&middle[PIXMAPGRID];

    for(py=height-1; py>=0; py--)
    {
        offset = py * pixstride;
        for(pln = maxplane; pln >= 0; pln--)
        {
            plptr[pln] = planes[pln]+offset;
        }
        for(st=0; st<pixfullblox; st++)
        {
            bmask = middle;
            for(bit=0;bit<PIXMAPGRID;bit++)
            {
                outptr = buffptr;
                for(pln = maxplane; pln >=0; pln--)
                {
                    if( (plptr[pln]->lword) & (*bmask) )
                    {
                        outptr = pixlookup[pln].out;
                        break;
                    }
                }
                fwrite(outptr,1,bufflen,fp);
                bmask++;
            }
            for(pln = maxplane; pln >=0; pln--)
            {
                plptr[pln]++;
            }
        }
        // handle non-even case
        bmask = middle;
        for(bit=0;bit<pixwrem;bit++)
        {
            outptr = buffptr;
            for(pln = maxplane; pln >=0; pln--)
            {
                if( (plptr[pln]->lword) & (*bmask) )
                {
                    outptr = pixlookup[pln].out;
                    break;
                }
            }
            fwrite(outptr,1,bufflen,fp);
            bmask++;
        }
    }

    fflush(fp);
    return 0;

}

int dpt::write_ppm_string(char **s, int encoding) 
{
    if( !(_init & ALLOCATED) )
    {
        return -1;
    }

	//fprintf(fp,"P3\n");
    struct rgb * pixlookup;

    char * buffptr;
    int bufflen;

    char *stout;

    if ( encoding == 255 )
    {
        pixlookup = pixcol;
        buffptr = pixbuff;
        bufflen = 3;
        // add a little extra for base-64 padding and header
        *s = (char*)malloc(bufflen * width * height+128);
        stout = *s;
        sprintf(stout,"%s",ppmheader);
    } 
    else if (encoding == 64 )
    {
        pixlookup = pixcol64;
        buffptr = pixbuff64;
        bufflen = 4;
        // add a little extra for base-64 padding and header
        *s = (char*)malloc(bufflen * width * height+128);
        stout = *s;
        sprintf(stout,"%s",ppmheader64);
    } 
    else 
    {
        fprintf(stderr,"Unsupported encoding style!\n");
        return -1;
    }
    //fprintf(stderr,"Starting string write!\n");
    //dump_bytes(stout);
    stout = (stout + strlen(stout));

    // just for testing
    //memset(plalloc[3],129,height*(pixwidth+1)*sizeof(pixmap));

    pixint *bmask;
    int py; int st; int pln; int bit;

    char * outptr;
    long offset;

    //pixint *bend=&middle[PIXMAPGRID];
    int buffcopy;

    for(py=height-1; py>=0; py--)
    {
        offset = py * pixstride;
        for(pln = maxplane; pln >= 0; pln--)
        {
            plptr[pln] = planes[pln]+offset;
        }
        for(st=0; st<pixfullblox; st++)
        {
            bmask = middle;
            for(bit=0;bit<PIXMAPGRID;bit++)
            {
                outptr = buffptr;
                for(pln = maxplane; pln >=0; pln--)
                {
                    if( (plptr[pln]->lword) & (*bmask) )
                    {
                        outptr = pixlookup[pln].out;
                        break;
                    }
                }
                for(buffcopy=0; buffcopy<bufflen; buffcopy++)
                {
                    *stout++=*outptr++;
                }
                bmask++;
            }
            for(pln = maxplane; pln >=0; pln--)
            {
                plptr[pln]++;
            }
        }
        // handle non-even case
        bmask = middle;
        for(bit=0;bit<pixwrem;bit++)
        {
            outptr = buffptr;
            for(pln = maxplane; pln >=0; pln--)
            {
                if( (plptr[pln]->lword) & (*bmask) )
                {
                    outptr = pixlookup[pln].out;
                    break;
                }
            }
            for(buffcopy=0; buffcopy<bufflen; buffcopy++)
            {
                *stout++=*outptr++;
            }
            bmask++;
        }
    }
    *stout='\0';
    //fprintf(stderr,"Done string write!\n");
    //dump_bytes(*s);

    return 0;
}

int dpt::get_max_slice()
{
    return maxslice;
}

char *dpt::get_color_value(int slice)
{
    if( (slice < 0) || (slice > nslices) )
    {
        return NULL;
    }
    return pixcol[slice].out;
}

int dpt::colorize(int slice,int rgb) {

    if( !(_init & ALLOCATED) )
    {
        return -1;
    }
    if ( slice >= nslices )
        return 0;

	assert(slice >= 0);
    char buff[4];
    sprintf(buff,"%c%c%c",R_COLOR(rgb),G_COLOR(rgb),B_COLOR(rgb));
    pixcol[slice].out[0]=buff[0];
    pixcol[slice].out[1]=buff[1];
    pixcol[slice].out[2]=buff[2];
    pixcol[slice].out[3]='\0';
#ifdef USEPNG
    png_palette[slice].red = R_COLOR(rgb);
    png_palette[slice].green = G_COLOR(rgb);
    png_palette[slice].blue = B_COLOR(rgb);
#endif

    tobase64(pixcol[slice].out, pixcol64[slice].out,3);

    //fprintf(stderr,"Encoded %x to %s (%d)\n",rgb, pixcol64[slice].out,slice);
    return 0;
}

#ifdef USEJPEG

int dpt::write_jpeg_file(FILE *fp, char *src)
{
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;

    //fprintf(stderr,"Writing jpeg!\n");
    //dump_bytes(src);

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);


    jpeg_stdio_dest(&cinfo, fp);

    cinfo.image_width = width;  /* image width and height, in pixels */
    cinfo.image_height = height;
    cinfo.input_components = 3; /* # of color components per pixel */
    cinfo.in_color_space = JCS_RGB; /* colorspace of input image */

    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo,jpegquality,TRUE);
    cinfo.dct_method = (J_DCT_METHOD)jpegspeed;

    jpeg_start_compress(&cinfo, TRUE);

    JSAMPROW row_pointer[1];    /* pointer to a single row */
    int row_stride;         /* physical row width in buffer */

    row_stride = width * 3;   /* JSAMPLEs per row in image_buffer */

    while (cinfo.next_scanline < cinfo.image_height) {
        row_pointer[0] = (JSAMPROW)& src[cinfo.next_scanline * row_stride];
        jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }

    jpeg_finish_compress(&cinfo);
    jpeg_destroy_compress(&cinfo);

    return 0;
}


int dpt::write_jpeg_string(char *src, char *out, int &len)
{
    struct jpeg_compress_struct *cinfo = 
        (jpeg_compress_struct*)malloc(sizeof(jpeg_compress_struct));
    struct jpeg_error_mgr jerr;


    cinfo->err = jpeg_std_error(&jerr);

    jpeg_create_compress(cinfo);

    cinfo->image_width = width;  /* image width and height, in pixels */
    cinfo->image_height = height;
    cinfo->input_components = 3; /* # of color components per pixel */
    cinfo->in_color_space = JCS_RGB; /* colorspace of input image */

    jpeg_set_defaults(cinfo);
    jpeg_set_quality(cinfo,jpegquality,TRUE);
    cinfo->dct_method = (J_DCT_METHOD)jpegspeed;

    if (cinfo->dest == NULL) 
    {    /* first time for this JPEG object? */
        cinfo->dest = (struct jpeg_destination_mgr *)
        (*cinfo->mem->alloc_small)
        ((j_common_ptr) cinfo, JPOOL_PERMANENT, sizeof(my_dest_mgr));
    }

    my_dest_mgr *jdest;
    jdest = (my_dest_mgr*)cinfo->dest;
    jdest->pub.init_destination = init_destbuff;
    jdest->pub.term_destination = term_destination;
    jdest->pub.empty_output_buffer = empty_buffer;

    jdest->bufflen = len;
    jdest->buff = out;


    jpeg_start_compress(cinfo, TRUE);

    JSAMPROW row_pointer[1];    /* pointer to a single row */
    int row_stride;         /* physical row width in buffer */

    row_stride = width * 3;   /* JSAMPLEs per row in image_buffer */

    while (cinfo->next_scanline < cinfo->image_height) {
        row_pointer[0] = (JSAMPROW)& src[cinfo->next_scanline * row_stride];
        jpeg_write_scanlines(cinfo, row_pointer, 1);
    }

    jpeg_finish_compress(cinfo);

    len = (jdest->pub.next_output_byte - (JOCTET*)out);

    jpeg_destroy_compress(cinfo);

    return 0;
}
#endif

#ifdef USEPNG

int dpt::write_png_file(FILE *fp, char *src)
{
    png_ptr = png_create_write_struct (PNG_LIBPNG_VER_STRING, (png_voidp)png_error, png_error_f, png_warn_f);
    if( png_ptr == NULL )
    {
        png_error_f(png_ptr,"Initial");
        return -1;
    }
    png_info_ptr = png_create_info_struct(png_ptr);
    if( png_info_ptr == NULL )
    {
        png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
        png_error_f(png_ptr,"Second");
        return -1;
    }

    if (setjmp(png_jmpbuf(png_ptr)))
    {
        png_destroy_write_struct(&png_ptr, (png_infopp)&png_info_ptr);
        return -1;
    }

    png_init_io(png_ptr, fp);

    png_set_IHDR(png_ptr, png_info_ptr, width, height, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    png_set_PLTE(png_ptr, png_info_ptr, png_palette, nslices);

    png_write_info(png_ptr, png_info_ptr);

    int row_stride;         /* physical row width in buffer */

    row_stride = width * 3;   /* BYTES per row in image_buffer */

    png_byte *row_pointer[1];    /* pointer to a single row */
    
    int cur_row = 0;

    while (cur_row < height)
    {
        row_pointer[0] = (png_byte*)& src[cur_row * row_stride];
        png_write_rows(png_ptr, row_pointer, 1);
        cur_row++;
    }

    png_write_end(png_ptr, png_info_ptr);
    png_destroy_write_struct(&png_ptr, &png_info_ptr);

    return 0;
}

//TODO - test!
int dpt::write_png_string(char *src, char *out, int &len)
{
    png_ptr = png_create_write_struct (PNG_LIBPNG_VER_STRING, (png_voidp)png_error, png_error_f, png_warn_f);
    if( png_ptr == NULL )
    {
        png_error_f(png_ptr,"Initial");
        return -1;
    }
    png_info_ptr = png_create_info_struct(png_ptr);
    if( png_info_ptr == NULL )
    {
        png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
        png_error_f(png_ptr,"Second");
        return -1;
    }

    if (setjmp(png_jmpbuf(png_ptr)))
    {
        png_destroy_write_struct(&png_ptr, (png_infopp)&png_info_ptr);
        return -1;
    }

    png_dest = out;
    png_len = 0;

    png_set_write_fn( png_ptr, NULL, png_write_mydata, png_flush_data);

    png_set_IHDR(png_ptr, png_info_ptr, width, height, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    png_set_PLTE(png_ptr, png_info_ptr, png_palette, nslices);


    png_write_info(png_ptr, png_info_ptr);

    int row_stride;         /* physical row width in buffer */

    row_stride = width * 3;   /* BYTES per row in image_buffer */

    png_byte *row_pointer[1];    /* pointer to a single row */
    
    int cur_row = 0;

    while (cur_row < height)
    {
        row_pointer[0] = (png_byte*)& src[cur_row * row_stride];
        png_write_rows(png_ptr, row_pointer, 1);
        cur_row++;
    }

    png_write_end(png_ptr, png_info_ptr);
    png_destroy_write_struct(&png_ptr, &png_info_ptr);

    len = png_len;

    return 0;
}
#endif

char *alphabet="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

void dpt::tobase64(char *in, char *out, int len)
{
    // assume we have enough space!
    char *ptr = in;
    int rem = len % 3;

    char *end = in+len;

    unsigned char s1, s2, s3;
    unsigned char b1, b2, b3, b4;

    while( (ptr+3) < end)
    {
        s1 = *ptr++ & 0xff;
        s2 = *ptr++ & 0xff;
        s3 = *ptr++ & 0xff;

        //fprintf(stderr,"%x %x %x: ",s1,s2,s3);

        b1 = (s1 >> 2);
        b2 = ((s1 & 0x03) << 4) | (s2 >> 4);
        b3 = ( (s2 & 0x0f) << 2 ) | (s3 >> 6);
        b4 = (s3 & 0x3f);

        //fprintf(stderr,"-> %x, %x, %x, %x\n",b1,b2,b3,b4);

        *out++ = alphabet[ b1 ];
        *out++ = alphabet[ b2 ];
        *out++ = alphabet[ b3 ];
        *out++ = alphabet[ b4 ];
    }

    switch(rem)
    {
        // have 3 more to do
        case 0:
            s1 = *ptr++ & 0xff;
            s2 = *ptr++ & 0xff;
            s3 = *ptr++ & 0xff;

            //fprintf(stderr,"%x %x %x: ",s1,s2,s3);

            b1 = (s1 >> 2);
            b2 = ((s1 & 0x03) << 4) | (s2 >> 4);
            b3 = ( (s2 & 0x0f) << 2 ) | (s3 >> 6);
            b4 = (s3 & 0x3f);

            //fprintf(stderr,"-> %x, %x, %x, %x\n",b1,b2,b3,b4);

            *out++ = alphabet[ b1 ];
            *out++ = alphabet[ b2 ];
            *out++ = alphabet[ b3 ];
            *out++ = alphabet[ b4 ];
            break;

        case 1:
            s1 = *ptr++ & 0xff;
            b1 = (s1 >> 2);
            b2 = ((s1 & 0x03) << 4) ;
            *out++ = alphabet[ b1 ];
            *out++ = alphabet[ b2 ];
            *out++ = '=';
            *out++ = '=';
            break;

        case 2:
            s1 = *ptr++ & 0xff;
            s2 = *ptr++ & 0xff;

            b1 = (s1 >> 2);
            b2 = ((s1 & 0x03) << 4) | (s2 >> 4);
            b3 = ( (s2 & 0x0f) << 2 ) ;
            *out++ = alphabet[ b1 ];
            *out++ = alphabet[ b2 ];
            *out++ = alphabet[ b3 ];
            *out++ = '=';
            break;
    }
    


    *out = '\0';
}
void dpt::dump_bytes(char *s)
{
    fprintf(stderr,"First values:\n");
    if( s == NULL )
    {
        fprintf(stderr,"s is null!\n");
        return;
    }
    for(int i=0; i<width; i++)
    {
        fprintf(stderr,"%d: %x %x %x ",i,*(s+i),*(s+i+1),*(s+i+2));
        if( (i % 2) == 0 )
        {
            fprintf(stderr,"\n");
        }
    }
}

void dpt::dump_row(int row, int plane)
{

    if( !(_init & ALLOCATED) )
    {
        return;
    }
    pixmap * pm = planes[plane]+(row*pixstride);
    for(int i=0;i<pixstride;i++)
    {
        fprintf(stderr,"element %d: %x\n",i,uint(pm->lword));
        pm++;
    }
}


END_NAMESPACE_ADS
