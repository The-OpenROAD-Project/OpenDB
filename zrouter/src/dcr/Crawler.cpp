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

#include <errno.h>
#include <sstream>
#include "db.h"
#include "Crawler.h"
#include "Crawler_Tcl.h"
#include "IZdcr.h"
#include "ZObject.h"
#include "logger.h"

#define TCL_IN_ARGS(name) \
					name * in_args = (name *) in
#define TCL_OUT_ARGS(name) \
					name * out_args = (name *) out

#define TCL_CONSTRUCTOR(name) \
					name::name( ZArgs * in, ZArgs * out ) : ZTechModule<name>( in, out )

BEGIN_NAMESPACE_ADS
int getLayers_1(dbTech * tech, char *buff) // TODO turn char* to dbString
{
	dbString layerBuff = "       0 SUB 1\n";

    uint cnt = 0;
    dbSet<dbTechLayer> layers = tech->getLayers();
    dbSet<dbTechLayer>::iterator itr;
    
    for ( itr = layers.begin(); itr != layers.end(); ++itr )
    {
        dbTechLayer * layer = *itr;
        dbTechLayerType type = layer->getType();
        
        if ( type.getValue() == dbTechLayerType::ROUTING )
        {
            dbString lname = layer->getName();
            char buffer[2048];
            
            sprintf(buffer, "       %d %s 1\n",
                    	layer->getRoutingLevel(), lname.c_str() );

            layerBuff += buffer;
			cnt ++;
        }
    }

    strcpy(buff, layerBuff.c_str());

    //sprintf(layerBuff, "%s %d %s ", layerBuff, ++cnt, "block");
    return cnt;
}

TCL_CONSTRUCTOR(Dcr)
{
	// _guiCnt= 1;
}
Dcr::~Dcr()
{
}
TCL_METHOD ( Dcr::get )
{
    //TCL_IN_ARGS ( ZIn_Dcr_init );
    TCL_OUT_ARGS ( ZOut_Dcr_init );

	out_args->dcr(_dcr);
		
    return TCL_OK;
}
TCL_METHOD ( Dcr::init )
{
    TCL_IN_ARGS ( ZIn_Dcr_init );
    TCL_OUT_ARGS ( ZOut_Dcr_init );
	
	if (adsNewComponent( _context, ZCID(Zdcr), _dcr )!= Z_OK)
	{
		assert(0);
	}	ZALLOCATED(_dcr);
	
	const char *name= (char*) in_args->name();
	const char *module= (char*) in_args->module();
	bool add_env= in_args->add_env();	
	bool store= in_args->store_boxes();
	_dcr->init(_context, add_env, name, module, !store);
	
    _zptPtr = (dpt *) _dcr->getDpt();


	out_args->dcr(_dcr);
	
    fprintf(stdout, "Crawler:Initiated Crawler <%s> for Technology Module ...\n",
		_dcr->getName());
	
    return TCL_OK;
}
TCL_METHOD ( Dcr::attach )
{
/*
    TCL_IN_ARGS ( ZIn_Dcr_attach );

	if (_guiCnt>8)
	{
		fprintf(stdout, "Cannot attach more than 8 applicatioons on the gui\n");
		return TCL_OK;
	}
	_gui[_guiCnt++]= in_args->gui();
*/

	return TCL_OK;
}
TCL_METHOD ( Dcr::add_menu )
{
    TCL_IN_ARGS ( ZIn_Dcr_add_menu );

	const char *name= (char*) in_args->name();
	//_dcr->setupModuleMenu(name, "z");

	fprintf(stdout, "menu= %s\n", name);

	return 0;
}
TCL_METHOD ( Dcr::add_sub_menu )
{
    TCL_IN_ARGS ( ZIn_Dcr_add_sub_menu );
	const char *name= (char*) in_args->name();
//	_dcr->addSubMenu(name);

	fprintf(stdout, "sub_menu= %s\n", name);

	return 0;
}
TCL_METHOD ( Dcr::inspect )
{
    TCL_IN_ARGS ( ZIn_Dcr_inspect );

	const char *chip=		in_args->chip();
	const char *objectName=	in_args->obj();
	const char *action=		in_args->info();
	const char *selectType=	in_args->s();
	const char *bb=			in_args->bbox();
	const char *layer=		in_args->layer();
	const char *type=		in_args->type();
	const char *hier=		in_args->hierarchy();
	bool noclip=			in_args->noclip();
	FILE *outFP=			in_args->channel();

	return _dcr->inspect(objectName, selectType, action, (char *)chip, bb, layer, hier, type, outFP, noclip, true);
}
TCL_METHOD ( Dcr::chip_get )
{
    TCL_IN_ARGS ( ZIn_Dcr_chip_get );

	const char *bb=	in_args->bbox();
	const char *layer=	in_args->layer();
	const char *type=	in_args->type();
	const char *hierarchy=	in_args->hierarchy();
	//double gx=	in_args->gx();
	//double gy=	in_args->gy();
	int maxobjects=	in_args->maxobjects();
	bool gridFlag=	in_args->grid();
	FILE *outFP= in_args->channel();
	const char *chip= in_args->chip();
	bool noclip= in_args->noclip();
	bool cursor= in_args->q();
	
	//uint blockId= 1;
	_dcr->resetSearchParams((char *)chip, bb, layer, hierarchy, type, noclip, cursor);
	
	_dcr->chip_get(chip, gridFlag, maxobjects, outFP);

	return TCL_OK;
}
TCL_METHOD ( Dcr::chip_attr_get )
{
    TCL_IN_ARGS ( ZIn_Dcr_chip_attr_get );

	char layers[1024]; strcpy(layers, "");

	uint n= 0;
	dbTech *tech= _db->getTech();
	if (tech!=NULL)
	{
		n= getLayers_1(tech, layers);
	}
	_dcr->chipAttr(layers, n, in_args->def_types());

	return TCL_OK;
}
TCL_METHOD ( Dcr::tiles_get )
{
    //TCL_IN_ARGS ( ZIn_Dcr_tiles_get );

	return 0;
}

