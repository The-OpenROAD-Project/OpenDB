import importlib.util
spec = importlib.util.spec_from_file_location("opendbpy", "./OpenDB/build/src/swig/python/opendbpy.py")
odb = importlib.util.module_from_spec(spec)
spec.loader.exec_module(odb)
db = odb.dbDatabase.create()
lef_parser = odb.lefin(db, True)
tech = lef_parser.createTech("./OpenDB/tests/data/gscl45nm.lef")
vias = tech.getVias()
for via in vias:
    via.getName()
assert len(vias) == 14, "Number of vias mismatch"
