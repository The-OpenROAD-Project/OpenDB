import opendbpy as odb
import helper
import odbUnitTest

class TestDestroy(odbUnitTest.TestCase):
    def setUp(self):
        self.db = helper.create2LevelBlock()
        self.block = self.db.getChip().getBlock()
        self.i1 = self.block.findInst('i1')
        self.i2 = self.block.findInst('i2')
        self.i3 = self.block.findInst('i3')
        
        self.n1 = self.i1.findITerm('a').getNet()
        self.n2 = self.i1.findITerm('b').getNet()
        self.n3 = self.i2.findITerm('a').getNet()
        self.n4 = self.i2.findITerm('b').getNet()
        self.n5 = self.i3.findITerm('a').getNet()
        self.n6 = self.i3.findITerm('b').getNet()
        self.n7 = self.i3.findITerm('o').getNet()
    def tearDown(self):
        self.db.destroy(self.db)
    def test_destroy_n1(self):
        self.n1.destroy(self.n1)
        #check for Inst
        self.assertIsNone(self.i1.findITerm('a').getNet())
        #check for Iterms
        for iterm in self.block.getITerms():
            if(iterm.getNet() is None):
                self.assertEqual(iterm.getInst().getName(),'i1')
                self.assertEqual(iterm.getMTerm().getName(),'a')
            else:
                self.assertNotEqual(iterm.getNet().getName(),'n1')
        #check for block and BTerms
        nets = self.block.getNets()
        for net in nets:
            self.assertNotEqual(net.getName(),'n1')
        bterms = self.block.getBTerms()
        self.assertEqual(len(bterms),4)
        for bterm in bterms:
            self.assertNotEqual(bterm.getName(),'IN1')
            self.assertNotEqual(bterm.getNet().getName(),'n1')
        self.assertIsNone(self.block.findBTerm('IN1'))
        self.assertIsNone(self.block.findNet('n1'))
    def test_destroy_i1(self):
        self.i1.destroy(self.i1)
        #check for block
        self.assertIsNone(self.block.findInst('i1'))
        for inst in self.block.getInsts():
            self.assertNotEqual(inst.getName(),'i1')
        self.assertEqual(len(self.block.getITerms()),6)
        #check for Iterms
        for iterm in self.block.getITerms():
            self.assertNotIn(iterm.getNet().getName(),['n1','n2'])
            self.assertNotEqual(iterm.getInst().getName(),'i1')
        #check for BTERMS
        IN1 = self.block.findBTerm('IN1')
        self.assertIsNone(IN1.getITerm())
        IN2 = self.block.findBTerm('IN2')
        self.assertIsNone(IN2.getITerm())
        #check for nets
        self.assertEqual(self.n1.getITermCount(),0)
        self.assertEqual(self.n2.getITermCount(),0)
        self.assertEqual(self.n5.getITermCount(),1)
        self.assertNotEqual(self.n5.getITerms()[0].getInst().getName(),'i1')
if __name__=='__main__':
    odbUnitTest.mainParallel(TestDestroy)
#     odbUnitTest.main()