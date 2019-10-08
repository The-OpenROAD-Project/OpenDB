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

#include "../include/dbTransform.h"
#include "../include/geom.h"

using namespace ads;

int main()
{
    dbTransform t1(dbOrientType::R90, adsPoint(10,10));
    dbTransform t2(dbOrientType::MX, adsPoint(100,100));
    dbTransform t3(dbOrientType::R270, adsPoint(50,50));
    dbTransform x(t1);
    t2.concat(x);
    x = t2;
    dbTransform t = x;
    t3.concat(x);
    x = t3;

    printf("%d %d %s\n", x.getOffset().x(), x.getOffset().y(), x.getOrient().getString() );

    t.invert();
    adsPoint p = x.getOffset();
    p.x() += 1;
    x.setOffset( p);
    x.concat(t);
    printf("%d %d %s\n", x.getOffset().x(), x.getOffset().y(), x.getOrient().getString() );
    t.invert();
    x.concat(t);
    printf("%d %d %s\n", x.getOffset().x(), x.getOffset().y(), x.getOrient().getString() );
    t.invert();
    x.concat(t);
    printf("%d %d %s\n", x.getOffset().x(), x.getOffset().y(), x.getOrient().getString() );
    
    
    x = dbTransform();
    adsPoint p0(50,50);
    adsPoint p1(50,50);
    x.apply(p);
    printf("%d %d -> %d %d\n", p0.x(), p0.y(), p1.x(), p1.y() );

    x.invert();
    x.apply(p);
    printf("%d %d -> %d %d\n", p0.x(), p0.y(), p1.x(), p1.y() );
}