TCL_METHOD ( Dcr::config )
{
    fprintf(stderr,"In Dcr Config: 1\n");
    TCL_IN_ARGS ( ZIn_Dcr_config );
    const char *size = in_args->grid();
    const char *bbox = in_args->bbox();
    const char *colors = in_args->colors();
    int slices = in_args->slices();
    int rc=0;

    if ( (size != NULL) )
    {
        int size1, size2;
        sscanf(size,"%d %d",&size1,&size2);
        //fprintf(stderr,"zpt size with: %d,%d\n",size1,size2);
        rc += _zptPtr->setSize(size1,size2);
    }

    if( (bbox != NULL) )
    {
        float xm, ym, xx, yx;
        int xmin, ymin, xmax, ymax;
        sscanf(bbox,"%f %f %f %f",&xm,&ym,&xx,&yx);
        xmin = (int)xm; xmax = (int)xx;
        ymin = (int)ym; ymax = (int)yx;
        //fprintf(stderr,"zpt bbox with: %d,%d %d,%d (%s)\n",xmin,ymin,xmax,ymax,bbox);
        rc += _zptPtr->scaling(xmin,ymin,xmax,ymax);
    }

    if( slices > 0 )
    {
        //fprintf(stderr,"zpt slices with: %d\n",slices);
        rc += _zptPtr->setSlices(slices);
    }

    if ( colors != NULL )
    {
        char *clbuff = strdup(colors);
        char *rgbp;
        char *slp = strtok(clbuff," \n\t");
        char *ptr;
        int slice, rgb;
        while( slp != NULL )
        {
            slice = strtol(slp,&ptr,10);
            if( ptr == slp)
            {
                warning(0,"No slice value: %s\n",slp);
                return TCL_ERROR;
            }

            rgbp = strtok(NULL," \n\t");
            if( rgbp == NULL )
            {
                warning(0,"No color value for slice %d\n",slice);
                return TCL_ERROR;
            }
            rgb = strtol(rgbp,&ptr,16);
            if( ptr == rgbp)
            {
                warning(0,"No color value extracted from: %s\n",rgbp);
                return TCL_ERROR;
            }
            
            //notice(0,"Colorize: %d, %06x\n",slice,rgb);
            Logger::getLogger()->notice(0,"Colorize: %d, %06x\n",slice,rgb);
            rc += _zptPtr->colorize(slice,rgb);
            slp = strtok(NULL," \n\t");
        }

        free(clbuff);
    }

    if( rc != 0 )
    {
        warning(rc,"Error in Dcr config");
        return TCL_ERROR;
    }
    else
    {
        return TCL_OK;
    }
}
TCL_METHOD ( Dcr::colors )
{
    TCL_OUT_ARGS (ZOut_Dcr_colors);
    int max = _zptPtr->get_max_slice();
    //fprintf(stderr,"Getting color info for %d slices\n",max+1);
    if( max >= 0 )
    {
        char *buff = (char*)malloc(max*32+32); // leave some extra space
        buff[0]='\0';
        char conv[128];
        char *rgbc;
        long rgb;

        for(int i=0;i<=max;i++)
        {
            rgbc = _zptPtr->get_color_value(i);
            rgb = (*rgbc++ & 0xff) << 16;
            rgb += (*rgbc++ & 0xff) << 8;
            rgb += (*rgbc & 0xff);

            sprintf(conv,"%d %06x ",i,(unsigned int)rgb);
            strcat(buff,conv);
        }
        out_args->colors(buff);
        free(buff);
        return TCL_OK;
    } else {
        out_args->colors("");
        return TCL_OK;
    }

}
TCL_METHOD ( Dcr::clear )
{
    _zptPtr->alloc_mem();
	return TCL_OK;
}

