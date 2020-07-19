import opendbpy as odb
import os

#logical expr OUT = (IN1&IN2)
#     (n1)   +-----
# IN1--------|a    \    (n3)
#     (n2)   | (i1)o|-----------OUT
# IN2--------|b    /            
#            +-----             
def create1LevelBlock():
    db = odb.dbDatabase.create()
    tech = odb.dbTech.create(db)
    lib = odb.dbLib.create(db,"lib")
    #Creating Chip and Block
    odb.dbChip.create(db)
    blockName = '1LevelBlock'
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



#logical expr OUT = (IN1&IN2) | (IN3&IN4)
#     (n1)   +-----
# IN1--------|a    \    (n5)
#     (n2)   | (i1)o|-----------+
# IN2--------|b    /            |       +-------
#            +-----             +--------\a     \    (n7)
#                                         ) (i3)o|---------------OUT
#     (n3)   +-----             +--------/b     /
# IN3--------|a    \    (n6)    |       +-------
#     (n4)   | (i2)o|-----------+
# IN4--------|b    /
#            +-----
def create2LevelBlock():
    db = odb.dbDatabase.create()
    tech = odb.dbTech.create(db)
    lib = odb.dbLib.create(db,"lib")
    #Creating Chip and Block
    odb.dbChip.create(db)
    blockName = '2LevelBlock'
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
    #creating Master or2
    or2 = odb.dbMaster_create(lib,"or2")
    or2.setWidth(500)
    or2.setHeight(500)
    or2.setType('CORE')
    odb.dbMTerm.create(or2,'a','INPUT')
    odb.dbMTerm.create(or2,'b','INPUT')
    odb.dbMTerm.create(or2,'o','OUTPUT')
    or2.setFrozen()
    #creating instances
    i1 = odb.dbInst.create(block,and2,"i1")
    i2 = odb.dbInst.create(block,and2,"i2")
    i3 = odb.dbInst.create(block,or2,"i3")
    #creating nets and block terms
    n1 = odb.dbNet.create(block,"n1")
    n2 = odb.dbNet.create(block,"n2")
    n3 = odb.dbNet.create(block,"n3")
    n4 = odb.dbNet.create(block,"n4")
    n5 = odb.dbNet.create(block,"n5")
    n6 = odb.dbNet.create(block,"n6")
    n7 = odb.dbNet.create(block,"n7")
    
    IN1 = odb.dbBTerm.create(n1,'IN1')
    IN1.setIoType('INPUT')
    IN2 = odb.dbBTerm.create(n2,'IN2')
    IN2.setIoType('INPUT')
    IN3 = odb.dbBTerm.create(n3,'IN3')
    IN3.setIoType('INPUT')
    IN4 = odb.dbBTerm.create(n4,'IN4')
    IN4.setIoType('INPUT')
    OUT = odb.dbBTerm.create(n7,'OUT')
    OUT.setIoType('OUTPUT')
    #connecting nets
    odb.dbITerm.connect(i1,n1,i1.getMaster().findMTerm('a'))
    odb.dbITerm.connect(i1,n2,i1.getMaster().findMTerm('b'))
    odb.dbITerm.connect(i1,n5,i1.getMaster().findMTerm('o'))
    
    odb.dbITerm.connect(i2,n3,i2.getMaster().findMTerm('a'))
    odb.dbITerm.connect(i2,n4,i2.getMaster().findMTerm('b'))
    odb.dbITerm.connect(i2,n6,i2.getMaster().findMTerm('o'))
    
    odb.dbITerm.connect(i3,n5,i3.getMaster().findMTerm('a'))
    odb.dbITerm.connect(i3,n6,i3.getMaster().findMTerm('b'))
    odb.dbITerm.connect(i3,n7,i3.getMaster().findMTerm('o'))
    
    return db
