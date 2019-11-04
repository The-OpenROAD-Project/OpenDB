%{
#include <libgen.h>
odb::dbLib*
read_lef(odb::dbDatabase* db, const char* path)
{
    if (db == NULL) {
        db = dbDatabase::create();
    }
    lefin lefParser(db, false);
    const char *libname = basename(const_cast<char*>(path));
    return lefParser.createTechAndLib(libname, path);
}

odb::dbChip*
read_def(odb::dbLib* lib, const char* path)
{
    std::vector<odb::dbLib *> libs;
    libs.push_back(lib);
    dbDatabase *db = lib->getDb();
    defin defParser(db);
    return defParser.createChip(libs, path);
}

odb::dbChip*
read_design(odb::dbDatabase* db, const char* lef_path, const char* def_path)
{
    if (db == NULL) {
        db = dbDatabase::create();
    }
    dbLib *lib = read_lef(db, lef_path);
    dbChip *chip = read_def(lib, def_path);
    return chip;
}

int     write_def(odb::dbBlock* block, const char* path, odb::defout::Version version = odb::defout::Version::DEF_5_5) {
    defout writer;
    writer.setVersion(version);
    return writer.writeBlock(block, path);
}
int     write_lef(odb::dbLib* lib, const char* path) {
    lefout writer;
    return writer.writeTechAndLib(lib, path);
}

odb::dbDatabase*
import_db(odb::dbDatabase* db, const char* db_path)
{
    if (db == NULL) {
        db = odb::dbDatabase::create();
    }
    FILE *fp = fopen(db_path, "rb");
    if (!fp) {
        int errnum = errno;
        fprintf(stderr, "Error opening file: %s\n", strerror( errnum ));
        fprintf(stderr, "Errno: %d\n", errno);
        return NULL;
    }
    db->read(fp);
    fclose(fp);
    return db;
}

int
export_db(odb::dbDatabase* db, const char* db_path)
{
    FILE *fp = fopen(db_path, "wb");
    if (!fp) {
        int errnum = errno;
        fprintf(stderr, "Error opening file: %s\n", strerror( errnum ));
        fprintf(stderr, "Errno: %d\n", errno);
        return errno;
    }
    db->write(fp);
    fclose(fp);
    return 1;
}
%}
odb::dbLib*      read_lef(odb::dbDatabase* db, const char* path);
odb::dbChip*     read_def(odb::dbLib* lib, const char* path);
odb::dbChip*     read_design(odb::dbDatabase* db, const char* lef_path, const char* def_path);
int     write_def(odb::dbBlock* block, const char* path, odb::defout::Version version = odb::defout::Version::DEF_5_5);
int     write_lef(odb::dbLib* lib, const char* path);
odb::dbDatabase* import_db(odb::dbDatabase* db, const char* db_path);
int         export_db(odb::dbDatabase* db, const char* db_path);