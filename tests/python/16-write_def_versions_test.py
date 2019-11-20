import importlib.util
import os
import re
spec = importlib.util.spec_from_file_location("opendbpy", "../../build/src/swig/python/opendbpy.py")
odb = importlib.util.module_from_spec(spec)
spec.loader.exec_module(odb)
db = odb.dbDatabase.create()
chip = odb.odb_read_design(db, ["../data/gscl45nm.lef"], ["../data/design.def"])
tech = db.getTech()
block = chip.getBlock()

versions = [odb.defout.DEF_5_3, odb.defout.DEF_5_4, odb.defout.DEF_5_5, odb.defout.DEF_5_6]
#result = odb.odb_write_def(block, 'test_out.def', DEF_5_6)
version_dict = {0: 'DEF_5_3', 1: 'DEF_5_4', 2:'DEF_5_5', 3:'DEF_5_6'}


for i,version in enumerate(versions):
    print(version)
    result = odb.odb_write_def(block, 'test_out.def', version)
    if result == None:
        exit("DEF not written")
    db= odb.dbDatabase.create()
    def_parser = odb.defin(db)
    lef_parser = odb.lefin(db, True)
    tech_lib = lef_parser.createTechAndLib("../data/gscl45nm.lib", "../data/gscl45nm.lef")
    libs = db.getLibs()
    chip = def_parser.createChip(libs,"test_out.def")
    if chip == None:
        exit("Failed to read written DEF")
    f = open("test_out.def", "r")
    os.system("cat test_out.def")
    written_def = f.read()

    if re.match(r'VERSION\s+(\d+)\.(\d+)\s+?;',written_def):
        data = re.findall(r'VERSION\s+(\d+)\.(\d+)\s+?;', written_def)
    major = data[0][0]
    minor = data[0][1]
    version_str = str(version)
    if major == None or  minor == None or "DEF_"+ major + "_" + minor!=version_dict[i]:
        exit("Invalid written DEF for version")

