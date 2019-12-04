import importlib.util
spec = importlib.util.spec_from_file_location("opendbpy", "./OpenDB/build/src/swig/python/opendbpy.py")
odb = importlib.util.module_from_spec(spec)
spec.loader.exec_module(odb)
db = odb.dbDatabase.create()
lef_parser = odb.lefin(db, True)
tech_lib = lef_parser.createTechAndLib("./OpenDB/tests/data/gscl45nm.lib","./OpenDB/tests/data/gscl45nm.lef")
libs = db.getLibs()
def_parser = odb.defin(db)
chip  = def_parser.createChip(libs,"./OpenDB/tests/data/design.def")
if chip == None:
    exit("Read DEF Failed")
print(chip)
exit()
