import importlib.util
spec = importlib.util.spec_from_file_location("opendbpy", "./OpenDB/build/src/swig/python/opendbpy.py")
odb = importlib.util.module_from_spec(spec)
spec.loader.exec_module(odb)
db = odb.dbDatabase.create()
chip = odb.odb_read_design(db, ["./OpenDB/tests/data/gscl45nm.lef"], ["./OpenDB/tests/data/design.def"])
tech = db.getTech()
libs = db.getLibs()

if chip == None:
    exit("ERROR: READ DEF Failed")

export_result = odb.odb_export_db(db, "./OpenDB/build/export.db")
if export_result!=1:
    exit("Export DB Failed")

new_db = odb.dbDatabase.create()
new_db = odb.odb_import_db(new_db, "./OpenDB/build/export.db")

if new_db == None:
    exit("Import DB Failed")

diff_file = open("./OpenDB/build/db-export-import-diff.txt", "w+")
diff_file_content = diff_file.read()
diff_file.close()

if diff_file_content != "":
    exit("Error: Difference found between exported and imported DB")
    
