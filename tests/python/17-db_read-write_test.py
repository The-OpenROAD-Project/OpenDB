import opendbpy as odb
import os 

current_dir = os.path.dirname(os.path.realpath(__file__))
tests_dir = os.path.abspath(os.path.join(current_dir, os.pardir))
opendb_dir = os.path.abspath(os.path.join(tests_dir, os.pardir))
data_dir = os.path.join(tests_dir, "data")

db = odb.dbDatabase.create()
chip = odb.odb_read_design(db, [os.path.join(data_dir, "gscl45nm.lef")], [os.path.join(data_dir, "design.def")])
tech = db.getTech()
libs = db.getLibs()

if chip == None:
    exit("ERROR: READ DEF Failed")

export_result = odb.odb_export_db(db, os.path.join(opendb_dir, "build/export.db"))
if export_result!=1:
    exit("Export DB Failed")

new_db = odb.dbDatabase.create()
new_db = odb.odb_import_db(new_db, os.path.join(opendb_dir, "build/export.db"))

if new_db == None:
    exit("Import DB Failed")

if odb.db_diff(db, new_db):
    exit("Error: Difference found between exported and imported DB")
