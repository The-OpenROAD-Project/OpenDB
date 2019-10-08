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
#include <time.h>
#include <unistd.h>

#include "gs.h"

USING_NAMESPACE_ADS;

int main(int argc,char**argv) {
	gs *d = new gs();
	int x0,y0,x1,y1;
	int w,h,s,slice,rgb;
    int xres,yres;
    int ns;
	char line[1024];
	int n = 0;
	time_t stime;
	time_t etime;

	time(&stime);

    int ll[2];
    int ur[2];

    int pl1, pl2, pl3;
    Ath__array1D<SEQ *> *array = NULL;
    SEQ *seq;
	// process input for the following commands:
    int enc = 255;
    int row;
    FILE *input;

    if ( argc <= 1 )
    {
        input = stdin;
    }
    else
    {
        fprintf(stderr,"Using file: %s\n",argv[1]);
        input = fopen(argv[1],"r");
    }

	while (fgets(line,sizeof(line),input)) {
		switch (line[0]) {
            case '#':
                // comment
                continue;
			case 'c': 
				// c slice xres yres x0 y0 x1 y1 
				if (sscanf(line,"c %d %d %d %d %d %d %d",
					&slice,&xres,&yres,&x0,&y0,&x1,&y1)!=7) {
					assert(0);
				}
                fprintf(stderr,"Calling configure with: %d, %d, %d, %d, %d, %d, %d\n",slice,xres,yres,x0,y0,x1,y1);
				d->configureSlice(slice,xres,yres,x0,y0,x1,y1);
				break;

			case 'C' : 
				// C slice hexcolor
				if (sscanf(line,"C %d %x",&slice,&rgb) != 2) {
					assert(0);
				}
                //fprintf(stderr,"Color: %d/%d\n",slice,rgb);
                //fflush(stderr);
				d->colorize(slice,rgb);
				break;

            case 'd':
                if ( sscanf(line,"d %d %d",&row,&slice) == 2 ) {
                    d->dump_row(row,slice);
                }
                break;


			case 'b' : 
				// b x0 y0 x1 y1 slice (box)
				if (sscanf(line,"b %d %d %d %d %d",&x0,&y0,&x1,&y1,&s)!=5 ){
					assert(0);
				}
				d->box(x0,y0,x1,y1,s);
				n++;
                //if ( n > 10000) exit (-1);
				break;

            case 'i':
                if ( sscanf(line,"i %d %d %d",&pl1,&pl2,&pl3) != 3 ) {
                    assert(0);
                }
                d->intersect_rows(pl1,pl2,pl3);
                break;

            case 'l':
                if ( sscanf(line,"l %d",&s) != 1 ) {
                    assert(0);
                }
                d->clear_layer(s);
                break;


            case 'r' :
                int plane;
                uint black;
                int start;
                int rowcol;
				if (sscanf(line,"r %d %d %d %d %d",&x0,&y0,&x1,&y1,&rowcol)!=5 ){
					assert(0);
				}
                start=0;
                ll[0]=x0;
                ll[1]=y0;
                ur[0]=x1;
                ur[1]=y1;

                for(plane=0;plane<ns;plane++)
                {
                    array = new Ath__array1D<SEQ*>();
                    start = 0;
                    fprintf(stderr,"Plane: %d\n",plane);
                    black = d->get_seq(ll,ur,rowcol,plane,array);
                    fprintf(stderr,"Numblack: %u\n",black);
                    for(unsigned int i=0; i<array->getCnt(); i++)
                    {
                        seq = (SEQ*)array->get(i);
                        d->show_seq(seq);
                    }
                }


                break;

            case 'p':
                d->check_mem();
                break;

            case 'n':
                if ( sscanf(line,"n %d",&ns) == 1 ) {
                    d->setSlices(ns);
                }
                break;


            case 'u':
                if ( sscanf(line,"u %d %d %d",&pl1,&pl2,&pl3) != 3 ) {
                    assert(0);
                }
                d->union_rows(pl1,pl2,pl3);
                break;

            case 'x':
                if ( sscanf(line,"x %d %d %d",&pl1,&pl2,&pl3) != 3 ) {
                    assert(0);
                }
                d->xor_rows(pl1,pl2,pl3);
                break;

			case 'w' :
				sscanf(line,"w %d %d %d %d %d %d",&w, &h, &ll[0], &ll[1], &ur[0], &ur[1]);
                d->create_image(stdout,NULL,GS_FILE,255,w,h,ll,ur);
				break;

			default:
				fprintf(stderr,"ignoring line: %s\n",line);
			break;
		}
	}
	time(&etime);
	//fprintf(stderr, "dtest: rendered %4d objects @ %6.2f o/sec\n", n, n / (double)(etime - stime));
    if( array != NULL )
        delete array;

    delete d;

}
