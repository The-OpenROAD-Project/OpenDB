%{
#include <libgen.h>
#include "lefin.h"
#include "lefout.h"
#include "defin.h"
#include "defout.h"

odb::dbLib*
odb_read_lef(odb::dbDatabase* db, const char* path)
{
  lefin lefParser(db, false);
  const char *libname = basename(const_cast<char*>(path));
  if (!db->getTech()) {
    return lefParser.createTechAndLib(libname, path);
  } else {
    return lefParser.createLib(libname, path);
  }
}

odb::dbChip*
odb_read_def(odb::dbDatabase* db, std::string path)
{
  std::vector<odb::dbLib *> libs;
  for (dbLib *lib : db->getLibs()) {
    libs.push_back(lib);
  }
  defin defParser(db);
  return defParser.createChip(libs, path.c_str());
}

int
odb_write_def(odb::dbBlock* block, const char* path,
	      odb::defout::Version version = odb::defout::Version::DEF_5_5) {
  defout writer;
  writer.setVersion(version);
  return writer.writeBlock(block, path);
}
int     odb_write_lef(odb::dbLib* lib, const char* path) {
    lefout writer;
    return writer.writeTechAndLib(lib, path);
}

odb::dbDatabase*
odb_read_db(odb::dbDatabase* db, const char* db_path)
{
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
odb_write_db(odb::dbDatabase* db, const char* db_path)
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

odb::dbLib*
odb_read_lef(odb::dbDatabase* db, const char* path);
odb::dbChip*
odb_read_def(odb::dbDatabase* db, std::string path);

int
odb_write_def(odb::dbBlock* block, const char* path,
	      odb::defout::Version version = odb::defout::Version::DEF_5_5);
int
odb_write_lef(odb::dbLib* lib, const char* path);

odb::dbDatabase* odb_read_db(odb::dbDatabase* db, const char* db_path);
int odb_write_db(odb::dbDatabase* db, const char* db_path);
