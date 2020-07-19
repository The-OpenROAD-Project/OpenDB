import importlib.util
import opendbpy as odb
import os
import unittest

#TestNet: A unit test class for class dbNet
#it inherits from unittest.TestCase and has access to the testing functions(asserts)
class TestNet(unittest.TestCase):
    #This Function is called before each of the test cases defined below
    #You should use it to create the instances you need to test (in our case n1,n2,n3)
    def setUp(self):
        #Creating Database and Lib
        self.db = odb.dbDatabase.create()
        tech = odb.dbTech.create(self.db)
        lib = odb.dbLib.create(self.db,"lib")
        #Creating Chip and Block
        odb.dbChip.create(self.db)
        blockName = 'NetUnitTest'
        block = odb.dbBlock_create(self.db.getChip(),blockName)
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
        self.n1 = odb.dbNet.create(block,"n1")
        self.n2 = odb.dbNet.create(block,"n2")
        self.n3 = odb.dbNet.create(block,"n3")
        IN1 = odb.dbBTerm.create(self.n1,'IN1')
        IN1.setIoType('INPUT')
        IN2 = odb.dbBTerm.create(self.n2,'IN2')
        IN2.setIoType('INPUT')
        OUT = odb.dbBTerm.create(self.n3,'OUT')
        OUT.setIoType('OUTPUT')
        #connecting nets
        odb.dbITerm.connect(inst,self.n1,inst.getMaster().findMTerm('a'))
        odb.dbITerm.connect(inst,self.n2,inst.getMaster().findMTerm('b'))
        odb.dbITerm.connect(inst,self.n3,inst.getMaster().findMTerm('o'))
        pass
    #this function is called after each of the test cases
    #you should free up space and destroy unneeded objects(cleanup step)
    def tearDown(self):
        self.db.destroy(self.db)
        
    #each test case should start with the name "test"
    def test_naming(self):
        self.changeAndTest(self.n1,'rename','getName','_n1','_n1')
        self.check(self.n1,'getConstName','_n1')
        self.assertFalse(self.change(self.n1,'rename','n2'))
    def test_dbSetterAndGetter(self):
        self.changeAndTest(self.n1,'setRCDisconnected','isRCDisconnected',False,False)
        self.changeAndTest(self.n1,'setRCDisconnected','isRCDisconnected',True,True)
        self.changeAndTest(self.n1,'setWeight','getWeight',2,2)
        self.changeAndTest(self.n1,'setSourceType','getSourceType','NETLIST','NETLIST')
        self.changeAndTest(self.n1,'setXTalkClass','getXTalkClass',1,1)
        self.changeAndTest(self.n1,'setCcAdjustFactor','getCcAdjustFactor',1,1)
        self.changeAndTest(self.n1,'setSigType','getSigType','RESET','RESET')
    def test_dbCc(self):
        self.changeAndTest(self.n1,'setDbCc','getDbCc',2,2)
        self.changeAndTest(self.n1,'addDbCc','getDbCc',5,3)
    def test_cc(self):
        node2 = odb.dbCapNode_create(self.n2,0,False)
        node1 = odb.dbCapNode_create(self.n1,1,False)
        node1.setInternalFlag()
        ccseg = odb.dbCCSeg_create(node1,node2)
        self.n1.calibrateCouplingCap()
        self.check(self.n1,'maxInternalCapNum',1)
        self.check(self.n1,'groundCC',True,1)
        self.check(self.n2,'groundCC',False,1)
        self.check(self.n1,'getCcCount',1)   
        
        
    #Function to change a value and test the change effect
    def changeAndTest(self,obj,SetterName,GetterName,expectedVal,*args):
        getattr(obj, SetterName)(*args)
        self.assertEqual(getattr(obj, GetterName)(),expectedVal)
    def check(self,obj,GetterName,expectedVal,*args):
        self.assertEqual(getattr(obj, GetterName)(*args),expectedVal)
    def change(self,obj,SetterName,*args):
        return getattr(obj, SetterName)(*args)
if __name__=='__main__':
    unittest.main()