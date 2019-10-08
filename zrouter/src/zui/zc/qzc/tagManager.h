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

#ifndef _TAGMANAGER_H
#define _TAGMANAGER_H

#include <vector>
#include <stdio.h>
#include "debug.h"

using namespace std;

class counter
{
    public:
        int cnt;
        int id;
        char *val;
        int status;

        void *data;

        counter(int idval, char *tagval)
        {
            cnt = 0;
            id = idval;
            val = strdup(tagval);
            status = 0;
            data = NULL;
        }

        ~counter() 
        { 
            if (val) 
            {
#ifdef DEBUG_MEM
                fprintf(stderr,"Counter Destructor Deleting %s\n",val);
#endif
                free(val); 
            }

            if (data)
                fprintf(stderr,"Error - data not null on delete!\n");
        }
};

class tagManager
{
    private:
        vector<counter *> tags;

        counter* getId(char *tag)
        {
            counter *c = findId(tag);
            if( c != NULL )
            {
                return c;
            }

            int idx = tags.size();
#ifdef DEBUG_MEM
            fprintf(stderr,"Adding new tag: %s\n",tag);
#endif
            counter *cp = new counter(idx,tag);
            // default, if not turned off
            if( _autonew == 1 )
            {
#ifdef DEBUG_MEM
                fprintf(stderr,"Adding new Tag Manager!\n");
#endif
                cp->data = (void*)(new tagManager());
            }
            tags.push_back(cp);
            return tags[idx];
        }

        int getCount(char *tag)
        {
            for(unsigned int i=0; i<tags.size(); i++)
            {
                if ( strcasecmp(tags[i]->val, tag) == 0 )
                {
                    return tags[i]->cnt;
                }
                return -1;
            }
            return -1;
        }

        int _autonew;

    public:
        tagManager() { _autonew = 1; }

        ~tagManager()
        {
#ifdef DEBUG_MEM
            fprintf(stderr,"tagManager destructor!\n");
#endif
            while( tags.size() > 0 )
            {
#ifdef DEBUG_MEM
                fprintf(stderr,"Call to erase for tag %s!\n",tags[0]->val);
#endif
                erase( tags[0]->val);
            }

        }

        void setAuto(int x)
        {
            _autonew = x;
        }

        void dump(int level=0)
        {
            for(unsigned int i=0; i<tags.size(); i++)
            {
                if( level == 0)
                {
                    fprintf(stderr,"Parent ");
                    fprintf(stderr,"(%d): %s: status: %d, count: %d\n",tags[i]->id,tags[i]->val,tags[i]->status,tags[i]->cnt);
                }
                else
                {
                    for(int j=0;j<level;j++)
                        fprintf(stderr,"    Child ");
                        fprintf(stderr,"(%d): %s: status: %d, count: %d\n",tags[i]->id,tags[i]->val,tags[i]->status,tags[i]->cnt);
                }
                if( _autonew  )
                {
                    tagManager *tm = (tagManager*)tags[i]->data;
                    if( tm != NULL )
                        tm->dump(level+1);
                }
                
            }
        }
        void normalize()
        {
            for(unsigned int i=0; i<tags.size(); i++)
            {
                tags[i]->id = i;
            }
        }

        void erase(char *type)
        {
#ifdef DEBUG_MEM
            fprintf(stderr,"Erasing %s!\n",type);
#endif
            counter *c = findId(type);
            if( c != NULL )
            {
                if ( (c->data != NULL) && _autonew != 0)
                {
                    tagManager *tm = (tagManager*)(c->data);
                    delete tm;
                    c->data = NULL;
                }
                tags.erase(tags.begin()+c->id);
                normalize();
                delete c;
            }
        }


        void erase(char *type, char *tag)
        {
            counter *c = findId(type);
            if( c == NULL )
            {
                return;
            }
            tagManager *tm = (tagManager*)(c->data);
            if( tm != NULL )
            {
                counter *c = tm->findId(tag);
                if( c != NULL )
                {
                    tm->tags.erase(tm->tags.begin()+c->id);
                    tm->normalize();
                    c->data = NULL;
                    delete c;
                }
            }
        }

        void erase(char *type, int idx)
        {
            counter *c = findId(type);
            if( c == NULL )
            {
                return;
            }
            tagManager *tm = (tagManager*)(c->data);
            if( (tm != NULL) && (tm->tags.size() > (unsigned int)idx) )
            {
                c = tm->tags[idx];
                tm->tags.erase(tm->tags.begin()+idx);
                tm->normalize();
                c->data = NULL;
                delete c;
            }
        }

        counter *findId(char *tag)
        {
            for(unsigned int i=0; i<tags.size(); i++)
            {
                if ( strcasecmp(tags[i]->val, tag) == 0 )
                {
                    return tags[i];
                }
            }

            return NULL;
        }

        int findIdIdx(char *tag)
        {
            for(unsigned int i=0; i<tags.size(); i++)
            {
                if ( strcasecmp(tags[i]->val, tag) == 0 )
                {
                    return i;
                }
            }
            return -1;
        }

