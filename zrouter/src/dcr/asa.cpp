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

#include "asa.h"
#include <ctype.h>
#include <errno.h>

//BEGIN_NAMESPACE_ADS

#ifdef DEBUG
define error(x,y) fprintf(stderr,y)
#endif

char *hier[128];

char *tlist[128];
dcrASA::dcrASA()
{
    reset();

    for( int i=0; i<128; i++)
    {
        hier[i] = tlist[i] = NULL;
    }
    hier['b'] = strdup("block");
    hier['i'] = strdup("inst");
    hier['s'] = strdup("signal");
    hier['p'] = strdup("power");

    tlist['a'] = strdup("rarrow");
    tlist['b'] = strdup("bbox");
    tlist['c'] = strdup("bcut");
    tlist['l'] = strdup("larrow");
    tlist['o'] = strdup("obs");
    tlist['p'] = strdup("pin");
    tlist['r'] = strdup("track");
    tlist['t'] = strdup("tcut");
    tlist['v'] = strdup("via");
    tlist['w'] = strdup("wire");
}


dcrASA::~dcrASA()
{
    free(hier['b']);
    free(hier['i']);
    free(hier['s']);
    free(hier['p']);

    free(tlist['a']);
    free(tlist['b']);
    free(tlist['c']);
    free(tlist['l']);
    free(tlist['o']);
    free(tlist['p']);
    free(tlist['r']);
    free(tlist['t']);
    free(tlist['v']);
    free(tlist['w']);
}

void dcrASA::reset()
{
    _fp = NULL;
    _interp = NULL;
    _ls = false;
    _dest = UNKNOWN;
}

static int xs = 0;
static int ys = 0;
static int xe = 0;
static int ye = 0;

void dcrASA::setOutput(FILE *fp)
{
    _fp = fp;
#ifndef _WIN32
    setbuffer(fp,fbuff,sizeof(fbuff));
#endif
    _dest = CHANNEL;
    xs = 0;
    ys = 0;
    xe = 0;
    ye = 0;
}

void dcrASA::setOutput(Tcl_Interp *interp)
{
    _interp = interp;
    _dest = INTERP;
    Tcl_ResetResult(_interp);
    xs = 0;
    ys = 0;
    xe = 0;
    ye = 0;
}

// format as: { x }, removing any existing { }
char *dcrASA::toList(char *x)
{
    int len = strlen(x);
    char * y = (char*)malloc(len+3); // +3 for { }, and \0

    char *sptr = x;
    char *eptr = x + len - 1;

    while( (sptr <= eptr) )
    {
        if ( isspace(*sptr) )
        {
            sptr++;
        }
        else if ( isspace(*eptr) )
        {
            eptr--;
        }
        else
        {
            break;
        }
    }
    y[0]     ='{';
    y[1]='\0';
    strncpy((y+1),sptr,eptr-sptr+1);
    y[eptr-sptr+2] ='}';
    y[eptr-sptr+3]='\0';

    return y;
}


