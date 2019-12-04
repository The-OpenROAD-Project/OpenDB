import importlib.util
spec = importlib.util.spec_from_file_location("opendbpy", "./OpenDB/build/src/swig/python/opendbpy.py")
odb = importlib.util.module_from_spec(spec)
spec.loader.exec_module(odb)
db = odb.dbDatabase.create()
chip = odb.odb_read_design(db, ["./OpenDB/tests/data/gscl45nm.lef"], ["./OpenDB/tests/data/design.def"])
if chip == None:
    exit("Read DEF failed")
tech = db.getTech()
lib = db.getLibs()

vias = tech.getVias()
via1 = vias[0]
layer1 = via1.getBottomLayer()
via2 = vias[1]
via3 = vias[2]
block = chip.getBlock()
net = odb.dbNet_create(block, "w1")
wire = odb.dbWire_create(net)
wire_encoder =  odb.dbWireEncoder()

wire_encoder.begin(wire)
wire_encoder.newPath(layer1, "ROUTED")
wire_encoder.addPoint(2000,2000)

jid1 = wire_encoder.addPoint(10000,2000)
wire_encoder.addPoint(18000,2000)
wire_encoder.newPath(jid1)
wire_encoder.addTechVia(via1)

jid2 = wire_encoder.addPoint(10000,10000)
wire_encoder.addPoint(10000,18000)
wire_encoder.newPath(jid2)

jid3=wire_encoder.addTechVia(via2)
wire_encoder.end()

result = odb.odb_write_def(block, "./OpenDB/build/wire_encoder.def")
if(result!=1):
    exit("Write DEF failed")
