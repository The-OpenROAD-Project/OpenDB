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

//Image - holds everything related to an image
#ifndef _IMAGESET_H
#define _IMAGESET_H

#include "debug.h"
#include "globals.h"
#include "renderbox.h"
#include "qbuffer.h"
#include "qcstring.h"
#include "qpixmap.h"
#include "qimage.h"
#include "tcl.h"

class Image
{
    private:
        char *_name;

        void init(char *name)
        {
            _name = strdup(name);
            //fprintf(stderr,"New Image: %s - %x!\n",name,(int)this);

            _pixmap = NULL;

            _iw = 0;
            _ih = 0;
            _haveimage = false;
        }

        RenderBox _bbox;
        QPixmap *_pixmap;
        QImage _image_main;
        QImage _image_scale;

        int _iw;
        int _ih;
        bool _haveimage;

    public:

        Image(char *name) 
        {
            if( name != NULL )
            {
                init(name);
            }
            else
            {
                init("none");
            }
        }
        Image()
        {
            init("none");
        }


        ~Image()
        {
#ifdef DEBUG_MEM
            fprintf(stderr,"Image Destructor!\n");
#endif
            free(_name);
#ifdef DEBUG_MEM
            fprintf(stderr,"ISD: free_lists();\n");
#endif
            if( _pixmap != NULL )
            {
                delete _pixmap;
            }
#ifdef DEBUG_MEM
            fprintf(stderr,"ISD: done!\n");
#endif
        };

        char *getName() { return _name; }

        void readChannel(char *channel, Tcl_Interp *interp)
        {
            if( _pixmap != NULL )
                delete _pixmap;

            _pixmap = new QPixmap();

            int mode;
            int len;
            Tcl_Channel ch = Tcl_GetChannel(interp, channel, &mode);

            QByteArray arr;
            QBuffer qb;
            qb.setBuffer(arr);
#ifdef IMAGE_DEBUG
            qb.open(IO_ReadWrite);
#else
            qb.open(IO_WriteOnly);
#endif

            char buff[1024];
            
            len = Tcl_Read(ch, buff,1023);
            while(len > 0)
            {
                qb.writeBlock(buff,len);
                len = Tcl_Read(ch, buff,1023);
            }

#ifdef IMAGE_DEBUG
            qb.at(0);
            qDebug("Read Channel - Image type: %s\n",QImageIO::imageFormat(&qb));
#endif
            qb.close();
            if( _pixmap->loadFromData(arr) )
            {
                _image_main = _pixmap->convertToImage();
                if( _image_main == NULL )
                {
                    fprintf(stderr,"Null Image in conversion!!\n");
                    delete _pixmap;
                    _pixmap = NULL;
                    return;
                }
                _haveimage = true;
            }
            else
            {
                fprintf(stderr,"Unable to load Image %s in readChannel!\n",_name);
            }

        }

        void loadData(unsigned char *data, int len, char *format)
        {
            if( _pixmap != NULL )
                delete _pixmap;

#ifdef IMAGE_DEBUG
            fprintf(stderr,"Image::loadData: %p, %d\n",data,len);
            QByteArray arr;
            QBuffer qb;
            arr.setRawData((const char*)data,len);
            qb.setBuffer(arr);
            qb.open(IO_ReadOnly);
            qDebug("Load Data - Image type: %s\n",QImageIO::imageFormat(&qb));
            qb.close();
            arr.resetRawData((const char*)data,len);
#endif

            _pixmap = new QPixmap();
            if( _pixmap->loadFromData(data,len, format) )
            {
                _image_main = _pixmap->convertToImage();
                if( _image_main == NULL )
                {
                    fprintf(stderr,"Null Image in loadData conversion!!\n");
                    delete _pixmap;
                    _pixmap = NULL;
                    return;
                }
                _haveimage = true;
            }
            else
            {
                fprintf(stderr,"Unable to load Image %s in loadData!\n",_name);
                FILE *fp = fopen("test.png","wb");
                fwrite(data,1,len,fp);
                fclose(fp);
            }
        }

        void loadFile(char *filename)
        {
            if( _pixmap != NULL )
                delete _pixmap;

            QString fname(filename);
#ifdef IMAGE_DEBUG
            qDebug("Load File - Image type: %s\n",QImageIO::imageFormat(fname));
#endif

            _pixmap = new QPixmap();
            if( _pixmap->load(fname))
            {
                _image_main = _pixmap->convertToImage();
                _haveimage = true;
            }
            else
            {
                fprintf(stderr,"Could not load Image file %s!\n",filename);
                delete _pixmap;
                _pixmap = NULL;
            }
        }

        void scale(int w, int h)
        {
            if( (_haveimage == true) )
            {
                if( (w != _iw) || (h != _ih))
                {
#ifdef IMAGE_DEBUG
                    fprintf(stderr,"image %s (%d,%d), scale before: %d, %d!\n",_name, w,h,_image_main.width(), _image_main.height());
#endif
                    _iw = w;
                    _ih = h;

                    _image_scale = _image_main.scale(w,h,QImage::ScaleFree);
                    _pixmap->convertFromImage(_image_scale);
#ifdef IMAGE_DEBUG
                    fprintf(stderr,"image scale %s (%d,%d), scale after: %d, %d - %d,%d!\n",_name, w,h,_image_scale.width(), _image_scale.height(), _pixmap->width(), _pixmap->height());
#endif
                }
#ifdef IMAGE_DEBUG
                else
                {
                    fprintf(stderr,"image %s, no scale!\n",_name);
                }
#endif
            }
        }

        void setPixmap(QPixmap *pm)
        {
            _pixmap = pm;
            _image_main = _pixmap->convertToImage().copy();
            _haveimage = true;
        }

        QPixmap *getPixmap() { return _pixmap; }
        QImage *getImage() { return &_image_main; }

        RenderBox *getBBox() { return &_bbox; }

        void getBBox(char *buff)
        {
            _bbox.getBBox(buff);
        }

};

#endif