void dcrASA::appendRecord(char *id, int layer, char *hierarchy, char *type, 
                  int x0, int y0, int x1, int y1, char *special)
{
    if ( _dest == INTERP)
    {
        if( id != NULL )
        {
            sprintf(pbuff,
            "{id %s type %s hierarchy %s bb {%d %d %d %d} layer %d",
            id, type, hierarchy, x0, y0, x1, y1, layer);
        }
        else
        {
            sprintf(pbuff,
            "{id 0 type %s hierarchy %s bb {%d %d %d %d} layer %d",
            type, hierarchy, x0, y0, x1, y1, layer);
        }

        if( special != NULL )
        {
            strcat(pbuff," ");
            strcat(pbuff,special);
        }
		strcat(pbuff,"}\n");
        Tcl_AppendResult(_interp,pbuff,NULL);
    }
    else
    {
#if 0
        // Compatible IO
        fputc('{',_fp);
        fputs("id 0 type ",_fp);
        fputs(type,_fp);
        fputs(" hierarchy ",_fp);
        fputs(hierarchy, _fp);
        fputs(" bb {", _fp);
        fprintf(_fp,"%d %d %d %d",x0,y0,x1,y1);
        fputs("} layer ",_fp);
        fprintf(_fp,"%d", layer);
        if( special != NULL )
        {
            fputs(special,_fp);
        }
        fputc('}',_fp);
        fputc('\n',_fp);
#endif
#if 0
        // Compressed IO
        fprintf(_fp,"%d ",layer);
        fputs(hierarchy, _fp);
        fputc(' ',_fp);
        fputs(type,_fp);
        fputc(' ',_fp);
        fprintf(_fp,"%d %d %d %d",x0,y0,x1-x0,y1-y0);
        if( special != NULL )
        {
            fputc(' ',_fp);
            fputs(special,_fp);
        }
        fputc('\n',_fp);
#endif
#if 1
        // Super compressed IO
        long diffxs;
        long diffys;
        int diffxe;
        int diffye;
        fprintf(_fp,"%d ",layer);

        // hierarchy
        char c = hierarchy[0];
        if( hier[c] != NULL
            && (strcmp(hier[c],hierarchy)) == 0 )
        {
            fputc(c,_fp);
        }
        else
        {
            fputs(hierarchy, _fp);
        }
        fputc(' ',_fp);

        c = type[0];
        if( (tlist[c] != NULL)
            && (strcmp(tlist[c],type)) == 0 )
        {
            fputc(c,_fp);
        }
        else
        {
            c = type[1];
            if( (tlist[c] != NULL)
                && (strcmp(tlist[c],type)) == 0 )
            {
                fputc(c,_fp);
            }
            else
            {
                fputs(type, _fp);
            }
        }


        diffxs = (long)x0 - (long)xs;
        diffys = (long)y0 - (long)ys;

        diffxe = (x1-x0) - xe;
        diffye = (y1-y0) - ye;

        fputc(' ',_fp);
        if ( (xs == 0 && ys == 0 ) || labs(diffxs) > (long)INT_MAX || labs(diffys) > (long)INT_MAX)
        {
            //fprintf(_fp," =%d %d %d %d",x0,y0,diffxe,diffye);
            //fprintf(stderr," =%d %d %d %d\n",x0,y0,diffxe,diffye);
            fputc(ASA_EQ,_fp);
            b100(x0,_fp);
            fputc(B100_ESCAPE,_fp);

            b100(y0,_fp);
            fputc(B100_ESCAPE,_fp);

            b100(diffxe,_fp);
            fputc(B100_ESCAPE,_fp);

            b100(diffye,_fp);
        } 
        else
        {
            //fprintf(_fp," %d %d %d %d",(int)diffxs,(int)diffys,diffxe, diffye);
            //fprintf(stderr," %d %d %d %d\n",(int)diffxs,(int)diffys,diffxe, diffye);
            b100(diffxs,_fp);
            fputc(B100_ESCAPE,_fp);

            b100(diffys,_fp);
            fputc(B100_ESCAPE,_fp);

            b100(diffxe,_fp);
            fputc(B100_ESCAPE,_fp);

            b100(diffye,_fp);
        }
        xs = x0;
        ys = y0;
        xe = (x1-x0);
        ye = (y1-y0);


        if( special != NULL )
        {
            fputc(B100_ESCAPE,_fp);
            fputs(special,_fp);
        }
        fputc('\n',_fp);
#endif
    }

}



int mylayer=0;
void dcrASA::setLayer(int layer)
{
    mylayer = layer;
    if( _dest == CHANNEL )
    {
        fprintf(_fp,"%c%d\n",ASA_LAYER,layer);
    }
}


char *myhier = NULL;
void dcrASA::setHier(char *hierarchy)
{
    //fprintf(stderr,"ASA, setHier: %s\n",hierarchy);
    if( _dest == CHANNEL )
    {
        char c = hierarchy[0];
        fputc(ASA_HIER,_fp);
        if( hier[c] != NULL && (strcmp(hier[c],hierarchy)) == 0 )
        {
            fputc(c,_fp);
        }
        else
        {
            fputs(hierarchy,_fp);
        }
        fputc('\n',_fp);
    }
    else
    {
        if( myhier != NULL)
        {
            free(myhier);
        }
        myhier = strdup(hierarchy);
    }
}

char *mytype = NULL;
void dcrASA::setType(char *type)
{
    //fprintf(stderr,"ASA, setType: %s\n",type);
    if( _dest == CHANNEL )
    {
        fputc(ASA_TYPE,_fp);
        char c = type[0];
        if( (tlist[c] != NULL)
            && (strcmp(tlist[c],type)) == 0 )
        {
            fputc(c,_fp);
        }
        else
        {
            c = type[1];
            if( (tlist[c] != NULL)
                && (strcmp(tlist[c],type)) == 0 )
            {
                fputc(c,_fp);
            }
            else
            {
                fputs(type, _fp);
            }
        }
        fputc('\n',_fp);
    }
    else
    {
        if( mytype != NULL)
        {
            free(mytype);
        }
        mytype = strdup(type);
    }
}

