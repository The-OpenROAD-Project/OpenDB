import importlib.util
spec = importlib.util.spec_from_file_location("opendbpy", "./OpenDB/build/src/swig/python/opendbpy.py")
odb = importlib.util.module_from_spec(spec)
spec.loader.exec_module(odb)
db = odb.dbDatabase.create()
chip = odb.odb_read_design(db, ["./OpenDB/tests/data/gscl45nm.lef"], ["./OpenDB/tests/data/design.def"])
libs = db.getLibs()
block = chip.getBlock()
result = odb.odb_write_lef(libs[0], "lef.out")
assert result==1, "LEF not written"
