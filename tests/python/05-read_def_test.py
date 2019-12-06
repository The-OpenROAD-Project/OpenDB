import opendbpy as odb
import os 

current_dir = os.path.dirname(os.path.realpath(__file__))
tests_dir = os.path.abspath(os.path.join(current_dir, os.pardir))
opendb_dir = os.path.abspath(os.path.join(tests_dir, os.pardir))
data_dir = os.path.join(tests_dir, "data")

db = odb.dbDatabase.create()
lef_parser = odb.lefin(db, True)
tech_lib = lef_parser.createTechAndLib(os.path.join(data_dir, "gscl45nm.lib"), os.path.join(data_dir, "gscl45nm.lef"))
libs = db.getLibs()
def_parser = odb.defin(db)
chip  = def_parser.createChip(libs, os.path.join(data_dir, "design.def"))
if chip == None:
    exit("Read DEF Failed")
print(chip)
exit()
