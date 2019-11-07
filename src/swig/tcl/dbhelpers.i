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
    if (!db->getTech()) {
        return lefParser.createTechAndLib(libname, path);
    } else {
        return lefParser.createLib(libname, path);
    }
}

std::vector<odb::dbLib*>
read_lef(odb::dbDatabase* db, std::vector<std::string> paths)
{
    if (db == NULL) {
        db = dbDatabase::create();
    }
    std::vector<odb::dbLib*> libs;

    for (std::string &path: paths) {
        libs.push_back(read_lef(db, path.c_str()));
    }
    return libs;
}

odb::dbChip*
read_def(std::vector<odb::dbLib*>& libs, std::vector<std::string> paths)
{
    if (paths.size() != 1) {
        fprintf(stderr, "Only one DEF file should be provided.\n");
        return NULL;
    }
    if (!libs.size()) {
        fprintf(stderr, "No library file(s) provided.\n");
        return NULL;
    }
    dbDatabase *db = libs[0]->getDb();
    defin defParser(db);
    return defParser.createChip(libs, paths[0].c_str());
}

odb::dbChip*
read_def(odb::dbDatabase* db, std::vector<std::string> paths)
{
    std::vector<odb::dbLib *> libs;
    for (auto it = db->getLibs().begin(); it != db->getLibs().end(); it++) {
        libs.push_back(*it);
    }
    return read_def(libs, paths);
}

odb::dbChip*
read_design(odb::dbDatabase* db, std::vector<std::string> &lef_path, std::vector<std::string> def_path)
{
    if (db == NULL) {
        db = dbDatabase::create();
    }
    auto libs = read_lef(db, lef_path);
    dbChip *chip = read_def(libs, def_path);
    return chip;
}

odb::dbChip*
read_design(odb::dbDatabase* db, std::vector<std::string> def_path)
{
    if (db == NULL) {
        db = dbDatabase::create();
    }
    dbChip *chip = read_def(db, def_path);
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
std::vector<odb::dbLib*>     read_lef(odb::dbDatabase* db, std::vector<std::string> path);
odb::dbChip*     read_def(odb::dbDatabase* db, std::vector<std::string> paths);
odb::dbChip*     read_def(std::vector<odb::dbLib*>& libs, std::vector<std::string> paths);
odb::dbChip*     read_design(odb::dbDatabase* db, std::vector<std::string> def_path);
odb::dbChip*     read_design(odb::dbDatabase* db, std::vector<std::string> lef_path, std::vector<std::string> def_path);
int     write_def(odb::dbBlock* block, const char* path, odb::defout::Version version = odb::defout::Version::DEF_5_5);
int     write_lef(odb::dbLib* lib, const char* path);
odb::dbDatabase* import_db(odb::dbDatabase* db, const char* db_path);
int         export_db(odb::dbDatabase* db, const char* db_path);