import importlib.util
spec = importlib.util.spec_from_file_location("opendbpy", "./OpenDB/build/src/swig/python/opendbpy.py")
odb = importlib.util.module_from_spec(spec)
spec.loader.exec_module(odb)
db = odb.dbDatabase.create()
chip = odb.odb_read_design(db,["./OpenDB/tests/data/Nangate45/NangateOpenCellLibrary.mod.lef"],
["./OpenDB/tests/data/gcd/floorplan.def"])
tech = db.getTech()
libs = db.getLibs()
lib = libs[0]
block = chip.getBlock()
sites = lib.getSites()
site = sites[0]

rt = odb.dbRow_create(block,"ROW_TEST", site, 0, 380, "MX", "HORIZONTAL", 420, 380)

assert rt.getName() == "ROW_TEST", "Rown name mismatch"
assert rt.getOrigin() == [0,380], "Row origin mismatch"
assert rt.getSite().getName() == site.getName(), "Row Site name mismatch"
assert rt.getDirection() == "HORIZONTAL", "Row diretion mismatch"
assert rt.getOrient() == "MX", "Row orientation mismatch"
assert rt.getSpacing() == 380, "Row spacing mismatch"
assert rt.getSiteCount() == 420, "row site count mismatch"

rt1 = odb.dbRow_create(block, "ROW_TEST", site, 0, 380, "R0", "HORIZONTAL", 420, 380)
assert rt1.getDirection() == "HORIZONTAL" , "Row get direction mismatch"
assert rt1.getOrient() == "R0", "Row orientation mismatch"

rt2 = odb.dbRow_create(block, "ROW_TEST", site, 0, 380, "R0", "VERTICAL", 420, 380)
assert rt2.getDirection() == "VERTICAL", "Row get direction mismatch"
assert rt2.getOrient() == "R0", "Row get orientation mismatch"
