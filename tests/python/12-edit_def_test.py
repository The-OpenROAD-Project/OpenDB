import importlib.util
spec = importlib.util.spec_from_file_location("opendbpy", "./OpenDB/build/src/swig/python/opendbpy.py")
odb = importlib.util.module_from_spec(spec)
spec.loader.exec_module(odb)
db = odb.dbDatabase.create()
chip = odb.odb_read_design(db, ["./OpenDB/tests/data/gscl45nm.lef"], ["./OpenDB/tests/data/design.def"])
tech = db.getTech()
libs = db.getLibs()
lib = libs[0]

block = chip.getBlock()
net = odb.dbNet_create(block, "w1")

net.setSigType("POWER")
swire = odb.dbSWire_create(net, "ROUTED")
if (swire.getNet().getName() != net.getName()):
    exit("ERROR: Net and signal wire mismatch")

sites = lib.getSites()
site = sites[0]
row = odb.dbRow_create(block, "row0", site, 0, 0, "RO", "HORIZONTAL", 1, 10)
if row == None:
    exit("ERROR: Row not created")
print(net)
print(row)
print(swire)