TCL_METHOD (Dcr::bc_get )
{
    //TCL_IN_ARGS ( ZIn_Dcr_bc_get );
    TCL_OUT_ARGS (ZOut_Dcr_bc_get);
    int a= _zptPtr->get_boxcount();
    notice(0,"Crawler bc: %d\n",a);
    out_args->bc(a);
	return TCL_OK;
}

TCL_METHOD( Dcr::test_milos )
{
    TCL_IN_ARGS( ZIn_Dcr_test_milos);
    const char *cmd = in_args->cmd();
    notice(0,"In test milos: %s\n",cmd);
    milos(0,cmd);
	return TCL_OK;
}

TCL_METHOD( Dcr::test_ade )
{
    TCL_IN_ARGS( ZIn_Dcr_test_ade);
    const char *cmd = in_args->cmd();
    notice(0,"In test ade: %s\n",cmd);
    ade(0,cmd);
	return TCL_OK;
}

TCL_METHOD (Dcr::paint )
{
	return TCL_OK;
}
#ifdef _WIN32
#define strncasecmp strncmp
#define strcasecmp strcmp
#endif

TCL_METHOD (Dcr::image )
{
    TCL_IN_ARGS ( ZIn_Dcr_image );
    TCL_OUT_ARGS (ZOut_Dcr_image);
    const char *encoding = in_args->encoding();
    const char *format = in_args->format();
#ifdef USEJPEG
    int quality = in_args->quality();
    int speed = in_args->speed();
#endif
    FILE *channel = in_args->channel();
    int enctype=255;
    int ftype=DPT_PPM;

    //notice(0,"In Dcr::image\n");

    if( encoding != NULL )
    {
        if( strncasecmp(encoding,"base",4) == 0 )
        {
            enctype = atoi(encoding+4);
        }
    }

    if( format != NULL )
    {
        if( strcasecmp(format,"ppm") == 0 )
        {
            ftype = DPT_PPM;
        }
#ifdef USEJPEG
        else if( strcasecmp(format,"jpeg") == 0 )
        {
            _zptPtr->setJpegAttributes(quality,speed);
            ftype = DPT_JPEG;
        }
#endif
#ifdef USEPNG
        else if( strcasecmp(format,"png") == 0 )
        {
            ftype = DPT_PNG;
        }
#endif
        else
        {
            warning(0,"Image format %s not supported!\n",format);
            ftype = DPT_PPM;
        }
    }

    if( channel != NULL )
    {
            //notice(0,"Calling create_image FILE with: %x, %d, %d\n",channel,enctype,ftype);
        _zptPtr->create_image(channel,NULL,DPT_FILE,enctype,ftype);
        return TCL_OK;
    } 
    else 
    {
        char *s;
        if( enctype != 64 )
        {
            char *myhi = strdup("hi");
            notice(0,"%% test: %s\n",myhi);
            warning(1,"{Variables must be returned base64 encoded %% %% !\n}");
#if 0
            char *myh2 = strdup("h2");
            notice(0,"%% test2: %s\n",myh2);
            notice(2,"Another test: %% %s\n",myhi);
            notice(0,"%% %% %s test3: %s\n","abc",myh2);
            Logger::getLogger()->notice(2,"Critical test: %% %d %s\n",0,"abc");
            debug("TMG","p","Test debug: %s\n","abc");


            //log debugging stuff
            int i;
            for(i=0; i<100; i++)
            {
                verbose('a',"Info message %d\n",i);
            }
            verbose(0,"Sent %d info messages!\n",i);
            //resetWarningCount("Info message %d\n",20,0);
            for(i=0; i<100; i++)
            {
                info('a',"Info message %d\n",i);
            }
            info(0,"Sent %d more info messages!\n",i);
            for(i=0; i<100; i++)
            {
                debug("DCR","A","Debug message %d\n",i);
            }
            debug("DCR","B","Sent %d debug messages!\n",i);
            error(2,"Var-base64\n");
#endif
            return TCL_ERROR;
        }
        else
        {
            //notice(0,"Calling create_image STRING with: %d, %d\n",enctype,ftype);
            int rc = _zptPtr->create_image(NULL,&s,DPT_STRING,enctype,ftype);
            if( rc == 0)
                out_args->ivar(s);
            else
                out_args->ivar(NULL);

            if( s != NULL )
                free(s);
        }
    }

	return TCL_OK;
}

END_NAMESPACE_ADS