//#define NOB100
void dcrASA::appendRecord(char *id, int x0, int y0, int x1, int y1, char *special)
{
    // Super compressed IO
    //fprintf(stderr,"ABS: %d %d %d %d %s\n",x0,y0,x1,y1,special);
    if ( _dest == INTERP)
    {
        if( id != NULL )
        {
            sprintf(pbuff, 
                "{id %s type %s hierarchy %s bb {%d %d %d %d} layer %d",
                id, mytype, myhier, x0, y0, x1, y1, mylayer);
        }
        else
        {
            sprintf(pbuff, 
                "{id 0 type %s hierarchy %s bb {%d %d %d %d} layer %d",
                mytype, myhier, x0, y0, x1, y1, mylayer);
        }

        if( special != NULL )
        {
            strcat(pbuff," ");
            strcat(pbuff,special);
        }
		strcat(pbuff,"}\n");
        Tcl_AppendResult(_interp,pbuff,NULL);
    }
    else
    {

        long diffxs = (long)x0 - (long)xs;
        long diffys = (long)y0 - (long)ys;

        int diffxe = (x1-x0) - xe;
        int diffye = (y1-y0) - ye;

        //fprintf(stderr,"Diff: %d %d %d %d\n",diffxs, diffys, diffxe, diffye);
        if ( (xs == 0 && ys == 0 ) || 
                labs(diffxs) > (long)INT_MAX || labs(diffys) > (long)INT_MAX) 
        {
         //   fprintf(stderr," =%d %d %d %d\n",x0,y0,diffxe,diffye);
#ifdef NOB100
            fprintf(_fp,"=%d %d %d %d",x0,y0,diffxe,diffye);
#else
            fputc(ASA_EQ,_fp);
            b100(x0,_fp);
            fputc(B100_ESCAPE,_fp);

            b100(y0,_fp);
            fputc(B100_ESCAPE,_fp);

            b100(diffxe,_fp);
            fputc(B100_ESCAPE,_fp);

            b100(diffye,_fp);
#endif
        } 
        else
        {
          //  fprintf(stderr," %d %d %d %d\n",(int)diffxs,(int)diffys,diffxe, diffye);
#ifdef NOB100
            fprintf(_fp,"%d %d %d %d",(int)diffxs,(int)diffys,diffxe, diffye);
#else
            b100(diffxs,_fp);
            fputc(B100_ESCAPE,_fp);

            b100(diffys,_fp);
            fputc(B100_ESCAPE,_fp);

            b100(diffxe,_fp);
            fputc(B100_ESCAPE,_fp);

            b100(diffye,_fp);
#endif
        }
        xs = x0;
        ys = y0;
        xe = (x1-x0);
        ye = (y1-y0);


        if( special != NULL )
        {
#ifdef NOB100
            fputc(' ',_fp);
#else
            fputc(B100_ESCAPE,_fp);
#endif
            fputs(special,_fp);
        }
        fputc('\n',_fp);
    }
}

void dcrASA::append(char *x)
{
   
    if ( _ls )
    {
        appendChar("{");
    }

    appendChar(x);

    if ( _ls )
    {
        appendChar("}");
    }
    appendChar("\n");
}

void dcrASA::appendChar(char *x)
{

    switch (_dest)
    {
        case INTERP:
            // do interp stuff
            //fprintf(stderr,"Writing to interp: %s\n",x);
            Tcl_AppendResult(_interp,x,NULL);
            break;
        case CHANNEL:
            // do channel stuff
            fwrite(x,1,strlen(x),_fp);
            fflush(_fp);
            break;
        default:
			ads::error(0,"Use of dcrASA before initialized!\n");
            break;
    }

}

void dcrASA::setList(bool ls)
{
    _ls = ls;
}

void dcrASA::output()
{
    int rc;
    switch (_dest)
    {
        case INTERP:
            // do interp stuff
            //Tcl_AppendResult(_interp,"\n",NULL);
            //fprintf(stderr,"Output to interp:!\n");
            break;
        case CHANNEL:
            //fwrite(".\n",1,2,_fp);
            fflush(_fp);
            rc = fclose(_fp);
            if( rc != 0 )
            {
                fprintf(stderr,"Error closing channel: %d\n",errno);
            }
            break;
        default:
			ads::error(0,"Use of dcrASA before initialized!\n");
            break;
    }
}

//END_NAMESPACE_ADS
