import importlib.util
spec = importlib.util.spec_from_file_location("opendbpy", "../../build/src/swig/python/opendbpy.py")
odb = importlib.util.module_from_spec(spec)
spec.loader.exec_module(odb)
db = odb.dbDatabase.create()
lef_parser = odb.lefin(db, True)
tech_lib = lef_parser.createTechAndLib("../data/gscl45nm.lib", "../data/gscl45nm.lef")
libs = db.getLibs()
def_parser = odb.defin(db)
chip  = def_parser.createChip(libs,"../data/design.def")
if chip == None:
    exit("Read DEF Failed")
print(chip)
exit()
