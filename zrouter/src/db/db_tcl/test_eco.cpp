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

#include "db.h"

BEGIN_NAMESPACE_ADS

void test_eco()
{
    dbDatabase * db = dbDatabase::create();
    //dbTech * tech = dbTech::create(db);
    dbLib * lib = dbLib::create( db, "lib" );
    dbMaster * master0 = dbMaster::create(lib, "master0" );
    dbMTerm * mtermA = dbMTerm::create(master0, "A");
    dbMTerm * mtermB = dbMTerm::create(master0, "B");
    dbMaster * master1 = dbMaster::create(lib, "master1" );
    dbMTerm::create(master1, "A");
    dbMTerm::create(master1, "B");
    master0->setFrozen();
    master1->setFrozen();
    dbChip * chip = dbChip::create( db );
    dbBlock * block = dbBlock::create( chip, "block" );
    block->setCornerCount(1);

    FILE * file = fopen("eco.db", "w");
    db->write( file );
    fclose(file);
    
    dbDatabase::beginEco(block);

    dbInst * inst0 = dbInst::create( block, master0, "inst0");
    dbInst * inst1 = dbInst::create( block, master0, "inst1");
    inst0->setOrigin(0,0);
    inst0->setOrient(dbOrientType(dbOrientType::R0));
    inst0->setPlacementStatus(dbPlacementStatus(dbPlacementStatus::PLACED));
    inst0->setUserFlag1();
    inst0->setUserFlag2();
    inst0->setUserFlag3();
    inst1->swapMaster(master1);

    dbNet * net0 = dbNet::create(block, "net0");
    dbNet * net1 = dbNet::create(block, "net1");
    dbNet * net2 = dbNet::create(block, "net2");

    net0->setWireType( dbWireType() );
    net0->setSigType( dbSigType() );
    net0->setSpef( true);
    net0->setSelect( true);
    net0->setMark( true);
    net0->setWireOrdered( true);
    net0->setBuffered( true);
    net0->setDisconnected( true);
    net0->setWireAltered( true);
    net0->setExtracted( true);
    net0->setRCgraph( true);
    net0->setReduced( true);
    net0->setIOflag();
    net0->setSpecial();
    net0->setWildConnected();
    
    dbITerm * iterm0 = dbITerm::connect( inst0, net0, mtermA );
    dbITerm * iterm1 = dbITerm::connect( inst0, net1, mtermB );
    dbITerm * iterm2 = dbITerm::connect( inst1, net1, mtermA );
    dbITerm * iterm3 = dbITerm::connect( inst1, net2, mtermB );
    
    iterm0->setSpecial();
    iterm0->setSpef(1);
    iterm0->setConnected();

    dbRSeg * r0 = dbRSeg::create( net0, 0, 0, 0, 0 );
    //dbRSeg * r1 = dbRSeg::create( net1, 0, 0, 0, 0 );
    dbCapNode * n0 = dbCapNode::create( net0, 0, true );
    dbCapNode * n1 = dbCapNode::create( net1, 1, true );
    dbCCSeg * cc0 = dbCCSeg::create( n0, n1 );

    r0->setTargetNode( n0->getId() );
    r0->setSourceNode( n1->getId() );
    r0->setResistance(1.0, 0);
    
    n0->setCapacitance(1.0, 0);
    n0->addCapacitance(2.0, 0);
    n0->adjustCapacitance(2.0);
    cc0->setCapacitance(1.0, 0);
    cc0->addCapacitance(1.0, 0);

    #if 1
    dbITerm::disconnect(iterm0);
    dbITerm::disconnect(iterm1);
    dbITerm::disconnect(iterm2);
    dbITerm::disconnect(iterm3);
    dbNet::destroy(net0);
    dbNet::destroy(net1);
    dbInst::destroy(inst0);
    dbInst::destroy(inst1);
    #endif

    dbDatabase::endEco(block);
    file = fopen("eco.db1", "w");
    db->write( file );
    fclose(file);

    file = fopen("db.eco", "w");
    dbDatabase::writeEco(block,file);
    fclose(file);

    file = fopen("eco.db", "r");
    db->read(file);
    fclose(file);
    
    file = fopen("db.eco", "r");
    dbDatabase::readEco(block,file);
    dbDatabase::commitEco(block);
    fclose(file);
    file = fopen("eco.db2", "w");
    db->write( file );
    fclose(file);

    file = fopen("eco.db1", "r");
    db->read(file);
    fclose(file);
}

END_NAMESPACE_ADS
