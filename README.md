# Introduction

OpenDB is a design database to support tools for physical chip
design. It was originally developed by Athena Design Systems. Nefelus,
Inc acquired the rights to the code and open sourced it in 2019 to
support the DARPA OpenRoad project.

The structure of OpenDB is based on the Cadence Design Systems text
file formats LEF (library) and DEF (design) formats version 5.6.
OpenDB supports a binary file format to save and load the design much
faster than using LEF and DEF.

OpenDB is written in C++ 98 with standard library style iterators.
The classes are designed to be fast enough to base an application
on without having to copy them into application specific structures.


#### Installation

```
git clone https://github.com/The-OpenROAD-Project/OpenDB.git
cd OpenDB
mkdir build
cd build
cmake ..
make
```

The original Athena code found in /zrouter is built using Makefiles.
See zrouter/src/BUILD/compilation_package_dependencies.

#### Directory structure

```
include/opendb/db.h - public header for all database classes
src/db - private/internal database representations
src/lefin - LEF reader
src/lefout - LEF writer
src/defin - DEF reader
src/defout - DEF writer
zroute - original Athena code hierarchy
```

#### Database API

All public database classes are defined in db.h. These class
definitions provide all functions for examining and modifying the
database objects. The database is an object itself so multiple
database objects can exist simultaineously (no global state).

dbTypes.h defines types returned by database class member functions.

All database objects are in the 'ads' namespace.

dbChip
dbBlock
dbTech
dbLib

All database objects have a 32bit object identifier accessed with the
dbObject::getOID base class member function that returns a
'uint'. This identifier is preserved across save/restores of the
database so it should be used to reference database object by data
structures instead of pointers if the reference lifetime is across
database save/restores. OIDs allow the database to have exactly the
same layout across save/restores.

The database distance units are nanometers and use the type 'uint'.

#### Database Internals
