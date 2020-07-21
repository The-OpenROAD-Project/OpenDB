import opendbpy as odb
import helper
import odbUnitTest

class TestBlock(odbUnitTest.TestCase):
    def setUp(self):
        self.db,self.lib = helper.createSimpleDB()
        self.parentBlock = odb.dbBlock_create(self.db.getChip(),'Parent')
        self.block = helper.create2LevelBlock(self.db,self.lib,self.parentBlock)
        self.block.setCornerCount(4)
        self.extcornerblock = self.block.createExtCornerBlock(1)
        odb.dbTechNonDefaultRule_create(self.block,'non_default_1')
        self.parentRegion = odb.dbRegion_create(self.block,'parentRegion')
        self.childRegion = odb.dbRegion_create(self.parentRegion,'childRegion')
    def tearDown(self):
        self.db.destroy(self.db)
    def test_find(self):
        #bterm
        self.assertEqual(self.block.findBTerm('IN1').getName(),'IN1')
        self.assertIsNone(self.block.findBTerm('in1'))
        #child
        self.assertEqual(self.parentBlock.findChild('2LevelBlock').getName(),'2LevelBlock')
        self.assertIsNone(self.parentBlock.findChild('1LevelBlock'))
        #inst
        self.assertEqual(self.block.findInst('i3').getName(),'i3')
        self.assertIsNone(self.parentBlock.findInst('i3'))
        #net
        self.assertEqual(self.block.findNet('n2').getName(),'n2')
        self.assertIsNone(self.block.findNet('a'))
        #iterm
        self.assertEqual(self.block.findITerm('i1,o').getInst().getName(),'i1')
        self.assertEqual(self.block.findITerm('i1,o').getMTerm().getName(),'o')
        self.assertIsNone(self.block.findITerm('i1\o'))
        #extcornerblock
        self.assertEqual(self.block.findExtCornerBlock(1).getName(),'extCornerBlock__1')
        self.assertIsNone(self.block.findExtCornerBlock(0))
        #nondefaultrule
        self.assertEqual(self.block.findNonDefaultRule('non_default_1').getName(),'non_default_1')
        self.assertIsNone(self.block.findNonDefaultRule('non_default_2'))
        #region
        self.assertEqual(self.block.findRegion('parentRegion').getName(),'parentRegion')
        self.assertEqual(self.block.findRegion('childRegion').getName(),'childRegion')
        self.assertEqual(self.block.findRegion('childRegion').getParent().getName(),'parentRegion')
    def test_find_extcornerblock(self):    
        pass
    def test_find_nondefaultrule(self):    
        pass
    def test_find_region(self):
        pass 
        
    
if __name__=='__main__':
#     odbUnitTest.mainParallel(TestBlock)
    odbUnitTest.main()
    