        void incTag(char *type, int idx) 
        {  
            counter *c = findId(type);
            if( c == NULL )
            {
                return;
            }
            tagManager *tm = (tagManager*)(c->data);
            if( (tm != NULL) && (tm->tags.size() > (unsigned int)idx) )
            {
                tm->tags[idx]->cnt++;
            }
        }

        int getTagCount(char *type)
        {
            return getCount(type);
        }

        int getTypeCount()
        {
            return tags.size();
        }

        char *getType(int idx)
        {
            if( tags.size() > (unsigned int)idx )
                return tags[idx]->val;

            return NULL;
        }

        int getTagCount(char *type, int idx)
        {
            counter *c = findId(type);
            if( c == NULL )
            {
                return -1;
            }
            tagManager *tm = (tagManager*)(c->data);
            if( (tm != NULL) && (tm->tags.size() > (unsigned int)idx) )
            {
                return tm->tags[idx]->cnt;
            }
            return -1;
        }

        int getTagCount(char *type, char *tag)
        {
            counter *c = findId(type);
            if( c == NULL )
            {
                return -1;
            }
            tagManager *tm = (tagManager*)(c->data);
            if( tm != NULL )
            {
                return tm->getCount(tag);
            }
            return -1;
        }

        int getTagSize(char *type)
        {
            counter *c = findId(type);
            if( c == NULL )
            {
                return -1;
            }
            tagManager *tm = (tagManager*)(c->data);
            if( tm != NULL )
            {
                return tm->tags.size();
            }
            return -1;
        }

        char *getTag(char *type, int idx)
        {
            counter *c = findId(type);
            if( c == NULL )
            {
                return NULL;
            }
            tagManager *tm = (tagManager*)(c->data);
            if( (tm != NULL) && (tm->tags.size() > (unsigned int)idx) )
            {
                return tm->tags[idx]->val;
            }
            return NULL;
        }

        counter *findTagId(char *type)
        {
            return findId(type);
        }

        counter *findTagId(char *type, char *tag)
        {
            counter *c = findId(type);
            if( c == NULL )
            {
                return NULL;
            }
            tagManager *tm = (tagManager*)(c->data);
            if( tm != NULL )
            {
                return tm->findId(tag);
            }
            return NULL;
        }

        counter *getTagId(int idx)
        {
            return tags[idx];
        }

        counter *getTagId(char *type)
        {
            return getId(type);
        }

        counter *getTagId(int tidx, char *tag)
        {
            if( tidx < 0 ) return NULL;

            counter *c = tags[tidx];
            if( c == NULL )
            {
                return NULL;
            }
            tagManager *tm = (tagManager*)(c->data);
            if( tm != NULL )
            {
                //fprintf(stderr,"Get sub-Id: %s\n",tag);
                return tm->getId(tag);
            }
            return NULL;
        }
        counter *getTagId(char *type, char *tag)
        {
            counter *c = getId(type);
            if( c == NULL )
            {
                return NULL;
            }
            tagManager *tm = (tagManager*)(c->data);
            if( tm != NULL )
            {
                //fprintf(stderr,"Get sub-Id: %s\n",tag);
                return tm->getId(tag);
            }
            return NULL;
        }

        void setTypeStatus(char *type, int status)
        {
            counter *c = findId(type);
            if( c != NULL)
            {
                c->status = status;
            }
        }

        void setTagStatus(char *type, int idx, int status)
        {
            counter *c = findId(type);
            if( c == NULL )
            {
                return;
            }
            tagManager *tm = (tagManager*)(c->data);
            if( (tm != NULL) && (tm->tags.size() > (unsigned int)idx) )
            {
                tm->tags[idx]->status = status;
            }
        }

        void setTagStatus(char *type, char *tag, int status)
        {
            counter *c = findId(type);
            if( c == NULL )
            {
                return;
            }
            tagManager *tm = (tagManager*)(c->data);
            if( tm != NULL )
            {
                c = tm->findId(tag);
                if( c != NULL )
                {
                    c->status = status;
                }
            }
        }

        int getTagStatus(char *type, int idx)
        {
            counter *c = findId(type);
            if( c == NULL )
            {
                return -1;
            }
            tagManager *tm = (tagManager*)(c->data);
            if( (tm != NULL) && (tm->tags.size() > (unsigned int)idx) )
            {
                return tm->tags[idx]->status;
            }
            return -1;

        }
};

template <class T>
class tmEraser
{
    public:
        void eraseTM(tagManager&tm)
        {
            T* t;
            counter *c;
            char *val;
            while( tm.getTypeCount() > 0 )
            {
                val = tm.getType(0);
                c = tm.getTagId(val);
                t = (T *)c->data;
                if( t != NULL )
                {
                    delete t;
                    c->data = NULL;
                }

                tm.erase(val);
            }
        }
};

#endif
