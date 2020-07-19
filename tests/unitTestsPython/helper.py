import opendbpy as odb
import os

def create1LevelBlock():
    db = odb.dbDatabase.create()
    tech = odb.dbTech.create(db)
    lib = odb.dbLib.create(db,"lib")
    #Creating Chip and Block
    odb.dbChip.create(db)
    blockName = 'NetUnitTest'
    block = odb.dbBlock_create(db.getChip(),blockName)
    #Creating Master and2 and instance inst
    and2 = odb.dbMaster_create(lib,"and2")
    and2.setWidth(1000)
    and2.setHeight(1000)
    and2.setType('CORE')
    odb.dbMTerm.create(and2,'a','INPUT')
    odb.dbMTerm.create(and2,'b','INPUT')
    odb.dbMTerm.create(and2,'o','OUTPUT')
    and2.setFrozen()
    inst = odb.dbInst.create(block,and2,"inst")
    #creating our nets
    n1 = odb.dbNet.create(block,"n1")
    n2 = odb.dbNet.create(block,"n2")
    n3 = odb.dbNet.create(block,"n3")
    IN1 = odb.dbBTerm.create(n1,'IN1')
    IN1.setIoType('INPUT')
    IN2 = odb.dbBTerm.create(n2,'IN2')
    IN2.setIoType('INPUT')
    OUT = odb.dbBTerm.create(n3,'OUT')
    OUT.setIoType('OUTPUT')
    #connecting nets
    odb.dbITerm.connect(inst,n1,inst.getMaster().findMTerm('a'))
    odb.dbITerm.connect(inst,n2,inst.getMaster().findMTerm('b'))
    odb.dbITerm.connect(inst,n3,inst.getMaster().findMTerm('o'))
    return db
