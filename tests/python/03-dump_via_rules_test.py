import importlib.util
spec = importlib.util.spec_from_file_location("opendbpy", "../../build/src/swig/python/opendbpy.py")
odb = importlib.util.module_from_spec(spec)
spec.loader.exec_module(odb)
db = odb.dbDatabase.create()
lef_parser = odb.lefin(db, True)
tech = lef_parser.createTech("../data/gscl45nm.lef")
if tech == None:
    exit("Tech not created")
 
via_rules = tech.getViaRules()

if via_rules == None:
    exit("Via rules not found")
