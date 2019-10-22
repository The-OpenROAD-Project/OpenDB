///////////////////////////////////////////////////////////////////////////////
// BSD 3-Clause License
//
// Copyright (c) 2019, Nefelus Inc
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// * Neither the name of the copyright holder nor the names of its
//   contributors may be used to endorse or promote products derived from
//   this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef ADS_DB_H
#define ADS_DB_H

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_GEOM_H
#include "geom.h"
#endif

#ifndef ADS_DB_OBJECT_H
#include "dbObject.h"
#endif

#ifndef ADS_DCR_ISDB_H
#include "ISdb.h"
#endif

#ifndef ADS_DB_SET_H
#include "dbSet.h"
#endif

#ifndef ADS_DB_VIA_PARAMS_H
#include "dbViaParams.h"
#endif

#ifndef ADS_DB_TYPES_H
#include "dbTypes.h"
#endif

#ifndef ADS_DB_PRINTCONTROL_H
#include "dbPrintControl.h"
#endif

#include "logger.h"

#include <vector>
#include <set>
#include <list>

#define ADS_MAX_CORNER 10

namespace odb {

class dbShape;
class lefout;
class dbViaParams;
class dbTransform;

template <class T> class dbId;

// Forward declarations of all database objects
class dbDatabase;
class dbBox;
class dbJournalEntry;

// Property objects
class dbProperty;
class dbBoolProperty;
class dbStringProperty;
class dbIntProperty;
class dbDoubleProperty;

// Design objects
class dbChip;
class dbBlock;
class dbBTerm;
class dbNet;
class dbInst;
class dbITerm;
class dbVia;
class dbGCellGrid;
class dbTrackGrid;
class dbObstruction;
class dbBlockage;
class dbWire;
class dbSWire;
class dbSBox;
class dbCapNode;
class dbRSeg;
class dbCCSeg;
class dbBlockSearch;
class dbRow;
class dbTechAntennaPinModel;
class dbBlockCallBackObj;
class dbMetrics;
class dbRegion;
class dbBPin;

// Lib objects
class dbLib;
class dbSite;
class dbMaster;
class dbMTerm;
class dbMPin;
class dbTarget;

// Tech objects
class dbTech;
class dbTechLayer;
class dbTechVia;
class dbTechViaRule;
class dbTechViaLayerRule;
class dbTechViaGenerateRule;
class dbTechNonDefaultRule;
class dbTechLayerRule;
class dbTechLayerSpacingRule;
class dbTechLayerAntennaRule;
class dbTechMinCutRule;
class dbTechMinEncRule;
class dbTechV55InfluenceEntry;
class dbTechSameNetRule;

class dbViaParams;

// Extraction Objects
class dbExtControl;

enum dbTimingMode { TMG_MIN=0, TMG_MAX=1 };

///////////////////////////////////////////////////////////////////////////////
///
/// C-string wrapper classe. Strings returned from the database are returned
/// in an instance of this class.
///
/// Todo: Improve this implementation.
///
///////////////////////////////////////////////////////////////////////////////
class dbString
{
    const char * _str;

    void assign( const char * str )
    {
        if ( _str ) free((void *)_str);
        _str = strdup(str);
        assert(_str);
    }
    
    void copy( const dbString & str )
    {
        assign(str._str);
    }

    void append( const char * str )
    {
        if ( _str == NULL )
            _str = strdup(str);
        else
        {
            int len = strlen(_str) + strlen(str);
            char * s = (char *) malloc(len+1);
            strcpy(s,_str);
            strcat(s,str);
            free((void *) _str);
            _str = s;
        }

        assert(_str);
    }

  public:
    dbString()                       { _str = NULL; }
    dbString( const char * str )     { _str = strdup(str); }
    dbString( const dbString & str ) { _str = NULL; copy(str); }
    ~dbString()                      { if(_str) free((void *)_str); }

    bool operator==(const char * str) { return strcmp( _str, str ) == 0; }
    bool operator!=(const char * str) { return strcmp( _str, str ) != 0; }
    bool operator==(const dbString & str) { return strcmp( _str, str._str ) == 0; }
    bool operator!=(const dbString & str) { return strcmp( _str, str._str ) != 0; }

    ///
    /// dbString assignment operator;
    ///
    dbString & operator=( const dbString & str )
    {
	if ( &str != this ) {
          copy(str);
        }
        return *this;
    }

    ///
    /// dbString assignment operator;
    ///
    dbString & operator=( const char * str )
    {
        assign(str); return *this;
    }

    ///
    /// dbString "+" operator;
    ///
    dbString operator+( const dbString & str )
    {
        dbString s(_str);
        s += str;
        return s;
    }

    ///
    /// dbString "+" operator;
    ///
    dbString & operator+=( const dbString & str )
    {
        append(str._str); return *this;
    }

    ///
    /// dbString "+" operator;
    ///
    dbString operator+( const char * str )
    {
        dbString s(_str);
       s += str;
        return s;
    }

    ///
    /// dbString "+" operator;
    ///
    dbString & operator+=( const char * str )
    {
        append(str); return *this;
    }

    ///
    /// Cast operator const char *:
    ///    dbString s = getName();
    ///    const char * p = s;
    operator const char * () { return _str; }

    ///
    /// c_str - Get the c-string pointer.
    ///
    const char * c_str() { return _str; }

    ///
    /// Return length of string.
    ///
    uint length() 
    {
        if( _str == NULL ) return 0;
        return strlen(_str);
    }
};

///
/// dbProperty - Property base class.
///
class dbProperty : public dbObject
{
  public:

    enum Type 
    {
        // Do not change the order or the values of this enum.
        STRING_PROP  = 0,
        BOOL_PROP    = 1,
        INT_PROP     = 2,
        DOUBLE_PROP  = 3
    };
    
    /// Get the type of this property.
    Type getType();

    /// Get thetname of this property.
    dbString getName();

    /// Get the owner of this property
    dbObject * getPropOwner();

    /// Find the named property. Returns NULL if the property does not exist.
    static dbProperty * find( dbObject * object, const char * name );

    /// Find the named property of the specified type. Returns NULL if the property does not exist.
    static dbProperty * find( dbObject * object, const char * name, Type type );

    /// Destroy a specific property
    static void destroy( dbProperty * prop );
    /// Destroy all properties of the specific object
    static void destroyProperties( dbObject * obj );
    static dbSet<dbProperty> getProperties( dbObject * object );
    static dbSet<dbProperty>::iterator destroy( dbSet<dbProperty>::iterator itr );
};

///
/// dbProperty - Boolean property.
///
class dbBoolProperty : public dbProperty
{
  public:
    /// Get the value of this property.
    bool getValue();

    /// Set the value of this property.
    void setValue( bool value );

    /// Create a bool property. Returns NULL if a property with the same name already exists.
    static dbBoolProperty * create( dbObject * object, const char * name, bool value );

    /// Find the named property of type bool. Returns NULL if the property does not exist.
    static dbBoolProperty * find( dbObject * object, const char * name );
};

///
/// dbProperty - String property.
///
class dbStringProperty : public dbProperty
{
  public:
    
    /// Get the value of this property.
    dbString getValue();

    /// Set the value of this property.
    void setValue( const char * value );

    /// Create a string property. Returns NULL if a property with the same name already exists.
    static dbStringProperty * create( dbObject * object, const char * name, const char * value );

    /// Find the named property of type string. Returns NULL if the property does not exist.
    static dbStringProperty * find( dbObject * object, const char * name );
};

///
/// dbProperty - Int property.
///
class dbIntProperty : public dbProperty
{
  public:
    
    /// Get the value of this property.
    int getValue();

    /// Set the value of this property.
    void setValue( int value );

    /// Create a int property. Returns NULL if a property with the same name already exists.
    static dbIntProperty * create( dbObject * object, const char * name, int value );

    /// Find the named property of type int. Returns NULL if the property does not exist.
    static dbIntProperty * find( dbObject * object, const char * name );
};

///
/// dbProperty - Double property.
///
class dbDoubleProperty : public dbProperty
{
  public:
    
    /// Get the value of this property.
    double getValue();

    /// Set the value of this property.
    void setValue( double value );

    /// Create a double property. Returns NULL if a property with the same name already exists.
    static dbDoubleProperty * create( dbObject * object, const char * name, double value );

    /// Find the named property of type double. Returns NULL if the property does not exist.
    static dbDoubleProperty * find( dbObject * object, const char * name );
};

///////////////////////////////////////////////////////////////////////////////
///
/// This class encapsulates a persitant ADS database.
/// You can open multiple databases, however, the number
/// of open databases is limited to the amount
/// of physical memory available.
///
///////////////////////////////////////////////////////////////////////////////
class dbDatabase : public dbObject
{
  public:
    ///
    /// Return the libs contained in the database. A database can contain
    /// multiple libs.
    ///
    dbSet<dbLib> getLibs();
    
    ///
    /// Find a specific lib.
    /// Returns NULL if no lib was found.
    ///
    dbLib * findLib( const char * name );

    ///
    /// Find a specific master
    /// Returns NULL if no master is found.
    ///
    dbMaster * findMaster( const char * name );

    ///
    /// Get the chip of this database.
    /// Returns NULL if no chip has been created.
    ///
    dbChip * getChip();

    ///
    /// Get the technology of this database
    /// Returns NULL if no chip has been created.
    ///
    dbTech * getTech();
    
    ////////////////////////
    /// DEPRECIATED
    ////////////////////////
    /// Return the chips contained in the database. A database can contain
    /// multiple chips.
    ///
    dbSet<dbChip> getChips();

    ///
    /// Returns the number of masters
    ///
    uint getNumberOfMasters();

    ///
    /// Translate a database-name to a database object.
    ///
    // dbObject * resolveDbName( const char * dbname );
    
    ///
    /// Read a database from this stream.
    /// WARNING: This function destroys the data currently in the database.
    /// Throws ZIOError..
    ///
    void read( FILE * file );

    ///
    /// Write a database to this stream.
    /// Throws ZIOError..
    ///
    void write( FILE * file );

    /// Throws ZIOError..
    void writeTech( FILE * file );
    void writeLib( FILE * file, dbLib * lib );
    void writeLibs( FILE * file );
    void writeBlock( FILE * file, dbBlock * block );
    void writeChip( FILE * file );
    void writeWires( FILE * file, dbBlock * block );
    void writeNets( FILE * file, dbBlock * block );
    void writeParasitics( FILE * file, dbBlock * block );
    void writeTiming( FILE * file, dbBlock * block );
    void writeTiming( FILE * file, dbBlock * block, dbTimingMode mode );
    void readTech( FILE * file );
    void readLib( FILE * file, dbLib * );
    void readLibs( FILE * file );
    void readBlock( FILE * file, dbBlock * block );
    void readWires( FILE * file, dbBlock * block );
    void readNets( FILE * file, dbBlock * block );
    void readParasitics( FILE * file, dbBlock * block );
    void readChip( FILE * file );

    enum TimingReadOp
    {
        TMG_OVERWRITE,
        TMG_MERGE_MIN,
        TMG_MERGE_MAX,
    };
    
    void readTiming( FILE * file, dbBlock * block, TimingReadOp slew_op[2], TimingReadOp slack_op[2] );
    void readTiming( FILE * file, dbBlock * block, dbTimingMode mode, TimingReadOp slew_op, TimingReadOp slack_op );

    ///
    /// ECO - The following methods implement a simple ECO mechanism for capturing
    /// netlist changes. The intent of the ECO mechanism is to support delta changes
    /// that occur in a "remote" node that must be applied back to the "master" node.
    /// Being as such, the database on the "remote" must be an exact copy of the
    /// "master" database, prior to changes. Furthermore, the "master" database
    /// cannot change prior to commiting the eco to the block.
    ///
    /// WARNING: If these invariants does not hold then the results will be
    ///          unpredictable.
    ///

    ///
    /// Begin collecting netlist changes on specified block.
    /// 
    /// NOTE: Eco changes can not be nested at this time.
    ///
    static void beginEco( dbBlock * block );

    ///
    /// End collecting netlist changes on specified block.
    /// 
    static void endEco( dbBlock * block );

    ///
    /// Returns true of the pending eco is empty
    /// 
    static bool ecoEmpty( dbBlock * block );

    ///
    /// Read the eco changes from the specified stream to be applied to the specified
    /// block.
    ///
    static void readEco( dbBlock * block, FILE * stream );

    ///
    /// Write the eco netlist changes to the specified stream.
    ///
    static void writeEco( dbBlock * block,  FILE * stream );
    static int checkEco( dbBlock * block);

    ///
    /// Commit any pending netlist changes.
    ///
    static void commitEco( dbBlock * block );

    ///
    /// Begin collecting timing changes on specified block.
    /// 
    /// NOTE: Eco changes can not be nested at this time.
    ///
    static void beginTmgEco( dbBlock * block, int scenario );

    ///
    /// End collecting timing changes on specified block.
    /// 
    static void endTmgEco( dbBlock * block );

    ///
    /// Returns true of the pending eco is empty
    /// 
    static bool ecoTmgEmpty( dbBlock * block );

    ///
    /// Merge the scenario changes from the specified stream to be applied to the specified
    /// block.
    ///
    static void mergeTmgEco( dbBlock * block, FILE * stream, int scenario );

    ///
    /// Write the eco netlist changes to the specified stream.
    ///
    static void writeTmgEco( dbBlock * block,  FILE * stream );

    static int checkTmgEco( dbBlock * block);

    ///
    /// Commit any pending timing changes.
    ///
    static void commitTmgEco( dbBlock * block );

    ///
    /// Initializes the database to nothing.
    ///
    void clear();

    ///
    /// Create an instance of a database
    ///
    static dbDatabase * create();

    ///
    /// Detroy an instance of a database
    ///
    static void destroy( dbDatabase * db );
    
    ///
    /// Create a duplicate (IN-MEMORY) instance of a database.
    ///
    /// WARNING: This action may result in an out-of-memory condition if
    ///          there is not enough memory (or swap space) to maintain
    ///          multiple in-core databases.
    ///
    static dbDatabase * duplicate( dbDatabase * db );

    ///
    /// diff the two databases
    /// Returns true if differences were found.
    ///
    static bool diff( dbDatabase * db0, dbDatabase * db1, FILE * file, int indent_per_level );
    
    ///
    /// Translate a database-id back to a pointer.
    ///
    static dbDatabase * getDatabase( uint oid );
};

///////////////////////////////////////////////////////////////////////////////
///
/// A box is the element used to represent layout shapes.
///
///////////////////////////////////////////////////////////////////////////////
class dbBox : public dbObject
{
  public:
    ///
    /// Get the lower coordinate.
    ///
    int xMin();

    ///
    /// Get the lower y coordinate.
    ///
    int yMin();

    ///
    /// Get the high x coordinate.
    ///
    int xMax();

    ///
    /// Get the high y coordinate.
    ///
    int yMax();

    ///
    /// Returns true if this box represents a via
    ///
    bool isVia();

    ///
    /// Get tech-via this box represents.
    /// returns NULL if this box does not represent a tech-via
    ///
    dbTechVia * getTechVia();

    ///
    /// Get block-via this box represents.
    /// returns NULL if this box does not represent a block-via
    ///
    dbVia * getBlockVia();
    
    ///
    /// Return the placed location of this via.
    ///
    void getViaXY( int & x, int & y );
    
    ///
    /// Get the box bounding points.
    ///
    void getBox( adsRect & rect );

    ///
    /// Get the translated boxes of this via
    ///
    void getViaBoxes( std::vector<dbShape> & boxes );

    ///
    /// Get the width (xMax-xMin) of the box.
    ///
    int getDir();

    ///
    /// Get the width (xMax-xMin) of the box.
    ///
    uint getDX();

    ///
    /// Get the height (yMax-yMin) of the box.
    ///
    uint getDY();
    uint getWidth(uint dir=1);
    uint getLength(uint dir=1);

    ///
    /// Set temporary flag visited
    ///
    void setVisited(bool value);
    bool isVisited();
    void setMarked(bool value);
    bool isMarked();

    ///
    /// Get the owner of this box
    ///
    dbObject * getBoxOwner();

    ///
    /// Get the owner type of this box
    ///
    dbBoxOwner getOwnerType();

    ///
    /// Get the layer of this box.
    /// Returns NULL if this shape is an object bbox.
    /// These bboxes have no layer.
    ///     dbBlock    - bbox has no layer
    ///     dbInst     - bbox has no layer
    ///     dbVia      - bbox has no layer
    ///     dbTechVia  - bbox has no layer
    ///
    /// These bboxes have no layer.
    ///    All dbBox(s) that represent VIA's.     
    ///
    dbTechLayer * getTechLayer();
    
    ///
    /// Add a physical pin to a dbBPin.
    /// Returns NULL if this dbBPin already has a pin.
    ///
    static dbBox * create( dbBPin * bpin, dbTechLayer * layer, int x1, int y1, int x2, int y2 );

    ///
    /// Add a box to a block-via.
    ///
    static dbBox * create( dbVia * via, dbTechLayer * layer, int x1, int y1, int x2, int y2 );

    ///
    /// Add an obstruction to a master.
    ///
    static dbBox * create( dbMaster * master, dbTechLayer * layer, int x1, int y1, int x2, int y2 );

    ///
    /// Add a via obstrction to a master.
    /// This function may fail and return NULL if this via has no shapes.
    ///
    static dbBox * create( dbMaster * master, dbTechVia * via, int x, int y );

    ///
    /// Add a wire-shape to a master-pin.
    ///
    static dbBox * create( dbMPin * pin, dbTechLayer * layer, int x1, int y1, int x2, int y2 );

    ///
    /// Add a via obstrction to a master-pin.
    /// This function may fail and return NULL if this via has no shapes.
    ///
    static dbBox * create( dbMPin * pin, dbTechVia * via, int x, int y );

    ///
    /// Add a shape to a tech-via;
    ///
    static dbBox * create( dbTechVia * via, dbTechLayer * layer, int x1, int y1, int x2, int y2 );

    ///
    /// Add a boundary to a region
    ///
    static dbBox * create( dbRegion * region, int x1, int y1, int x2, int y2 );

    ///
    /// Create a halo on an instance.
    ///
    static dbBox * create( dbInst * inst, int x1, int y1, int x2, int y2 );

    ///
    /// Translate a database-id back to a pointer.
    /// This function translates any dbBox which is part of a block.
    ///
    static dbBox * getBox( dbBlock * block, uint oid );

    /// Translate a database-id back to a pointer.
    /// This function translates any dbBox which is part of a tech.
    ///
    static dbBox * getBox( dbTech * tech, uint oid );

    ///
    /// Translate a database-id back to a pointer.
    /// This function translates any dbBox whichs is part of a master.
    ///
    static dbBox * getBox( dbMaster * master, uint oid );
};

///////////////////////////////////////////////////////////////////////////////
///
/// A sbox is the element used to represent special layout shapes.
///
///////////////////////////////////////////////////////////////////////////////
class dbSBox : public dbBox
{
  public:

    /// Direction of segment
    enum Direction
    {
        UNDEFINED=0,
        HORIZONTAL=1,
        VERTICAL=2
    };
    
    ///
    /// Get the shape type of this wire.
    ///
    dbWireShapeType getWireShapeType();

    ///
    /// Return the specified direction of this segment
    ///
    Direction getDirection();

    ///
    /// Get the swire of this shape
    ///
    dbSWire * getSWire();

    ///
    /// Add a rect to a dbSWire.
    ///
    /// If direction == UNDEFINED
    ///    |(x2-x1)| must be an even number or |(y2-y1)| must be an even number
    ///
    /// If direction == HORIZONTAL
    ///    |(y2-y1)| must be an even number
    ///
    /// If direction == VERTICAL
    ///    |(x2-x1)| must be an even number
    ///
    /// If the direction sementics are not met, this function will return NULL.
    ///
    /// These requirements are a result that the current DEF semantics (5.5) use PATH
    /// statements to output these rectangles, the paths must have even widths.
    static dbSBox * create( dbSWire * swire, dbTechLayer * layer, int x1, int y1, int x2, int y2, dbWireShapeType type, Direction dir = UNDEFINED );

    ///
    /// Add a block-via to a dbSWire
    /// This function may fail and return NULL if this via has no shapes.
    ///
    static dbSBox * create( dbSWire * swire, dbVia * via, int x, int y, dbWireShapeType type );

    ///
    /// Add a tech-via to a dbSWire.
    /// This function may fail and return NULL if this via has no shapes.
    ///
    static dbSBox * create( dbSWire * swire, dbTechVia * via, int x, int y, dbWireShapeType type );

    ///
    /// Translate a database-id back to a pointer.
    /// This function translates any dbBox whichs is part of a block
    ///
    static dbSBox * getSBox( dbBlock * block, uint oid );
};

///////////////////////////////////////////////////////////////////////////////
///
/// A Chip is the element the represents a VLSI/ASIC IC.
///
///////////////////////////////////////////////////////////////////////////////
class dbChip : public dbObject
{
  public:
    ///
    /// Get the top-block of this chip.
    /// Returns NULL if a top-block has NOT been created.
    ///
    dbBlock * getBlock();

    ///
    /// Create a new chip.
    /// Returns NULL if a chip already exists.
    /// Returns NULL if there is no database technology.
    ///
    static dbChip * create( dbDatabase * db );
    
    ///
    /// Translate a database-id back to a pointer.
    ///
    static dbChip * getChip( dbDatabase * db, uint oid );

    ///
    /// Destroy a chip.
    ///
    static void destroy( dbChip * chip );
};

///////////////////////////////////////////////////////////////////////////////
///
/// A Block is the element used to represent a layout-netlist.
/// A Block can have multiple children, however, currently only two-levels
/// of hierarchy is supported.
///
///////////////////////////////////////////////////////////////////////////////
class dbBlock : public dbObject
{
  public:

    ///
    /// Get block chip name.
    ///
    dbString getName();

    ///
    /// Get the block chip name.
    ///
    const char *getConstName();

    ///
    /// Get the bounding box of this block.
    ///
    dbBox * getBBox();

    ///
    /// Get the chip this block belongs too.
    ///
    dbChip * getChip();

    ///
    /// Get the database this block belongs too.
    ///
    dbDatabase * getDataBase();

    ///
    /// Get the parent block this block. Returns NULL if this block is the
    /// top-block of the chip.
    ///
    dbBlock * getParent();

    ///
    /// Returns the hierarchical parent of this block if it exists.
    ///
    dbInst * getParentInst();

    ///
    /// Get the child blocks of this block.
    ///
    dbSet<dbBlock> getChildren();

    ///
    /// Find a specific child-block of this block.
    /// Returns NULL if the object was not found.
    ///
    dbBlock * findChild( const char * name );

    ///
    /// Get all the block-terminals of this block.
    ///
    dbSet<dbBTerm> getBTerms();

    ///
    /// Find a specific bterm of this block.
    /// Returns NULL if the object was not found.
    ///
    dbBTerm * findBTerm( const char * name );

    ///
    /// Get all the instance-terminals of this block.
    ///
    dbSet<dbITerm> getITerms();

    ///
    /// Get the instances of this block.
    ///
    dbSet<dbInst> getInsts();

    ///
    /// Find a specific instance of this block.
    /// Returns NULL if the object was not found.
    ///
    dbInst * findInst( const char * name );

    ///
    /// Find a set of insts. Each name can be real name, or Ixxx, or xxx,
    /// where xxx is the inst oid.
    ///
    bool findSomeInst( const char * names, std::vector<dbInst *> & insts);

    ///
    /// Find a set of masters. Each name can be real name
    ///
    bool findSomeMaster( const char * names, std::vector<dbMaster *> & masters);

    ///
    /// Find a specific iterm of this block.
    ///
    /// The iterm name must be of the form:
    ///
    ///     <instanceName><hierDelimeter><termName>
    ///
    /// For example:   inst0/A
    ///
    dbITerm * findITerm( const char * name );

    ///
    /// Get the obstructions of this block
    ///
    dbSet<dbObstruction> getObstructions();

    ///
    /// Get the blockages of this block
    ///
    dbSet<dbBlockage> getBlockages();

    ///
    /// Get the nets of this block
    ///
    dbSet<dbNet> getNets();

    ///
    /// Get the capNodes of this block
    ///
    dbSet<dbCapNode> getCapNodes();

    ///
    /// Get the rsegs of this block
    ///
    dbSet<dbRSeg> getRSegs();

    ///
    /// Find a specific net of this block.
    /// Returns NULL if the object was not found.
    ///
    dbNet * findNet( const char * name );
	dbNet* findNet_tmg(const char* net_name); // tmg: bNet *tmg_findNet(dbBlock *block, const char *net_name)

    ///
    /// Find a set of nets. Each name can be real name, or Nxxx, or xxx,
    /// where xxx is the net oid.
    ///
    bool findSomeNet( const char * names, std::vector<dbNet *> & nets);

    //
    // Utility to save_lef
    //
    //void dbBlock::saveLef(char *filename);
    void saveLef(char *filename);

    //
    // Utility to save_def
    //
    //void dbBlock::saveDef(char *filename, char *nets);
    void saveDef(char *filename, char *nets);

    //
    // Utility to write db file
    //
    //void dbBlock::writeDb(char *filename, int allNode=0);
    void writeDb(char *filename, int allNode=0);
    
    ///
    /// Find a specific via of this block.
    /// Returns NULL if the object was not found.
    ///
    dbVia * findVia( const char * name );

    ///
    /// Get the vias of this block
    ///
    dbSet<dbVia> getVias();

    ///
    /// Get the GCell grid of this block
    /// Returns NULL if no grid exists.
    ///
    dbGCellGrid * getGCellGrid();
    
    ///
    /// Get the DEF units of this technology.
    ///
    int getDefUnits();

    ///
    /// Set the DEF units of this technology.
    ///
    void setDefUnits( int units );

    ///
    /// Get the Database units per micron.
    ///
    int getDbUnitsPerMicron();

    ///
    /// Get the hierarchy delimeter.
    /// Returns (0) if the delimeter was not set.
    /// A hierarchy delimeter can only be set at the time
    /// a block is created.
    ///
    char getHierarchyDelimeter();

    ///
    /// Set the bus name delimeters
    ///
    void setBusDelimeters( char left, char right );

    ///
    /// Get the bus name delimeters
    /// Left and Right are set to "zero" if the bus delimeters
    /// were not set.
    ///
    void getBusDelimeters( char & left, char & right );

    ///
    /// Get extraction counters
    ///
    void getExtCount(int & numOfNet, int & numOfRSeg, int & numOfCapNode, int & numOfCCSeg);

    ///
    /// Copy RC values from one extDb to another.
    ///
    void copyExtDb(uint fr, uint to, uint extDbCnt, double resFactor, double ccFactor, double gndcFactor);

    ///
    /// Adjust RC values.
    ///
    void adjustRC(double resFactor, double ccFactor, double gndcFactor);

    ///
    /// add cc capacitance to gnd capacitance of capNodes of this block
    /// 
    bool groundCC(float gndFactor);

    ///
    /// adjust CC's of nets
    ///
    bool adjustCC(float adjFactor, double ccThreshHold, std::vector<dbNet *> & nets, std::vector<dbNet *> & halonets);

    ///
    /// undo adjusted CC
    ///
    void undoAdjustedCC(std::vector<dbNet *> & nets, std::vector<dbNet*> & halonets);

    ///
    /// Get the number of process corners.
    ///
    int getCornerCount();

    ///
    /// having independent extraction corners ?
    ///
    bool extCornersAreIndependent();

    ///^M
    /// Get the number of corners kept n this block
    ///^M
    int getCornersPerBlock();

    ///
    /// Get the number of ext dbs
    ///
    int getExtDbCount();

    ///
    /// Get ext corner name by the index in ext Db
    ///
	void getExtCornerName(int corner, char *cName);
	
    ///
    /// Get the index in ext Db by name
    ///
	int getExtCornerIndex(const char* cornerName);

    ///
    /// Set corner name list
    ///
	void setCornerNameList(char *name_list);

    ///
    /// Get corner name list
    ///
	char* getCornerNameList();

    ///
    /// Set the number of process corners. The maximum number of
    /// process corners is limited to 256. This method will
    /// delete all dbRCSeg, dbCCSeg, which depend on this value.
    ///
    void setCornerCount(int cornerCnt, int extDbCnt, const char *name_list);
    void setCornerCount(int cnt);

    ///^M
    /// Set the number of corners kept in this block
    ///^M
    void setCornersPerBlock(int cornersPerBlock);

    ///^M
    /// Initialize the parasitics value tables
    ///^M
    void initParasiticsValueTables();

    ///^M
    /// create child block for one extraction corner
    ///^M
    dbBlock *createExtCornerBlock(uint corner);
    ///^M
    /// find child block for one extraction corner
    ///^M
    dbBlock *findExtCornerBlock(uint corner);
    ///^M
    /// get extraction data block for one extraction corner
    ///^M
    dbBlock *getExtCornerBlock(uint corner);

    ///
    /// Initialize timing information from all iterms and all bterms,
    /// expect power and ground terminals.
    ///
    void initTiming();

    ///^M
    /// Initialize timing data table. set array size to 2*numberOfScenario
    ///^M
    void initTmbTbl();

    ///
    /// Invalidate timing of remote nodes
    ///
    void ecoInvalidateTiming();

    ///
    /// Clears timing information from all iterms and all bterms.
    ///
    void clearTiming();
    int beginTmgUpdate(bool mme, bool noActivePins);
    uint endTmgUpdate();
    void makeSlackProperty(bool activePins=false, int tmgIndex=-1);
    void printSlackProperties(bool activePins=false, int tmgIndex=-1, FILE *fp=NULL);
    void getSlackPropertyName(bool actPins, bool max, const char *slack, int tmgIndex, char *buf);
    uint printSlackTrace(bool max, bool wns, FILE *fp);

    void setSlacks(float wns[2], float tns[2], uint scenario[2]);
    void setTmgUpdateIndex(int v);
    int getTmgUpdateIndex();
    bool is_mme_update();
    void set_mme_update(bool value);


    ///
    /// when stream out a hier. block it will not stream references to children
    ///
    void set_skip_hier_stream(bool value);

    ///
    /// Get the track-grids of this block.
    ///
    dbSet<dbTrackGrid> getTrackGrids();

    ///
    /// Find a specific track-grid.
    /// Returns NULL if a track-grid has not be defined for this layer.
    ///
    dbTrackGrid * findTrackGrid( dbTechLayer * layer );

    ///
    /// Get the rows of this block
    ///
    dbSet<dbRow> getRows();

    ///
    /// Get the list of masters used in this block.
    ///
    void getMasters( std::vector<dbMaster *> & masters );

    ///
    /// Set the die area. The die-area is considered a constant regardless
    /// of the geometric elements of the dbBlock. It is generally a constant
    /// declared in DEF.
    ///
    void setDieArea( const adsRect & rect );

    ///
    /// Get the die area. The default die-area is (0,0,0,0).
    ///
    void getDieArea( adsRect & rect );

    ///
    /// Set print control
    ///
    void setPrintControl(const char *name, const char *netn, const char *instn);

    ///
    /// Get print count if net is in the group of the print type
    ///
    uint getPrintCnt(dbPrintControl::Type ptype, dbNet *net);

    void setPtFile ( FILE *ptf);
    FILE *getPtFile();
    ///
    /// Set the extmain instance.
    ///
    void setExtmi (void *ext);

    ///
    /// Get the extmain instance.
    ///
    void *getExtmi ();

    ///
    /// Get the extraction control settings
    ///
    dbExtControl *getExtControl ();

    ///
    /// Get the extraction corner names
    ///
    void getExtCornerNames(std::list<std::string>&ecl);

    /// 
    /// Get the capacitor-coupled segments.
    ///
    dbSet<dbCCSeg> getCCSegs();

	///
    /// Allow bufferin of altered nes
    ///
    bool isBufferAltered();

    ///
    ///
    ///
    void setBufferAltered(bool value);

    /// 
    /// Build search database for fast area searches for insts
    ///
	// uint makeInstSearchDB();

    /// 
    /// Get search database object for fast area searches on physical objects
    ///
	/// dbBlockSearch *getSearchDb();
	
    ///
    /// reset _netSdb
    ///
    // void resetNetSdb();

    /// ^M
    /// Get search database module for fast area searches on signal nets
    ///^M
    //    ZPtr<ISdb> getSignalNetSdb();
    /// ^M^M
    /// Get search database module for fast area searches on signal nets
    /// Generate netSdb if it does not exist
    /// ^M
    //    ZPtr<ISdb> getSignalNetSdb(ZContext & context, dbTech *tech);

    /// 
    /// Get search database module for fast area searches on physical objects
    ///
	// ZPtr<ISdb> getNetSdb();
    /// ^M
    /// Get search database module for fast area searches on physical objects^M
    /// Generate netSdb if it does not exist
    /// 
	// ZPtr<ISdb> getNetSdb(ZContext & context, dbTech *tech);
    /// 
    /// Remove search database
    /// 
	// void removeSdb(std::vector<dbNet *> & nets);
    /// 
    /// Put insts/nets/tracks on Search DB
    ///
	// dbBlockSearch *initSearchBlock(dbTech *tech, bool nets, bool insts, ZContext & context, bool skipViaCuts=false);

    /// 
    /// Get insts from Search DB given a bbox to do area search
    ///
	// uint getInsts(int x1, int y1, int x2, int y2, std::vector<dbInst *> & result); 


	void updateNetFlags(std::vector<dbNet *> & result);

    ///
    /// destroy coupling caps of nets
    ///
    void destroyCCs(std::vector<dbNet *> & nets);

    ///
    /// destroy RC segments of nets
    ///
    void destroyRSegs(std::vector<dbNet *> & nets);

    ///
    /// destroy capnodes of nets
    ///
    void destroyCNs(std::vector<dbNet *> & nets, bool cleanExtid);

    ///
    /// destroy parasitics of nets
    ///
    void destroyParasitics(std::vector<dbNet *> & nets);
    void destroyCornerParasitics(std::vector<dbNet *> & nets);

    ///
    /// get cc_halo_net's of input nets
    ///
    void getCcHaloNets(std::vector<dbNet *> & changedNets, std::vector<dbNet *> & ccHaloNets);

    ///
    /// destroy old parasitics of nets
    ///
    void destroyOldParasitics(std::vector<dbNet *> & nets, std::vector<uint> * capnn, std::vector<uint> * rsegn);
    void destroyOldCornerParasitics(std::vector<dbNet *> & nets, std::vector<uint> & capnn, std::vector<uint> & rsegn);

    ///
    /// restore old parasitics of nets
    ///
    void restoreOldParasitics(std::vector<dbNet *> & nets, bool coupled_rc, std::vector<dbNet*> &ccHaloNets, std::vector<uint> * capnn, std::vector<uint> * rsegn);
    void restoreOldCornerParasitics(dbBlock *pBlock, std::vector<dbNet *> & nets, bool coupled_rc, std::vector<dbNet*> &ccHaloNets, std::vector<uint> & capnn, std::vector<uint> & rsegn);

    ///
    /// keep old parasitics of nets and replace by zeroRc's'
    ///
    void replaceOldParasitics(std::vector<dbNet *> & nets, std::vector<uint> & capnn, std::vector<uint> & rsegn);

    ///
    /// restore old parasitics
    ///
    void restoreOldParasitics(std::vector<dbNet *> & nets, std::vector<uint> & capnn, std::vector<uint> & rsegn);
 
    ///
    /// keep old parasitics of nets
    /// 
    void keepOldParasitics(std::vector<dbNet *> & nets, bool coupled_rc, std::vector<dbNet*> &ccHaloNets, std::vector<uint> * capnn, std::vector<uint> * rsegn);
    void keepOldCornerParasitics(dbBlock *pBlock, std::vector<dbNet *> & nets, bool coupled_rc, std::vector<dbNet*> &ccHaloNets, std::vector<uint> & capnn, std::vector<uint> & rsegn);

    ///
    /// merge rsegs before doing exttree
    ///
    // void preExttreeMergeRC(double max_cap, uint corner);

    ///
    /// clear
    ///
    void clear();

    ///
    /// get wire_updated nets
    ///
    void getWireUpdatedNets(std::vector<dbNet *> & nets, adsRect *bbox=NULL);

    /// 
    /// return the regions of this design 
    /// 
    dbSet<dbRegion> getRegions();

    /// 
    /// Find a specific region. Returns NULL if the region was not found.
    /// 
    dbRegion * findRegion( const char * name );

    /// 
    ///  Find the non-default-rule
    /// 
    dbTechNonDefaultRule * findNonDefaultRule(  const char * name );

    /// 
    ///  Get the non-default-rules specific to this block.
    /// 
    dbSet<dbTechNonDefaultRule> getNonDefaultRules();

    ///
    ///  add opt statitiscs
    ///
    void addOptStats( const char * identifier, uint addedCells, uint resizedCells, uint removedCells, double slack[2] );

    ///
    ///  Levelelize from set of insts
    ///
	uint levelize(std::vector<dbInst *> & startingInsts, std::vector<dbInst *> & instsToBeLeveled);

    ///
    ///  Levelelize from Primary inputs or inout to sequential
    ///
	uint levelizeFromPrimaryInputs();

    ///
    ///  Levelelize from sequential
    ///
	uint levelizeFromSequential();

    ///
    ///  Mark inst backwards usinh user flag 2
    ///
	int markBackwardsUser2(dbInst *firstInst, bool mark, std::vector<dbInst *> & resultTable);

	///
	///  Mark inst backwards usinh user flag 2
	///
	int markBackwardsUser2(std::vector<dbInst *> & startingInsts, std::vector<dbInst *> & instsToBeLeveled, bool mark, std::vector<dbInst *> & resultTable);
	
	///
	///  Mark net backwards using user flag 2
	///
	int markBackwardsUser2(dbNet *net, bool mark, std::vector<dbInst *> & resultTable);

	///
	///  Mark net backwards using user flag 2
	///
	int markBackwardsUser2(dbNet *net, std::vector<dbInst *> & instsToMark, bool mark, std::vector<dbInst *> & resultTable);

	///
	///  Mark masters as sequential based on mterms being of "use clock"; mark iterms as clocked 
	///
	void markClockIterms();

	///
	/// set First driving iterm on all signal nets; set 0 is none exists
	void setDrivingItermsforNets();

	void clearUserInstFlags();

private:
    friend class ZDB;

    /// 
    /// Build search database for fast area searches
    ///
	void makeSearchDB(bool nets, bool insts, ZContext & context);
public:

    /// 
    /// Get the Container class for Nets Search DB
    ///
	// ZPtr<ISdb> getSearchDbNets();


    /// 
    /// Get the Container class for Insts Search DB
    ///
	// ZPtr<ISdb> getSearchDbInsts();

    ///
    /// Set the number of remote timing scenarios.
    /// The scenarios are labled 1..N
    /// This method will destroy the current set of dbMetrics.
    ///
    void setNumberOfScenarios(int n);

    ///
    /// Get the number of remote timing scenarios.
    /// 
    int getNumberOfScenarios();

    ///
    /// Return the timing metrics for the given scenario.
    /// The remote scenarios are labled 1..N,
    /// getScenario(0) returns the metrics for the main node.
    ///
    dbMetrics * getScenario( int s );

    /// 
    /// This method copies the via-table from the src block to the destination block.
    ///
    /// WARNING: This method deletes any vias previously defined in the destination block.
    ///          If there are wire which reference these vias, the references will be left
    ///          dangling.
    /// 
    static void copyViaTable( dbBlock * dst, dbBlock * src );
    
    /// 
    /// Get the Read Only Container class for Nets Search DB
    ///
	// TODO ZPtr<ISdb_r> getSearchDbNets_r();

    ///
    /// Get the Read Only Container class for Insts Search DB
    ///
    // TODO ZPtr<ISdb_r> getSearchDbInsts_r();

    ///
    /// Create a chip's top-block. Returns NULL of a top-block already
    /// exists.
    ///
    static dbBlock * create( dbChip * chip, const char * name, char hier_delimeter = 0);

    ///
    /// Create a hierachical/child block. This block has no connectivity.
    /// Returns NULL if a block with the same name exists.
    ///
    static dbBlock * create( dbBlock * block, const char * name, char hier_delimeter = 0);

    ///
    /// duplicate - Make a duplicate of the specified "child" block. If name == NULL, the
    /// name of the block is also duplicated. If the duplicated block does not
    /// have a unique name, then "findChild" may return an incorrect block.
    /// UNIQUE child-block-names are not enforced! (This should be fixed)!
    ///
    /// A top-block can not be duplicated. This methods returns NULL if the specified
    /// block has not parent.
    ///
    static dbBlock * duplicate( dbBlock * block, const char * name = NULL );

    ///
    /// Translate a database-id back to a pointer.
    ///
    static dbBlock * getBlock( dbChip * chip, uint oid );

    ///
    /// Translate a database-id back to a pointer.
    ///
    static dbBlock * getBlock( dbBlock * block, uint oid );

    ///
    /// Destroy a block.
    ///
    static void destroy( dbBlock * block );

    ///
    /// Delete the bterm from the block.
    ///
    static dbSet<dbBlock>::iterator destroy( dbSet<dbBlock>::iterator & itr );

    ///
    /// Show the netlist differences of these blocks
    /// Returns true if differences were found.
    ///
    static bool differences( dbBlock * block1, dbBlock * block2, FILE * out, int indent_per_level = 4 );

private:
    void ComputeBBox();
};

///////////////////////////////////////////////////////////////////////////////
///
///
///////////////////////////////////////////////////////////////////////////////

class dbMetrics : public dbObject
{
  public:
    int getWorstSlack();
    void setWorstSlack(int slack);
    int getTotalSlack();
    void setTotalSlack(int slack);
    
    ///
    /// Translate a database-id back to a pointer.
    ///
    static dbMetrics * getMetrics( dbBlock * block, uint oid );

};


///////////////////////////////////////////////////////////////////////////////
///
/// A block-terminal is the element used to represent connections in/out of
/// a block.
///
///////////////////////////////////////////////////////////////////////////////
class dbBTerm : public dbObject
{
  public:
    ///
    /// Get the block-terminal name.
    ///
    dbString getName();

    ///
    /// Get the block-terminal name.
    ///
    const char *getConstName();

    ///
    /// Change the name of the bterm.
    /// Returns true if successful.
    /// Returns false if a bterm with the same name already exists.
    ///
    bool rename( const char * name );

    ///
    /// Set the signal type of this block-terminal.
    ///
    void setSigType( dbSigType type );

    ///
    /// Get the signal type of this block-terminal.
    ///
    dbSigType getSigType();

    ///
    /// Set the IO direction of this block-terminal.
    ///
    void setIoType( dbIoType type );

    ///
    /// Get the IO direction of this block-terminal.
    ///
    dbIoType getIoType();

    ///
    /// Set spef mark of this block-terminal.
    ///
	void setSpefMark(uint v);

    ///
    /// get spef mark of this block-terminal.
    ///
	bool isSetSpefMark();

    ///
    /// Set mark of this block-terminal.
    ///
	void setMark(uint v);

    ///
    /// get mark of this block-terminal.
    ///
	bool isSetMark();

    ///
    /// set ext id of this block-terminal.
    ///
	void setExtId(uint v);

    ///
    /// get ext id of this block-terminal.
    ///
	uint getExtId();

    ///
    /// is this terminal SPECIAL (i.e. not for regular signal routing).
    ///
	bool isSpecial() const;

    ///
    /// set SPECIAL attribute -- expected to be done once by DEF parser.
    ///
	void setSpecial();

    ///
    /// Set the tmg-tmp-D flag. - payam
    ///
    void setTmgTmpD( bool v ); // payam

    ///
    /// returns true if the tmg-tmp-D flag is set. - payam
    ///
    bool isSetTmgTmpD(); // payam

    ///
    /// Set the tmg-tmp-C flag. - payam
    ///
    void setTmgTmpC( bool v ); // payam

    ///
    /// returns true if the tmg-tmp-C flag is set. - payam
    ///
    bool isSetTmgTmpC(); // payam

    ///
    /// Set the tmg-tmp-B flag.
    ///
    void setTmgTmpB( bool v );

    ///
    /// returns true if the tmg-tmp-B flag is set.
    ///
    bool isSetTmgTmpB();

    ///
    /// Set the tmg-tmp-A flag.
    ///
    void setTmgTmpA( bool v );

    ///
    /// returns true if the tmg-tmp-A flag is set.
    ///
    bool isSetTmgTmpA();

    /// Get the net of this block-terminal.
    ///
    dbNet * getNet();

    ///
    /// Get the block of this block-terminal.
    ///
    dbBlock * getBlock();

    ///
    /// Get the hierarchical parent iterm of this bterm.
    ///
    /// Returns NULL if this bterm has no parent iterm.
    ///
    ///
    ///     (top-block)
    ///     +------------------------------------------------------------------+
    ///     |                                                                  |
    ///     |                                                                  |
    ///     |                                                                  |
    ///     |               (child-block / instance)                           |
    ///     |               +----------------------------------+               |
    ///     |               |                                  |               |
    ///     |B             I|B                                 |               |
    ///     |o.............o|o..........                       |               |
    ///     |  (net in      |  (net in child-block)            |               |
    ///     |   top-block)  |                                  |               |
    ///     |               |                                  |               |
    ///     |               |                                  |               |
    ///     |               |                                  |               |
    ///     |               |                                  |               |
    ///     |               +----------------------------------+               |
    ///     |                                                                  |
    ///     |                                                                  |
    ///     +------------------------------------------------------------------+
    ///
    ///
    /// B = dbBterm
    /// I = dbIterm
    ///
    dbITerm * getITerm();

    ///
    /// Initialize timing.
    ///
    ///    slewRise[MIN] = infinity
    ///    slewFall[MIN] = infinity
    ///    slackRise[MIN] = infinity
    ///    slackFall[MIN] = infinity
    ///    slewRise[MAX] = -infinity
    ///    slewFall[MAX] = -infinity
    ///    slackRise[MAX] = infinity
    ///    slackFall[MAX] = infinity
    ///
    ///  where:
    ///
    ///    infinity = INT_MAX
    ///    -infinity = INT_MIN
    ///
    void initTiming();

    // payam: changed int to float
    float getSlewRise( dbTimingMode mode );
    float getSlewFall( dbTimingMode mode );
    float getSlackRise( dbTimingMode mode );
    float getSlackFall( dbTimingMode mode );
    float getSlewRise( dbTimingMode mode, uint scenario );
    float getSlewFall( dbTimingMode mode, uint scenario );
    float getSlackRise( dbTimingMode mode, uint scenario );
    float getSlackFall( dbTimingMode mode, uint scenario );
    void setSlewRise( dbTimingMode mode, float value, uint scenario=0 );
    void setSlewFall( dbTimingMode mode, float value, uint scenario=0 );
    void setSlackRise( dbTimingMode mode, float value, uint scenario=0 );
    void setSlackFall( dbTimingMode mode, float value, uint scenario=0 );
    bool getWorstSlack(float wns[2], float tns[2], uint scenario[2]);

    //cc void ecoTiming();

    ///
    /// Get the bpins of this bterm.
    ///
    dbSet<dbBPin> getBPins();

    ///
    /// This method finds the first "placed" dbPin box.
    /// returns false if there are no placed bpins.
    ///
    bool getFirstPin( dbShape & shape );

    ///
    /// This method finds the location the first "placed" dbPin box.
    /// The location is the computed center of the bbox.
    /// returns false if there are no placed bpins. x and y are set to zero.
    //
    bool getFirstPinLocation( int & x, int & y );

    ///
    /// This method returns the placementstatus of the first dbBPin.
    /// Returns NONE if bterm has no dbPins.
    ///
    dbPlacementStatus getFirstPinPlacementStatus();

    ///
    /// Get the ground sensistivity pin (5.6 DEF)
    ///
    dbBTerm * getGroundPin();

    ///
    /// Set the ground sensistivity pin (5.6 DEF)
    ///
    void setGroundPin( dbBTerm * pin );

    ///
    /// Get the supply sensistivity pin (5.6 DEF)
    ///
    dbBTerm * getSupplyPin();

    ///
    /// Set the supply sensistivity pin (5.6 DEF)
    ///
    void setSupplyPin( dbBTerm * pin );
    
    ///
    /// Create a new block-terminal.
    /// Returns NULL if a bterm with this name already exists
    ///
    static dbBTerm * create( dbNet * net, const char * name );

    ///
    /// Delete the bterm from the block.
    ///
    static void destroy( dbBTerm * bterm );

    ///
    /// Delete the bterm from the block.
    
    static dbSet<dbBTerm>::iterator destroy( dbSet<dbBTerm>::iterator & itr );

    ///
    /// Translate a database-id back to a pointer.
    ///
    static dbBTerm * getBTerm( dbBlock * block, uint oid );
};


///////////////////////////////////////////////////////////////////////////////
///
/// A BPIn is the element that represents a physical connection to a block
/// terminal.
///
///////////////////////////////////////////////////////////////////////////////

class dbBPin : public dbObject
{
  public:
    ///
    /// Get the placement status of this block-terminal.
    ///
    dbPlacementStatus getPlacementStatus();

    ///
    /// Set the placement status of this block-terminal.
    ///
    void setPlacementStatus( dbPlacementStatus status );

    ///
    /// Get bterm of this pin.
    ///
    dbBTerm * getBTerm();

    ///
    /// Get bbox of this pin
    ///
    dbBox * getBox();

    ///
    /// Returns true if this bpin has an effective-width rule.
    ///
    bool hasEffectiveWidth();

    ///
    /// Set the effective width rule.
    ///
    void setEffectiveWidth( int w );

    ///
    /// Return the effective width rule.
    ///
    int getEffectiveWidth();

    ///
    /// Returns true if this bpin has an min-spacing rule.
    ///
    bool hasMinSpacing();

    ///
    /// Set the min spacing rule.
    ///
    void setMinSpacing( int w );

    ///
    /// Return the min spacing rule.
    ///
    int getMinSpacing();
    
    ///
    /// Create a new block-terminal-pin
    ///
    static dbBPin * create( dbBTerm * bterm );

    ///
    /// Delete the bpin from this bterm
    ///
    static void destroy( dbBPin * bpin );

    ///
    /// Delete the bterm from the bterm.
    static dbSet<dbBPin>::iterator destroy( dbSet<dbBPin>::iterator & itr );

    ///
    /// Translate a database-id back to a pointer.
    ///
    static dbBPin * getBPin( dbBlock * block, uint oid );
};


///////////////////////////////////////////////////////////////////////////////
///
/// A Net is the element that represents a "net" on a block.
///
///////////////////////////////////////////////////////////////////////////////
class dbNet : public dbObject
{
  public:
    ///
    /// Get the net name.
    ///
    dbString getName();

    ///
    /// Get the net name.
    ///
    const char *getConstName();

    ///
    /// Print net name with or without id and newline
    ///
	void printNetName(FILE *fp, bool idFlag=true, bool newLine=true);

    ///
    /// Change the name of the net.
    /// Returns true if successful.
    /// Returns false if a net with the same name already exists.
    ///
    bool rename( const char * name );

    ///
    /// RC netowork disconnect
    ///
    bool isRCDisconnected();

    ///
    ///
    ///
    void setRCDisconnected(bool value);

    ///
    /// Get the weight assigned to this net.
    /// (Default: 1)
    ///
    int getWeight();

    ///
    /// Set the weight assigned of this net.
    ///
    void setWeight( int weight );

    ///
    /// Get the source assigned to this net.
    ///
    dbSourceType getSourceType();
    
    ///
    /// Set the source assigned of this net.
    ///
    void setSourceType( dbSourceType type );

    ///
    /// Get the x-talk-class assigned to this net.
    /// (Default: 0)
    ///
    int getXTalkClass();

    ///
    /// Set the x-talk-class assigned of this net.
    ///
    void setXTalkClass( int value );

    ///
    /// Set the driving term id assigned of this net.
    ///
	void setDrivingITerm( int id );

    ///
    /// Returns driving term id assigned of this net. -1 if not set, 0 if non existent
    ///
	int getDrivingITerm();

    ///
    /// Returns true if a fixed-bump flag has been set.
    ///
    bool hasFixedBump();

    ///
    /// Set the value of the fixed-bump flag.
    ///
    void setFixedBump( bool value );

    ///
    /// Get the Regular Wiring of a net (TODO: per path)
    ///
    dbWireType getWireType();

    ///
    /// Set the Regular Wiring of a net (TODO: per path)
    ///
    void setWireType(dbWireType wire_type);

    ///
    /// Get the signal type of this block-net.
    ///
    dbSigType getSigType();

    ///
    /// Get the signal type of this block-net.
    ///
    void setSigType( dbSigType sig_type );

    ///
    /// Assuming no intersection, check if the net is in the bbox.
    ///
    bool isEnclosed(adsRect *bbox);

    ///
    /// Returns the mark flag value. This flag specified that the
    /// net has been marked.
    ///
    bool isMarked();

    ///
    /// Returns the mark_1 flag value. This flag specified that the
    /// net has been mark_1ed.
    /// 
    bool isMark_1ed();

    ///
    /// Set the mark flag to the specified value.
    ///
    void setMark( bool value );

    ///
    /// Set the mark_1 flag to the specified value.
    ///
    void setMark_1( bool value );

    ///
    /// Returns the select flag value. This flag specified that the
    /// net has been select.
    ///
    bool isSelect();

    ///Net
    /// Set the select flag to the specified value.
    ///
    void setSelect( bool value );

    ///
    /// check if wire of this net equals that of the target net
    /// return value = 0: equal
    ///                x: not equal
    ///                1x: wire seg after junction not equal 
    ///
    uint wireEqual(dbNet *target);

    void wireMatch(dbNet *target);
    void printWire(int fid, int tid, char *type);
    void printWire();
    void printWire(char *type);
    ///
    /// Returns the wire-ordered flag value. This flag specified that the
    /// wires of this net have been ordered into a single dbWire.
    ///
    bool isWireOrdered();

    ///
    /// Set the wire-ordered flag to the specified value.
    /// Note: This flag is set to false any time a dbWire
    /// is created on this net.
    ///
    void setWireOrdered( bool value );

    ///
    /// Returns the buffered flag value. This flag specified that the
    /// net has been buffered.
    ///
    bool isBuffered();

    ///
    /// Set the buffered flag to the specified value.
    ///
    void setBuffered( bool value );

    ///
    /// Returns the disconnected flag value. This flag specified that the
    /// wire are connected to all the iterms of this net.
    ///
    bool isDisconnected();

    ///
    /// Set the disconnected flag to the specified value.
    /// Note: This flag is set to false any time a dbWire
    /// is created on this net.
    ///
    void setDisconnected( bool value );
 
	///
	/// wire_update flag to be used at when the wire is replaced with a new wire
	/// NOTE: rcgraph, extracted, ordered, reduced all have to be reset
	///
	void setWireAltered( bool value );
	bool isWireAltered();

	///
	/// rc_graph flag set when Rseg and CapNodes were created
	///
	void setRCgraph( bool value );
	bool isRCgraph();

	///
	/// reduced flag set when Arnoldi modeling takes place
	///
	void setReduced( bool value );
	bool isReduced();

	///
	/// extracted flag set when net was extracted
	///
	void setExtracted( bool value );
	bool isExtracted();

	///
	/// Sinlge bit general purpose flag to be used at spef
	///
	void setSpef( bool value );
	bool isSpef();

    ///
    /// Set/Reset the size-only flag
    ///
    void setSizeOnly( bool v );

    ///
    /// Returns true if the size-only flag is set.
    ///
    bool isSizeOnly();
    
    ///
    /// Set/Reset the don't-touch flag
    ///
    void setDoNotTouch( bool v );

    ///
    /// Returns true if the don't-touch flag is set.
    ///
    bool isDoNotTouch();
    
    ///
    /// Get the block of this block-terminal.
    ///
    dbBlock * getBlock();

    ///
    /// Get all the instance-terminals of this net.
    ///
    dbSet<dbITerm> getITerms();

    ///
    /// Get the 1st instance-terminal of this net.
    ///
    dbITerm * get1stITerm();

    ///
    /// Get the 1st inputSignal Iterm; can be 
    ///
	dbITerm *get1stSignalInput(bool io);

    ///
    /// Get the 1st output Iterm; can be 
    ///
	dbITerm *getFirstOutput();

    ///
    /// Get all the block-terminals of this net.
    ///
    dbSet<dbBTerm> getBTerms();

    ///
    /// Get the 1st block-terminal of this net.
    ///
    dbBTerm * get1stBTerm();

    ///
    /// Get the special-wires of this net.
    ///
    dbSet<dbSWire> getSWires();

    ///
    /// Get the wire of thie net.
    /// Returns NULL if this net has no wire.
    ///
    dbWire * getWire();

    ///
    /// Get the first swire of this net.
    /// Returns NULL if this net has no swires.
    ///
	dbSWire *getFirstSWire();
    ///
    /// Move segements of the wire of this net to that of tnet
    ///
    void donateWire(dbNet *tnet, dbRSeg **new_rsegs);

    ///
    /// Get the global wire of thie net.
    /// Returns NULL if this net has no global wire.
    ///
    dbWire * getGlobalWire();

    ///
    /// Returns true if this dbNet is marked as special. Special nets/iterms are declared
    /// in the SPECIAL NETS section of a DEF file.
    ///
    bool isSpecial();

    ///
    /// Mark this dbNet as special.
    ///
    void setSpecial();

    ///
    /// Unmark this dbNet as special.
    ///
    void clearSpecial();

    ///
    /// Set the IO flag if there are any BTerms on net
    bool setIOflag();

    ///
    /// returns true if there are BTerms on net
    bool isIO();

    ///
    /// Returns true if this dbNet is was connected using a wild-card.
    ///
    bool isWildConnected();

    ///
    /// Mark this dbNet as wild-connected.
    ///
    void setWildConnected();

    ///
    /// Unmark this dbNet as wild-connected.
    ///
    void clearWildConnected();

    /// 
    /// Get the gndc calibration factor of this net
    ///
	float getGndcCalibFactor();

    ///
    /// Set the gndc calibration factor of this net
    ///
        void setGndcCalibFactor(float gndcCalib);

    ///
    /// Calibrate the capacitance of this net
    ///
        void calibrateCapacitance();

    ///
    /// Calibrate the ground capacitance of this net
    ///
        void calibrateGndCap();

    ///
    /// Calibrate the coupling capacitance of this net
    /// 
        void calibrateCouplingCap();
        void calibrateCouplingCap(int corner);

    ///
    /// Get the cc calibration factor of this net
    ///
        float getCcCalibFactor();

    ///
    /// Set the cc calibration factor of this net
    /// 
        void setCcCalibFactor(float ccCalib);

    ///
    /// Adjust resistances of this net
    ///
        void adjustNetRes(float factor);

    ///
    /// Adjust resistances of this net for a corner
    ///
		void adjustNetRes(float factor, uint corner);

    ///
    /// Adjust ground cap of this net
    ///
        void adjustNetGndCap(float factor);

    ///
    /// Adjust ground cap of this net for a corner
    ///
		void adjustNetGndCap(uint corner, float factor);

    ///
    /// get ccAdjustFactor of this net
    ///
    float getCcAdjustFactor();

    ///
    /// set ccAdjustFactor of this net
    ///
    void setCcAdjustFactor( float factor );

    ///
    /// get ccAdjustOrder of this net
    ///
    uint getCcAdjustOrder();

    ///
    /// set ccAdjustOrder of this net
    ///
    void setCcAdjustOrder( uint order );

    ///
    /// adjust CC's of this net
    ///
    bool adjustCC(uint adjOrder, float adjFactor, double ccThreshHold, std::vector<dbCCSeg *> & adjustedCC, std::vector<dbNet *> & halonets);

    ///
    /// undo adjusted CC
    ///
    void undoAdjustedCC(std::vector<dbCCSeg *> & adjustedCC, std::vector<dbNet*> & halonets);

    ///
    /// add cc capacitance to gnd capacitance of capNodes of this net
    /// 
    bool groundCC(float gndFactor);

    ///
    /// Add to the dbCC of this net
    ///
        void addDbCc(float cap);

    ///
    /// Get dbCC of this net
    ///
        float getDbCc();

    ///
    /// Set dbCC of this net
    ///
        void setDbCc(float cap);

    ///
    /// Get refCC of this net
    ///
        float getRefCc();

    ///
    /// Set refCC of this net
    ///
        void setRefCc(float cap);

    ///
    /// get the CC match ratio against this net
    ///
        float getCcMatchRatio();

    ///
    /// set the CC match ratio against this net
    ///
        void setCcMatchRatio(float ratio);

    ///
    /// Get the gdn cap of this net to *gndcap, total cap to *totalcap
    ///
    void getGndTotalCap(double *gndcap, double *totalcap, double MillerMult);

    ///
    /// merge rsegs before doing exttree
    ///
    void preExttreeMergeRC(double max_cap, uint corner);


    /// 
    /// Get Cap Node given a node_num
    ///
	dbCapNode* findCapNode(uint nodeId);
	
    ///
    /// Print the CapNodes of this net.
    ///
    void printCapN(char *type);

    ///
    /// donate parasitics
    ///
    void donateRC(dbITerm *donorterm, dbITerm *rcvterm, dbRSeg *& rtrseg, dbRSeg *& lastrrseg, dbCapNode *& lastrcapnd, uint & ricapndCnt, dbRSeg *& fstdrseg, dbRSeg *& dtrseg, dbCapNode *& fstdcapnd, std::vector<dbCCSeg*> * gndcc, dbRSeg *& bridgeRseg);

    ///
    /// reverse donate parasitics
    ///
    void unDonateRC(dbRSeg * rtrseg, dbRSeg * lastrrseg, dbITerm *it, dbCapNode *lastrcapnd, uint ricapndCnt, dbRSeg *dtrseg, dbRSeg *fstdrseg, dbCapNode *fstdcapnd, dbITerm *ot, std::vector<dbCCSeg*> * gndcc);

    /// 
    /// Get the Cap Nodes of this net.
    ///
    dbSet<dbCapNode> getCapNodes();

    /// 
    /// delete the Cap Nodes of this net.
    ///
    void destroyCapNodes(bool cleanExtid);

    ///
    /// Reverse the rsegs seqence of this net.
    ///
    void reverseRSegs();

    ///
    /// create dummy zero rseg and capNodes
    ///
    void createZeroRc(bool foreign);

    ///
    /// Set the 1st R segment of this net.
    ///
    void set1stRSegId(uint rseg_id);

    ///
    /// Get the zeroth R segment of this net.
    ///
    dbRSeg *getZeroRSeg();

    ///
    /// Get the 1st R segment id of this net.
    ///
    uint get1stRSegId();

    ///
    /// find the rseg having srcn and tgtn
    ///
    dbRSeg * findRSeg(uint srcn, uint tgtn);

    ///
    /// Set the 1st Cap node of this net.
    ///
    void set1stCapNodeId(uint capn_id);

    ///
    /// Get the 1st Cap node of this net.
    /// 
    uint get1stCapNodeId();

    ///
    /// Reset, or Set the extid of the bterms and iterms to the capnode id's
    ///
    void setTermExtIds(int capId);

    ///
    /// check if any of the RSegs has shape_id
    ///
    bool anchoredRSeg();

    /// 
    /// Print the R segments of this net.
    ///
    void printRSeg(char *type);

    ///
    /// Print the Wire and Parasitics segments of this net.
    ///
    void printWnP(char *type);

    ///
    /// get rseg  count
    ///
    uint getRSegCount();

    /// 
    /// Get the RSegs segments.
    ///
    dbSet<dbRSeg> getRSegs();

    ///
    /// compact internal capnode number'
    ///
    void collapseInternalCapNum(FILE *capNodeMap);
    ///
    /// find max number of cap nodes that are internal
    ///
    uint maxInternalCapNum();

    ///
    /// get capNode count
    ///
    uint getCapNodeCount();

    ///
    /// get CC seg count
    ///
    uint getCcCount();

    /// 
    /// delete the R segments of this net.
    ///
    void destroyRSegs();

    ///
    /// reverse the link order of CCsegs of capNodes
    ///
    void reverseCCSegs();

    /// 
    /// Get the source capacitor-coupled segments of this net..
    ///
    void getSrcCCSegs( std::vector<dbCCSeg *> & segs );

    /// 
    /// Get the target capacitor-coupled segments of this net..
    ///
    void getTgtCCSegs( std::vector<dbCCSeg *> & segs );

    ///
    /// Get the nets having coupling caps with this net
    ///
    void getCouplingNets(uint corner, double ccThreshold, std::set<dbNet *> & cnets);

    /// 
    /// delete the capacitor-coupled segments.
    ///
    void destroyCCSegs();

    ///
    /// destroy parasitics
    ///
    void destroyParasitics ();

    /// 
    /// Get total capacitance in FF
    ///
	double getTotalCapacitance(uint corner=0, bool cc=false);

    /// 
    /// Get total coupling capacitance in FF
    ///
	double getTotalCouplingCap(uint corner=0);

    /// 
    /// Get total resistance in mil ohms
    ///
	double getTotalResistance(uint corner=0);

    ///
    /// Set the nondefault rule applied to this net for wiring.
    ///
    void setNonDefaultRule( dbTechNonDefaultRule * rule );

    ///
    /// Get the nondefault rule applied to this net for wiring.
    /// Returns NULLS if there is no nondefault rule.
    ///
    dbTechNonDefaultRule * getNonDefaultRule();

    ///
    /// Get stats of this net
    ///
	void getNetStats (uint & wireCnt, uint & viaCnt, uint &len, uint &layerCnt, uint *levelTable);

    ///
    /// Get wire counts of this net
    ///
    void getWireCount (uint & wireCnt, uint & viaCnt);

    ///
    /// Get wire counts of this signal net
    ///
    void getSignalWireCount (uint & wireCnt, uint & viaCnt);

    ///
    /// Get wire counts of this power net
    ///
    void getPowerWireCount (uint & wireCnt, uint & viaCnt);

    ///
    /// Get term counts of this net
    ///
    uint getTermCount ();

    ///
    /// Get iterm counts of this signal net
    ///
    uint getITermCount ();

    ///
    /// Get bterm counts of this signal net
    ///
    uint getBTermCount ();

    ///
    /// Get print count if net is in the group of the print type
    ///
    uint getPrintCnt( dbPrintControl::Type type );

    ///
    /// Delete the swires of this net
    ///
    void destroySWires();

    ///
    /// Invalidate timing of remote nodes
    ///
    void ecoInvalidateTiming();

    ///
    /// Create a new net.
    /// Returns NULL if a net with this name already exists
    ///
    static dbNet * create( dbBlock * block, const char * name, bool
    skipExistingCheck=false );

    ///
    /// Delete this net from this block.
    ///
    static void destroy( dbNet * net );

    ///
    /// mark nets of a block.
    ///
    static void markNets(std::vector<dbNet *> & nets, dbBlock* block, bool mk);

	///
    /// set level for fanout instances
    ///
	uint setLevelAtFanout(uint level, bool fromPI, std::vector<dbInst *> &instVector);

    ///
    /// Delete the net from the block.
    ///
    static dbSet<dbNet>::iterator destroy( dbSet<dbNet>::iterator & itr );

    ///
    /// Translate a database-id back to a pointer.
    ///
    static dbNet * getNet( dbBlock * block, uint oid );

    ///
    /// Translate a valid database-id back to a pointer.
    ///
    static dbNet * getValidNet( dbBlock * block, uint oid );
};

///////////////////////////////////////////////////////////////////////////////
///
/// A dbInstance is the element used to represent instances of master-cells in
/// a block.
///
///////////////////////////////////////////////////////////////////////////////
class dbInst : public dbObject
{
  public:
    ///
    /// Get the instance name.
    ///
    dbString getName();

    ///
    /// Need a version that does not do strdup every time
    ///
    const char *getConstName();

    ///
    /// Compare, like !strcmp
    ///
    bool isNamed( const char *name );

    ///
    /// Change the name of the inst.
    /// Returns true if successful.
    /// Returns false if a inst with the same name already exists.
    ///
    bool rename( const char * name );

    ///////////////////////////////////////////////////////////////// 
    /// 
    /// IMPORTANT -  INSTANCE PLACEMENT 
    /// 
    /// There are seven methods used to get/set the placement.
    ///
    ///     getOrigin           - Get the origin of this instance (Where the master is 
    ///     setOrigin           - Set the origin of this instance
    ///     getOrient           - Get orient of this instance
    ///     setOrient           - Set orient of this instance
    ///     getLocation         - Get the lower-left corner of this instance
    ///     setLocation         - Set the lower-left corner of this instance
    ///     setLocationOrient   - Set the orient of this instance and maintain the lower-left corner.
    ///
    /// The getLocation/setLocation are used to get and set the lower-left corner of the bounding box
    /// of the instance. These methods use the DEF semantics.
    ///
    ///
    ///  MASTER COORDINATE SYSTEM:
    ///
    ///                                              |
    ///                                            --o-- (0,0)
    ///                                              |
    ///
    ///
    ///   +----------------------+ (Master bbox after rotation applied)
    ///   |                      |
    ///   |                      |
    ///   |                      |
    ///   |                      |
    ///   |                      |
    ///   +----------------------+
    ///  Mx,My
    ///
    ///
    ///  BLOCK COORDINATE SYSTEM:
    ///
    ///                                              |
    ///                                            --o-- (x,y) (True origin (getOrigin/setOrigin)
    ///                                              |
    ///
    ///
    ///   +----------------------+ (Master bbox after rotation applied)
    ///   |                      |
    ///   |                      |
    ///   |                      |
    ///   |                      |
    ///   |                      |
    ///   +----------------------+
    ///  Ix,Ix (Location getLocation/setLocation)
    ///
    ///
    /// getLocation returns:(Note Mx/My is the location of the bbox AFTER rotation)
    ///
    ///     Ix = x + Mx
    ///     Iy = y + My
    ///     
    /// setLocation(x,y) is equivalent to:
    ///     
    ///    dbMaster * master = inst->getMaster();
    ///    adsRect bbox;
    ///    master->getBBox(bbox);
    ///    dbTransform t(getOrient());
    ///    t.apply(bbox);
    ///    inst->setOrigin(x - bbox.xMin(),y - bbox.yMin());
    ///     
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///  WARNING:    
    ///
    ///     If dbInst::setLocation() is called BEFORE dbInst::setOrient() is called with the proper orient
    ///     the "real" origin will be computed incorrectly and the instance will be placed INCORRECTLY.
    ///
    ///     If you want to change the orient relative to the "location" use setLocationOrient(). Otherwise the
    ///     bounding box will be recomputed incorrectly.
    ///
    ///     getLocation/setLocation are provided for backward compatibility and there use should be avoided.
    ///
    /// TRANSFORMS:
    ///
    ///     When using dbTransform() to translate the shapes/pins of an instance, use getOrigin() to correctly
    ///     set up the transform:
    ///
    ///         int x, y;
    ///         inst->getOrigin(x,y);
    ///         dbTransform transform( inst->getOrient(), adsPoint(x,y) );
    ///
    ///         for all shapes of inst:
    ///             transform.apply( shape )
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    ///
    /// Get the "placed" origin of this instance.
    ///
    void getOrigin( int & x, int & y );

    ///
    /// Set the "placed" origin of this instance.
    ///
    void setOrigin( int x, int y );

    ///
    /// Get the orientation of this instance.
    ///
    dbOrientType getOrient();

    ///
    /// Set the orientation of this instance.
    ///
    void setOrient( dbOrientType orient );

    ///
    /// This method returns the lower-left corner
    /// of the bounding box of this instance.
    ///
    void getLocation(int & x, int & y );

    ///
    /// This method sets the lower-left corner
    /// of the bounding box of this instance.
    ///
    void setLocation(int x, int y );

    ///
    /// Set the orientation of this instance.
    /// This method holds the instance lower-left corner in place and
    /// rotates the instance relative to the lower-left corner.
    ///
    /// This method is equivalent to the following:
    ///
    ///     int x, y;
    ///     inst->getLocation();
    ///     inst->setOrient( orient );
    ///     inst->setLocation(x,y);
    ///
    ///
    void setLocationOrient( dbOrientType orient );

    ///
    /// Get the transform of this instance.
    /// Equivalent to getOrient() and getOrigin()
    ///
    void getTransform( dbTransform & t );

    ///
    /// Set the transform of this instance.
    /// Equivalent to setOrient() and setOrigin()
    ///
    void setTransform( dbTransform & t );

    ///
    /// Get the hierarchical transform of this instance.
    ///
    void getHierTransform( dbTransform & t );

    ///////////////////////////////////////////////////////////////// 

    ///
    /// This method returns the lower-left corner
    /// of the bounding box of this instance.
    ///
    dbBox * getBBox();

    ///
    /// Get the placement status of this instance.
    ///
    dbPlacementStatus getPlacementStatus();

    ///
    /// Set the placement status of this instance.
    ///
    void setPlacementStatus( dbPlacementStatus status );

    ///
    /// Get the eco state bits to be used when an ECO block is created
    ///
	bool getEcoCreate();
	bool getEcoDestroy();
	bool getEcoModify();

    ///
    /// Set the eco state bits to be used when an ECO block is created
    ///
	void setEcoCreate(bool v);
	void setEcoDestroy(bool v);
	void setEcoModify(bool v);
    
    ///
    /// Get the user-defined flag bit.
    ///
    bool getUserFlag1();

    ///
    /// Set the user-defined flag bit.
    ///
    void setUserFlag1();

    ///
    /// Clear the user-defined flag bit.
    ///
    void clearUserFlag1();

    ///
    /// Get the user-defined flag bit.
    ///
    bool getUserFlag2();

    ///
    /// Set the user-defined flag bit.
    ///
    void setUserFlag2();

    ///
    /// Clear the user-defined flag bit.
    ///
    void clearUserFlag2();

    ///
    /// Get the user-defined flag bit.
    ///
    bool getUserFlag3();

    ///
    /// Set the user-defined flag bit.
    ///
    void setUserFlag3();

    ///
    /// Clear the user-defined flag bit.
    ///
    void clearUserFlag3();

    ///
    /// Set/Reset the size-only flag
    ///
    void setSizeOnly( bool v );

    ///
    /// Returns true if the size-only flag is set.
    ///
    bool isSizeOnly();
    
    ///
    /// Set/Reset the don't-touch flag
    ///
    void setDoNotTouch( bool v );
    
    ///
    /// Returns true if the don't-touch flag is set.
    ///
    bool isDoNotTouch();

    ///
    /// Set/Reset the don't-size flag
    ///
    void setDoNotSize( bool v );

    ///
    /// Returns true if the don't-size flag is set.
    ///
    bool isDoNotSize();

    ///
    /// Get the block of this instance.
    ///
    dbBlock * getBlock();

    ///
    /// Get the Master of this instance.
    ///
    dbMaster * getMaster();

    ///
    /// Get the instance-terminals of this instance.
    ///
    dbSet<dbITerm> getITerms();

    ///
    /// Get the first output terminal of this instance.
    ///
    dbITerm *getFirstOutput();

    ///
    /// Get the region this instance belongs to. Returns NULL if instance has no assigned region.
    ///
    dbRegion * getRegion();

    ///
    /// Find the iterm of the given terminal name.
    /// Returns NULL if no terminal was found.
    ///
    dbITerm * findITerm( const char * terminal_name );

    ///
    /// Find the iterm of the given terminal name given the master term order
    ///
	dbITerm * getITerm( uint mterm_order_id );
	dbITerm * getClockedTerm();
	dbITerm * getOutputTerm();

    ///
    /// Get the all the instances connected to the net of each iterm of this instance.
    /// Only traverse nets of the specified SigType. Default is dbSigType::SIGNAL.
    ///
    void getConnectivity( std::vector<dbInst *> & neighbors, dbSigType::Value value = dbSigType::SIGNAL );

    ///
    /// Bind the hierarchical (child) block to this instance.
    ///
    /// This method creates connectivity across the hierarchy.
    ///
    ///     (block)
    ///     +------------------------------------------------------------------+
    ///     |                                                                  |
    ///     |                                                                  |
    ///     |                                                                  |
    ///     |               (child-block / instance)                           |
    ///     |               +----------------------------------+               |
    ///     |               |                                  |               |
    ///     |B             I|B                                 |               |
    ///     |o.............o|o..........                       |               |
    ///     |  (net in      |  (net in child-block)            |               |
    ///     |   top-block)  |                                  |               |
    ///     |               |                                  |               |
    ///     |               |                                  |               |
    ///     |               |                                  |               |
    ///     |               |                                  |               |
    ///     |               +----------------------------------+               |
    ///     |                                                                  |
    ///     |                                                                  |
    ///     +------------------------------------------------------------------+
    ///
    /// B = dbBterm
    /// I = dbIterm
    ///
    /// This method will return false under the following situations:
    ///
    ///    1) This instance is already bound to a block.
    ///    2) This block cannot already be bound to an instance.
    ///    2) The block must be a direct child of the instance block.
    ///    3) block bterms must map 1-to-1 (by name) to the master mterms.
    ///
    bool bindBlock( dbBlock * block, bool force=false );

    ///
    /// Unbind the hierarchical (child) block from this instance.
    /// Does nothing if this instance has no child block.
    ///
    void unbindBlock();


    //
    // reset _hierachy to 0; not fully understood and tested!!!
    //
	bool resetHierarchy( bool verbose);
    ///
    /// Get the hierarchical (child) block bound to this instance.
    /// Returns NULL if this instance has no child.
    ///
    dbBlock * getChild();

    ///
    /// Get the parent instance of this instance.
    /// Returns NULL if this instance has no parent.
    ///
    dbInst * getParent();

    ///
    /// Get the children of this instance.
    ///
    dbSet<dbInst> getChildren();

    ///
    /// Returns true if this instance has hierarchy.
    ///
    bool isHierarchical();

    ///
    /// Returns a halo assigned to this instance.
    /// Returns NULL if this instance has no halo.
    ///
    dbBox * getHalo();

    ///
    /// Get the weight assigned to this instance.
    /// (Default: 1)
    ///
    int getWeight();

    ///
    /// Set the weight assigned of this instance.
    ///
    void setWeight( int weight );

    ///
    /// Get the source assigned to this instance.
    ///
    dbSourceType getSourceType();
    
    ///
    /// Set the source assigned of this instance.
    ///
    void setSourceType( dbSourceType type );

    ///
    /// Get the iterm that represents this mterm
    ///
    dbITerm * getITerm( dbMTerm * mterm );

    ///
    /// Swap the master of this instance.
    ///
    /// Returns true if the operations succeeds.
    /// NOTE: If this instance is bound to a block hierarchy, the master cannot be swapped.
    ///
    /// This method invalidates any existing dbSet<dbITerm>::iterator.
    ///
    bool swapMaster( dbMaster * master );

    ///
    /// Invalidate timing of remote nodes
    ///
    void ecoInvalidateTiming();

    ///
    /// Level of instance; if negative belongs to Primary Input Logic cone, 0 invalid.
    ///
	int getLevel();
	
    ///
    /// Set ;evel of instance; if fromPI true, logic cone is connected to Primiary inputs
    ///
	void setLevel(uint v, bool fromPI);

    ///
    /// Create a new instance.
    /// Returns NULL if an instance with this name already exists.
    /// Returns NULL if the master is not FROZEN.
    ///
    static dbInst * create( dbBlock * block, dbMaster * master, const char * name );

    ///
    /// Create a new instance within the specified region.
    /// Returns NULL if an instance with this name already exists.
    /// Returns NULL if the master is not FROZEN.
    ///
    static dbInst * create( dbBlock * block, dbMaster * master, const char * name, dbRegion * region );

    ///
    /// Delete the instance from the block.
    ///
    static void destroy( dbInst * inst );

    ///
    /// Delete the net from the block.
    ///
    static dbSet<dbInst>::iterator destroy( dbSet<dbInst>::iterator & itr );

    ///
    /// Translate a database-id back to a pointer.
    ///
    static dbInst * getInst( dbBlock * block, uint oid );

    ///
    /// Translate a valid database-id back to a pointer.
    ///
    static dbInst * getValidInst( dbBlock * block, uint oid );
};

///////////////////////////////////////////////////////////////////////////////
///
/// A dbITerm (instance-terminal) is the element that represents a connection
/// to a master-terminal of an instance.
///
///////////////////////////////////////////////////////////////////////////////
class dbITerm : public dbObject
{
  public:
	  
	void print(FILE *fp=NULL, const char *trail="");

    ///
    /// Get the instance of this instance-terminal.
    ///
    dbInst * getInst();

    ///
    /// Get the net of this instance-terminal.
    /// Returns NULL if this instance-terminal has NOT been connected
    /// to a net.
    ///
    dbNet * getNet();

    /// 
    /// Get the master-terminal that this instance-terminal is representing.
    /// 
    dbMTerm * getMTerm();
    
    ///
    /// Get the block this instance-terminal belongs too.
    ///
    dbBlock * getBlock();

    ///
    /// Get the signal type of this instance-terminal.
    ///
    dbSigType getSigType();

    ///
    /// Get the IO direction of this instance-terminal.
    ///
    dbIoType getIoType();

    ///
    /// True is iterm is input of signal type; if io false INOUT is not considered iput
    ///
	bool isInputSignal(bool io=true);

     ///
    /// True is iterm is output of signal type; if io false INOUT is not considered iput
    ///
	bool isOutputSignal(bool io=true);

	///
    /// Mark this dbITerm as spef. v should 1 or 0
    ///
    void setSpef(uint v);

    ///
    /// Return true if this dbITerm flag spef is set to 1
    ///
    bool isSpef();

    ///
    /// set ext id
    ///
	void setExtId(uint v);

    ///
    /// get ext id
    ///
	uint getExtId();

    ///
    /// Returns true if this dbITerm is marked as special. Special nets/iterms are declared
    /// in the SPECIAL NETS section of a DEF file.
    ///
    bool isSpecial();

    ///
    /// Mark this dbITerm as special.
    ///
    void setSpecial();

    ///
    /// Unmark this dbITerm as special.
    ///
    void clearSpecial();

    ///
    /// Set clocked of this instance-terminal.
    ///
	void setClocked(bool v);

    ///
    /// get clocked of this instance-terminal.
    ///
	bool isClocked();

    ///
    /// Set mark of this instance-terminal.
    ///
	void setMark(uint v);

    ///
    /// get mark of this instance-terminal.
    ///
	bool isSetMark();

    ///
    /// Returns true if this dbITerm has been marked as physically connected.
    ///
    bool isConnected();
    
    ///
    /// Mark this dbITerm as physically connected.
    ///
    void setConnected();

    ///
    /// Unmark this dbITerm as physically connected.
    ///
    void clearConnected();


    ///
    /// Get the hierarchical child bterm of this iterm.
    /// Returns NULL if there is no child bterm.
    ///
    ///
    ///     (top-block)
    ///     +------------------------------------------------------------------+
    ///     |                                                                  |
    ///     |                                                                  |
    ///     |                                                                  |
    ///     |               (child-block / instance)                           |
    ///     |               +----------------------------------+               |
    ///     |               |                                  |               |
    ///     |B             I|B                                 |               |
    ///     |o.............o|o..........                       |               |
    ///     |  (net in      |  (net in child-block)            |               |
    ///     |   top-block)  |                                  |               |
    ///     |               |                                  |               |
    ///     |               |                                  |               |
    ///     |               |                                  |               |
    ///     |               |                                  |               |
    ///     |               +----------------------------------+               |
    ///     |                                                                  |
    ///     |                                                                  |
    ///     +------------------------------------------------------------------+
    ///
    ///
    /// B = dbBterm
    /// I = dbIterm
    ///
    dbBTerm * getBTerm();

    ///
    /// Set the tmg-tmp-D flag. - payam
    ///
    void setTmgTmpD( bool v );

    ///
    /// returns true if the tmg-tmp-D flag is set. - payam
    ///
    bool isSetTmgTmpD();

    ///
    /// Set the tmg-tmp-C flag. - payam
    ///
    void setTmgTmpC( bool v );

    ///
    /// returns true if the tmg-tmp-C flag is set. - payam
    ///
    bool isSetTmgTmpC();

    ///
    /// Set the tmg-tmp-B flag.
    ///
    void setTmgTmpB( bool v );

    ///
    /// returns true if the tmg-tmp-B flag is set.
    ///
    bool isSetTmgTmpB();

    ///
    /// Set temporary timing flag.
    ///
    void setTmgTmpA( bool value );

    ///
    ///  Returns true if flag is set.
    ///
    bool isSetTmgTmpA();

    ///
    /// Initialize timing.
    ///
    ///    slewRise[MIN] = infinity
    ///    slewFall[MIN] = infinity
    ///    slackRise[MIN] = infinity
    ///    slackFall[MIN] = infinity
    ///    slewRise[MAX] = -infinity
    ///    slewFall[MAX] = -infinity
    ///    slackRise[MAX] = infinity
    ///    slackFall[MAX] = infinity
    ///
    ///  where:
    ///
    ///    infinity = INT_MAX
    ///    -infinity = INT_MIN
    ///
    void initTiming();

    // payam: changed int to float
    float getSlewRise( dbTimingMode mode );
    float getSlewFall( dbTimingMode mode );
    float getSlackRise( dbTimingMode mode );
    float getSlackFall( dbTimingMode mode );
    float getSlewRise( dbTimingMode mode, uint scenario );
    float getSlewFall( dbTimingMode mode, uint scenario );
    float getSlackRise( dbTimingMode mode, uint scenario );
    float getSlackFall( dbTimingMode mode, uint scenario );
    void setSlewRise( dbTimingMode mode, float value, uint scenario=0 );
    void setSlewFall( dbTimingMode mode, float value, uint scenario=0 );
    void setSlackRise( dbTimingMode mode, float value, uint scenario=0 );
    void setSlackFall( dbTimingMode mode, float value, uint scenario=0 );
    bool getWorstSlack(float wns[2], float tns[2], uint scenario[2]);

    // Write the current timing information to the timing journal.
    //cc void ecoTiming();
    
    ///
    /// Create a connection between a instance and a net using a specific
    /// master-terminal. The dbITerm representing the connection is returned.
    /// Returns NULL if that specific master-terminal of that instance is
    /// already connected to a net.
    ///
    static dbITerm * connect( dbInst * inst, dbNet * net, dbMTerm * terminal );

    ///
    /// Connect this iterm to this net. 
    ///
    static void connect( dbITerm * iterm, dbNet * net );

    ///
    /// Disconnect this iterm from the net it is connected to.
    ///
    static void disconnect( dbITerm * iterm );

    ///
    /// Disconnect this iterm from the net it is connected to.
    ///
    static dbSet<dbITerm>::iterator disconnect( dbSet<dbITerm>::iterator & itr );

    ///
    /// Get the average of the centers for the iterm shapes
    /// Returns false if iterm has no shapes
    ///
    bool getAvgXY( int *x, int *y);

    ///
    /// Translate a database-id back to a pointer.
    ///
    static dbITerm * getITerm( dbBlock * block, uint oid );
};

///////////////////////////////////////////////////////////////////////////////
///
/// A dbVia is the element that represents a block specific via definition.
/// These vias are typically generated to be used in power routing.
///
///////////////////////////////////////////////////////////////////////////////
class dbVia : public dbObject
{
  public:
    ///
    /// Get the via name.
    ///
    dbString getName();

    ///
    /// Get the via name.
    ///
    const char *getConstName();

    ///
    /// Get the pattern value of this via.
    /// Returns and empty ("") string if a pattern has not been set.
    ///
    dbString getPattern();

    ///
    /// Set the pattern value of this via.
    /// The pattern is ignored if the pattern is already set on this via
    ///
    void setPattern( const char * pattern );

    ///
    /// Set generate rule that was used to genreate this via.
    ///
    void setViaGenerateRule( dbTechViaGenerateRule * rule );

    ///
    /// Get the generate rule that was used to genreate this via.
    ///
    dbTechViaGenerateRule * getViaGenerateRule();

    ///
    /// Returns true if this via has params.
    ///
    bool hasParams();
    
    ///
    /// Set via params to generate this via. This method will create the shapes
    /// of this via. All previous shapes are destroyed.
    ///
    void setViaParams( const dbViaParams & params );

    ///
    /// Get the via params used to generate this via.
    ///
    void getViaParams( dbViaParams & params );

    ///
    /// Get the block this via belongs too.
    ///
    dbBlock * getBlock();

    ///
    /// Get the bbox of this via
    /// Returns NULL if this via has no shapes.
    ///
    dbBox * getBBox();

    ///
    /// Get the boxes of this via.
    ///
    dbSet<dbBox> getBoxes();

    ///
    /// Get the upper-most layer of this via reaches
    /// Returns NULL if this via has no shapes.
    ///
    dbTechLayer * getTopLayer();

    ///
    /// Get the lower-most layer of this via reaches
    /// Returns NULL if this via has no shapes.
    ///
    dbTechLayer * getBottomLayer();

    ///
    /// Returns true if this via is a rotated version of a block or tech via.
    ///
    /// Vias in DEF can now be rotated.
    ///
    bool isViaRotated();

    ///
    /// Get the rotation of this via.
    ///
    dbOrientType getOrient();

    //
    // Get the tech-via that this roated via represents.
    /// Returns NULL if this via does not represent a tech via
    //
    dbTechVia * getTechVia();

    //
    // Get the block-via that this roated via represents.
    /// Returns NULL if this via does not represent a block via
    //
    dbVia * getBlockVia();

    ///
    /// Create a block specific via.
    /// Returns NULL if a via with this name already exists.
    ///
    static dbVia * create( dbBlock * block, const char * name );

    ///
    /// Created a rotated version of the specified block-via.
    /// Returns NULL if a via with this name already exists.
    ///
    static dbVia * create( dbBlock * block, const char * name, dbVia * via, dbOrientType type );

    ///
    /// Created a rotated version of the specified tech-via.
    /// Returns NULL if a via with this name already exists.
    ///
    static dbVia * create( dbBlock * block, const char * name, dbTechVia * via, dbOrientType type );

    /// Copy all the VIA's from the src-block to the dst-block.
    /// Returns false the copy failed.
    static bool copy( dbBlock * dst, dbBlock * src );

    /// Copy the VIA to the dst-block.
    static dbVia * copy( dbBlock * dst, dbVia * src );

    ///
    /// Translate a database-id back to a pointer.
    ///
    static dbVia * getVia( dbBlock * block, uint oid );
};

///////////////////////////////////////////////////////////////////////////////
///
///  A dbWire is the element used to represent net wires. A wire object
///  represents a sequence of from-to paths, which may form either
///  a disjoint or non-disjoint set of paths. A wire is encoded using the
///  dbWireEncoder class, see "dbWireCodec.h". A wire is decoded using
///  the dbWireDecoder class.
///
///////////////////////////////////////////////////////////////////////////////
class dbWire : public dbObject
{
  public:
    ///
    /// Get the block this wire belongs too.
    ///
    dbBlock * getBlock();

    ///
    /// Get the net this wire is attached too.
    /// Returns NULL if this wire is not attached to a net.
    ///
    dbNet * getNet();

    ///
    /// Append the wire to this wire. This operation will fail if the wire
    /// is from another block and the wire contains bterms or iterms.
    ///
    void append( dbWire * wire, bool singleSegmentWire=false );

    ///
    /// Move segements of this wire to wires of other nets
    ///
    void shuffleWireSeg (dbNet **newNets, dbRSeg **new_rsegs);


    ///
    /// Get junction id associated with the term
    ///
    uint getTermJid (int termid);

    ///
    /// check if this wire equals the target wire
    /// return value = 0: equal
    ///                x: not equal
    ///                1x: wire seg after junction not equal
    ///
    uint equal(dbWire *target);

    // void match(dbWire *target);

    ///
    /// Get the shape of this shape-id.
    /// PRECONDITION: shape-id is a segment or via 
    ///
    void getShape( int shape_id, dbShape & shape );

    ///
    /// Get the segment of this segment-id
    ///
    /// PRECONDITION: segment_id is a segment
    ///
    void getSegment( int segment_id, dbShape & shape );

    ///
    /// Get the segment of this segment-id, where layer is the layer of the segment
    ///
    /// PRECONDITION: segment_id is a segment
    ///
    void getSegment( int segment_id, dbTechLayer * layer, dbShape & shape );

    ///
    /// Get the previous via of this shape-id.
    /// returns false if the previous shape is not a via.
    ///
    bool getPrevVia( int shape_id, dbShape & shape );

    ///
    /// Get the via that follows of this shape-id.
    /// returns false if the next shape is not a via.
    ///
    bool getNextVia( int shape_id, dbShape & shape );

    ///
    /// Get the via boxes of this via_shape-id.
    ///
    /// returns false if this shape_id is not a via.
    ///
    bool getViaBoxes( int via_shape_id, std::vector<dbShape> & boxes );

    ///
    /// Returns true if this wire is a global-wire
    ///
    bool isGlobalWire();

    ///
    /// Get the bounding box of this wire
    ///
    bool getBBox( adsRect & r );

    ///
    /// Get the total path length contained in this wire.
    ///
    uint64 getLength();

    ///
    /// Get the number of entries contained in this wire.
    ///
    uint length();

    ///
    /// Get the count of wire segments contained in this wire.
    ///
    uint count();
    ///
    /// Get junction coordinate.
    ///
    void getCoord( int jid, int & x, int & y );

    ///
    /// Get junction property
    ///
    bool getProperty( int jid, int & property );

    ///
    /// Set junction property
    ///
    bool setProperty( int jid, int property );

    ///
    /// Set one data entry
    ///
    int getData(int n);

    ///
    /// Set one opcode entry
    ///
    unsigned char getOpcode(int n);

    ///
    /// Print opcodes and data of this wire
    ///
    void printWire();
    void printWire(FILE *fp, int fid, int tid);

    ///
    /// Attach this wire to a net.
    ///   1) If the net is already attached to another wire, the other wire will
    ///      be destroyed.
    ///   2) If this wire is already attached to another net, thie wire will be detachd from
    //       the other net.
    ///
    void attach( dbNet * net );

    ///
    /// Detach this wire from a net.
    ///
    void detach();

    ///
    /// Call this method to notify the eco engine that the wire has been changed
    ///
    void ecoUpdate();

    ///
    /// Copy the src wire to the desintation wire.
    ///
    ///    removeITermBTerms - if true, then any iterms or bterms referenced in the copied wire will be removed.
    ///    copyVias - if true, then any reference vias that do not exists in the dst-block are copied to the src-block.
    ///                        
    ///
    static void copy( dbWire * dst, dbWire * src, bool removeITermsBTerms = true, bool copyVias = true );

    ///
    /// Copy the src wire to the desintation wire. Filter vias and segments that do not intersect bbox.
    ///
    ///    removeITermBTerms - if true, then any iterms or bterms referenced in the copied wire will be removed.
    ///    copyVias - if true, then any reference vias that do not exists in the dst-block are copied to the src-block.
    ///                        
    ///
    static void copy( dbWire * dst, dbWire * src, const adsRect & bbox, bool removeITermsBTerms = true, bool copyVias = true );

    ///
    /// Create a wire.
    /// Returns NULL if this net already has the specified wire dbWire.
    ///
    static dbWire * create( dbNet * net, bool global_wire = false );

    ///
    /// Create an unattached wire.
    ///
    static dbWire * create( dbBlock * block, bool global_wire = false );

    ///
    /// Translate a database-id back to a pointer.
    ///
    static dbWire * getWire( dbBlock * block, uint oid );

    ///
    /// Destroy a wire.
    ///
    static void destroy( dbWire * wire );

  private:
    void addOneSeg(unsigned char op, int value, uint jj, int *did, dbRSeg **new_rsegs);
    void addOneSeg(unsigned char op, int value);
    void donateWireSeg (dbWire *w1, dbRSeg **new_rsegs);

    friend class dbNet;
};

///////////////////////////////////////////////////////////////////////////////
///
///  A dbSWire is the element used to represent special-net wires.
/// 
///////////////////////////////////////////////////////////////////////////////
class dbSWire : public dbObject
{
  public:
    ///
    /// Get the block this wire belongs too.
    ///
    dbBlock * getBlock();

    ///
    /// Get the net this wire is attached too.
    ///
    dbNet * getNet();

    ///
    /// Return the wire-type.
    ///
    dbWireType getWireType();

    ///
    /// Returns the shield net if the wire-type is dbWireType::SHIELD
    ///
    dbNet * getShield();
    
    ///
    /// Get the wires of this special wire.
    ///
    dbSet<dbSBox> getWires();

    ///
    /// Create a new special-wire.
    ///
    static dbSWire * create( dbNet * net, dbWireType type, dbNet * shield = NULL );

    ///
    /// Delete this wire
    ///
    static void destroy( dbSWire * swire );

    ///
    /// Delete this wire
    ///
    static dbSet<dbSWire>::iterator destroy( dbSet<dbSWire>::iterator & itr );

    ///
    /// Translate a database-id back to a pointer.
    ///
    static dbSWire * getSWire( dbBlock * block, uint oid );
};

///////////////////////////////////////////////////////////////////////////////
///
/// A dbGCellGrid is the element that represents a block specific grid definition.
///
///////////////////////////////////////////////////////////////////////////////
class dbGCellGrid : public dbObject
{
  public:
    ///
    /// Get the "X" grid coordinates
    ///
    void getGridX( std::vector<int> & x_grid );

    ///
    /// Get the "Y" grid coordinates
    ///
    void getGridY( std::vector<int> & y_grid );
    
    ///
    /// Get the block this grid belongs too.
    ///
    dbBlock * getBlock();

    ///
    /// Add a "X" grid pattern.
    ///
    void addGridPatternX( int origin_x, int line_count, int step );

    ///
    /// Add a "Y" grid pattern.
    ///
    void addGridPatternY( int origin_y, int line_count, int step );

    ///
    /// Get the number of "X" grid patterns.
    ///
    int getNumGridPatternsX();

    ///
    /// Get the number of "Y" grid patterns.
    ///
    int getNumGridPatternsY();
    
    ///
    /// Get the "ith" "X" grid pattern.
    ///
    void getGridPatternX( int i, int & origin_x, int & line_count, int & step );

    ///
    /// Get the "ith" "Y" grid pattern.
    ///
    void getGridPatternY( int i, int & origin_y, int & line_count, int & step );
    ///
    /// Create an empty GCell grid.
    /// Returns NULL if a grid already exists.
    ///
    static dbGCellGrid * create( dbBlock * block );

    ///
    /// Translate a database-id back to a pointer.
    ///
    static dbGCellGrid * getGCellGrid( dbBlock * block, uint oid );
};

///////////////////////////////////////////////////////////////////////////////
///
/// A dbTrackGrid is the element that represents a block tracks. 
///
///////////////////////////////////////////////////////////////////////////////
class dbTrackGrid : public dbObject
{
  public:
    ///
    /// Get the layer for this track-grid.
    ///
    dbTechLayer * getTechLayer();

    ///
    /// Get the "X" track coordinates for a this tech-layer.
    ///
    void getGridX( std::vector<int> & x_grid );

    ///
    /// Get the "Y" track coordinates for a this tech-layer.
    ///
    void getGridY( std::vector<int> & y_grid );
    
    ///
    /// Get the block this grid belongs too.
    ///
    dbBlock * getBlock();

    ///
    /// Add a "X" grid pattern.
    ///
    void addGridPatternX( int origin_x, int line_count, int step );

    ///
    /// Add a "Y" grid pattern.
    ///
    void addGridPatternY( int origin_y, int line_count, int step );

    ///
    /// Get the number of "X" grid patterns.
    ///
    int getNumGridPatternsX();

    ///
    /// Get the number of "Y" grid patterns.
    ///
    int getNumGridPatternsY();
    
    ///
    /// Get the "ith" "X" grid pattern.
    ///
    void getGridPatternX( int i, int & origin_x, int & line_count, int & step );

    ///
    /// Get the "ith" "Y" grid pattern.
    ///
    void getGridPatternY( int i, int & origin_y, int & line_count, int & step );
    ///
    /// Create an empty Track grid.
    /// Returns NULL if a the grid for this layer already exists.
    ///
    static dbTrackGrid * create( dbBlock * block, dbTechLayer * layer );

    ///
    /// Translate a database-id back to a pointer.
    ///
    static dbTrackGrid * getTrackGrid( dbBlock * block, uint oid );
};

///////////////////////////////////////////////////////////////////////////////
///
/// A dbObstruction is the element that represents a routing
/// obstruction in a block.
///
///////////////////////////////////////////////////////////////////////////////
class dbObstruction : public dbObject
{
  public:
    ///
    /// Get the bbox of this obstruction.
    ///
    dbBox * getBBox();
    
    ///
    /// Get the instance associated with this obstruction.
    /// Returns NULL of no instance was associated with this obstruction
    ///
    dbInst * getInstance();

    ///
    /// Declare this obstruction to be a "slot" obstruction.
    ///
    void setSlotObstruction();

    ///
    /// Returns true if this obstruction is a "slot" obstruction.
    ///
    bool isSlotObstruction();

    ///
    /// Declare this obstruction to be a "fill" obstruction.
    ///
    void setFillObstruction();

    ///
    /// Returns true if this obstruction is a "fill" obstruction.
    ///
    bool isFillObstruction();
    
    ///
    /// Declare this obstruction to have been pushed into this block. 
    ///
    void setPushedDown();

    ///
    /// Returns true if this obstruction was pushed into this block.
    ///
    bool isPushedDown();
     ///
    /// Returns true if this bpin has an effective-width rule.
    ///
    bool hasEffectiveWidth();

    ///
    /// Set the effective width rule.
    ///
    void setEffectiveWidth( int w );

    ///
    /// Return the effective width rule.
    ///
    int getEffectiveWidth();

    ///
    /// Returns true if this bpin has an min-spacing rule.
    ///
    bool hasMinSpacing();

    ///
    /// Set the min spacing rule.
    ///
    void setMinSpacing( int w );

    ///
    /// Return the min spacing rule.
    ///
    int getMinSpacing();
    
    ///
    /// Get the block this obstruction belongs too.
    ///
    dbBlock * getBlock();

    ///
    /// Create a routing obstruction.
    ///
    static dbObstruction * create( dbBlock * block, dbTechLayer * layer,
                                   int x1, int y1, int x2, int y2, dbInst * inst = NULL );

    ///
    /// Translate a database-id back to a pointer.
    ///
    static dbObstruction * getObstruction( dbBlock * block, uint oid );
};

///////////////////////////////////////////////////////////////////////////////
///
/// A dbBlockage is the element that represents a placement blockage in a block.
///
///////////////////////////////////////////////////////////////////////////////
class dbBlockage : public dbObject
{
  public:
    ///
    /// Get the bbox of this obstruction.
    ///
    dbBox * getBBox();
    
    ///
    /// Get the instance associated with this blockage.
    /// Returns NULL of no instance was associated with this blockage
    ///
    dbInst * getInstance();
    
    ///
    /// Declare this blockage to have been pushed into this block. 
    ///
    void setPushedDown();

    ///
    /// Returns true if this blockage was pushed into this block.
    ///
    bool isPushedDown();
    
    ///
    /// Get the block this obstruction belongs too.
    ///
    dbBlock * getBlock();

    ///
    /// Create a placement obstruction.
    ///
    static dbBlockage * create( dbBlock * block, int x1, int y1, int x2, int y2, dbInst * inst = NULL);

    ///
    /// Translate a database-id back to a pointer.
    ///
    static dbBlockage * getBlockage( dbBlock * block, uint oid );
};

///////////////////////////////////////////////////////////////////////////////
///
/// A RCSeg is the element that represents an RC element in a RC network.
///
/// The segment junction nodes are denoted "source" and "target". However,
/// this namimg is just a convention and there is no implied directional
/// meaning.
///
///////////////////////////////////////////////////////////////////////////////
class dbCapNode : public dbObject
{
  protected:

	friend class dbRSeg;
	friend class extMain;
	friend class extSpef;
	friend class te_tile;
	friend class tilext;
	friend class dbJournal;

    ///
    /// Get the capacitance of this capNode segment for this process corner. Returns value in
    /// femto-fards.
    ///
    void getCapTable(double *cap);

  public:
    ///
    /// Add the capacitances of other capnode to this capnode
    ///
    void addCapnCapacitance( dbCapNode *other );

    ///
    /// Add the gndCap of this capnode to *gndcap and *totalcap
    ///
    void addGndCap(double *gndcap, double *totalcap);

    /// Add the gndCap to *gndcap and *totalcap, ccCap to *totalcap
    ///^M
    void addGndTotalCap(double *gndcap, double *totalcap, double MillerMult);

    ///
    /// Get the gndCap of this capnode to *gndcap and *totalcap
    ///
    void getGndCap(double *gndcap, double *totalcap);

    ///^M
    /// Get the gndCap to *gndcap and *totalcap, ccCap to *totalcap
    ///^M
    void getGndTotalCap(double *gndcap, double *totalcap, double MillerMult);

    ///
    /// Add the caps of all corners of CC's from this capnode to *totalcap
    ///
    void accAllCcCap(double *totalcap, double MillerMult);

    ///
    /// Set the capacitance of this CapNode segment for this process corner. Value must be in
    /// femto-fards.
    ///
    void setCapacitance( double cap, int corner = 0 );

    ///
    /// Add the capacitance of this CapNode segment for this process corner. Value must be in
    /// femto-fards.
    ///
	void addCapacitance( double cap, int corner = 0);

    ///
    /// add cc capacitance to gnd capacitance of this capNode
    /// 
    bool groundCC(float gndFactor);

    /// 
    ///  Adjust the capacitance of this capNode for this process corner
    ///
    void adjustCapacitance(float factor, uint corner);

    ///
    ///  Adjust the capacitance of this capNode
    ///
    void adjustCapacitance(float factor);

    ///
    /// check if having CC's with capacitnce greater than ccThreshHold
    ///
    bool needAdjustCC(double ccThreshHold);

    ///
    /// adjust CC's of this capNode
    ///
    void adjustCC(uint adjOrder, float adjFactor, std::vector<dbCCSeg *> & adjustedCC, std::vector<dbNet *> & halonets);

    ///
    /// Get the capacitance of this capNode segment for this process corner. Returns value in
    /// femto-fards.
    ///
    double getCapacitance(uint corner = 0);

    ///
    /// Get the rc-network cap node.
    ///
    uint getNode();

    ///
    /// Get the shapeId of the cap node.
    ///
    uint getShapeId();

    ///
    /// Set the rc-network cap node.
    ///
    void setNode( uint nodeid );

    ///
    /// Get next cap node in same net
    ///
 	//  dbCapNode *getNext(dbBlock *block_);

    ///
    ///  is this node iterm/bterm/internal/branch/dangling/foreign .
    ///
	bool isName();
	bool isITerm();
	bool isBTerm();
	bool isInternal();
	bool isBranch();
	bool isDangling();
	bool isForeign();
	bool isTreeNode(); // bterm, iterm, branch
	// bool isSourceNodeBterm();
	bool isSourceTerm(dbBlock *mblock=NULL);
	bool isInoutTerm(dbBlock *mblock=NULL);

    ///
    /// Returns the select flag value. This flag specified that the
    /// net has been select.
    ///
    bool isSelect();

    ///
    /// Set the select flag to the specified value.
    ///
    void setSelect( bool value );

	///
    ///  increase children cnt; capNode is a branch of the rooted tree.
    ///
	uint incrChildrenCnt();
	uint getChildrenCnt();
	void setChildrenCnt(uint cnt);

	///
    ///  set iterm/bterm/internal/branch/foreign flag of this cap node.
    ///
	void setNameFlag();
	void setBTermFlag();
	void setITermFlag();
	void setInternalFlag();
	void setBranchFlag();
	void setForeignFlag();

	///
    ///  reset iterm/bterm/internal/branch/foreign flag of this cap node.
    ///
	void resetNameFlag();
	void resetBTermFlag();
	void resetITermFlag();
	void resetInternalFlag();
	void resetBranchFlag();
	void resetForeignFlag();

    ///
    /// Get the sort index of this node
    ///
        uint getSortIndex();

    ///
    /// Set the sort index of this node
    ///
        void setSortIndex(uint idx);

    ///
    /// Get the coordinates of this node if iterm or bterm
    ///
        bool getTermCoords(int & x, int & y, dbBlock *mblock=NULL);

    ///
    /// Get the iterm of this node
    ///
        dbITerm *getITerm(dbBlock *mblock=NULL);

    ///
    /// Get the bterm of this node 
    ///
        dbBTerm *getBTerm(dbBlock *mblock=NULL);

    ///
    /// Set the _ycoord of this node
    ///
    /// void setCoordY(int y);

    ///
    /// Get the _ycoord of this node
    ///
    /// void getCoordY(int & y);

    ///
    /// print the ccsegs of the capn
    ///
    void printCC();

    ///
    /// check the ccsegs of the capn
    ///
    bool checkCC();

    ///
    /// Get the coupling caps bound to this node
    ///
    dbSet<dbCCSeg> getCCSegs();

    ///
    /// Net given the capNode id
    ///
	dbNet *getNet();

    ///
    /// set net
    ///
    void setNet( uint netid );

    ///
    /// set next
    ///
    void setNext( uint nextid );

    ///
    /// Create a new rc-segment
    /// The default values for each process corner is 0.0.
    ///
    static dbCapNode * create( dbNet * net, uint node, bool foreign );

    ///
    /// add a seg onto a net
    /// 
    void addToNet();
    
    ///
    /// Destroy a rc-segment.
    ///
    static void destroy( dbCapNode * seg, bool destroyCC=true );

    ///
    /// Destroy a rc-segment.
    ///
    static dbSet<dbCapNode>::iterator destroy( dbSet<dbCapNode>::iterator & itr );

    ///
    /// Translate a database-id back to a pointer.
    ///
    static dbCapNode * getCapNode( dbBlock * block, uint oid );

    // friend void test_eco();
};
///////////////////////////////////////////////////////////////////////////////
///
/// A RSeg is the element that represents an Res element in a Res network.
///
/// The segment junction nodes are denoted "source" and "target". However,
/// this namimg is just a convention and there is no implied directional
/// meaning.
///
///////////////////////////////////////////////////////////////////////////////
class dbRSeg : public dbObject
{
  public:

    ///
    /// Add the resistances of other rseg to this rseg
    ///
    void addRSegResistance( dbRSeg *other );

    ///
    /// Add the capacitances of other rseg to this rseg
    ///
    void addRSegCapacitance( dbRSeg *other );

    ///
    /// Get the resistance of this RC segment for this process corner. Returns value in
    /// ohms.
    ///
    double getResistance(int corner = 0);

    ///
    /// Get the resistance of this RC segment to *res
    ///
    void getAllRes(double *res);

    ///
    /// Add the resistance of this RC segment to *res
    ///
    void addAllRes(double *res);

    ///
    /// Get the gdn cap of this RC segment to *gndcap and *totalcap
    ///
    void getGndCap(double *gndcap, double *totalcap);

    ///
    /// Add the gdn cap of this RC segment to *gndcap and *totalcap
    ///
    void addGndCap(double *gndcap, double *totalcap);

    ///
    /// Get the gdn cap of this RC segment to *gndcap, total cap to *totalcap
    ///
    void getGndTotalCap(double *gndcap, double *totalcap, double MillerMult);

    ///
    /// Add the gdn cap of this RC segment to *gndcap, total cap to *totalcap
    ///
    void addGndTotalCap(double *gndcap, double *totalcap, double MillerMult);

    ///
    /// do merge rsegs
    ///
    // void mergeRCs(std::vector<dbRSeg *> & mrsegs);

    ///
    /// Adjust the capacitance of this RC segment for this process corner.
    ///
    void adjustCapacitance(float factor, uint corner);

    ///
    /// Adjust the capacitance of the src capNode of this RC segment for the process corner.
    ///
    void adjustSourceCapacitance(float factor, uint corner);

    ///
    /// Adjust the capacitance of this RC segment.
    ///
    void adjustCapacitance(float factor);

    ///
    /// Set the capacitance of this RC segment for this process corner. Value must in
    /// FF.
    ///
    void setCapacitance(double cap, int corner = 0);

	///
    /// Returns the _update_cap flag value. This flag specified that the
    /// rseg has been updated
    ///
	bool updatedCap();

    ///
    /// Get the capacitance of this RC segment for this process corner. Returns value in
    /// FF.
    ///
    double getCapacitance(int corner = 0);

    ///
    /// Get the capacitance of this RC segment for this process corner,
    /// plus coupling capacitance. Returns value in FF.
    ///
    double getSourceCapacitance(int corner = 0);

    ///
    /// Get the first capnode capacitance of this RC segment 
    /// for this process corner, if foreign,
    /// plus coupling capacitance. Returns value in FF.
    ///
    double getCapacitance(int corner, double MillerMult);

    ///
    /// Get the CC segs of this RC segment,
    ///
    void getCcSegs (std::vector<dbCCSeg *> & ccsegs);

    ///
    /// print the CC segs of this RC segment,
    ///
    void printCcSegs ();
    void printCC();
    bool checkCC();

    ///
    /// Get the capacitance table of this RC segment. value is in FF
    ///
    void getCapTable(double *cap);

    ///
    /// Set the resistance of this RC segment for this process corner. Value must be in
    /// ohms.
    ///
    void setResistance( double res, int corner = 0 );

    ///
    ///  Adjust the resistance of this RC segment for this process corner
    ///
    void adjustResistance(float factor, int corner);

    ///
    ///  Adjust the resistance of this RC segment
    ///
    void adjustResistance(float factor);

    ///
    /// Set the next rseg
    ///
    void setNext(uint next_id);

    ///
    /// Get the rc-network source node of this segment,
    ///
    uint getSourceNode();

    ///
    /// Get the rc-network source node of this segment,
    ///
    dbCapNode * getSourceCapNode();

    ///
    /// Set the rc-network source node of this segment,
    ///
    void setSourceNode( uint nodeid );

    ///
    /// Get the rc-network target node of this segment,
    ///
    uint getTargetNode();

    ///
    /// Get the rc-network target node of this segment,
    ///
    dbCapNode * getTargetCapNode();

    ///
    /// Set the rc-network target node of this segment,
    ///
    void setTargetNode( uint nodeid );

    ///
    /// Get shape-id of this RC-segment.
    ///
    uint getShapeId();

    ///
    /// Set coordinates of this RC-segment.
    ///
    void setCoords(int x, int y);

    ///
    /// Get coordinates of this RC-segment.
    ///
    void getCoords(int & x, int & y);

    ///
    /// Set shape-id of this RC-segment, and the target capNode if internal.
    ///
    void updateShapeId(uint nsid);

    ///
    /// check path direction
    ///
    bool pathLowToHigh();

    ///
    /// check if cap allocated
    ///
    bool allocatedCap();

    ///
    /// returns length and width.
    ///
	uint getLengthWidth(uint &w);

    ///
    /// add a seg onto a net
    /// 
    bool addToNet();

    ///
    /// Get the net of this RC-segment.
    ///
    dbNet * getNet();

    ///
    /// Create a new r-segment
    /// The default values for each process corner is 0.0.
    ///
    static dbRSeg * create( dbNet * net, int x, int y, uint path_dir, bool allocate_cap);
    
    ///
    /// Destroy a rc-segment.
    ///
    static void destroy( dbRSeg * seg );
    static void destroy( dbRSeg * seg, dbNet *net );

    ///
    /// simple destroy a disconnected rc-segment
    ///
    static void destroyS( dbRSeg * seg );

    ///
    /// Destroy a rc-segment.
    ///
    static dbSet<dbRSeg>::iterator destroy( dbSet<dbRSeg>::iterator & itr );

    ///
    /// Translate a database-id back to a pointer.
    ///
    static dbRSeg * getRSeg( dbBlock * block, uint oid );
};

///////////////////////////////////////////////////////////////////////////////
///
/// A CCSeg is the element that represents an coupling capacitance element
/// in a RC network.
///
/// The segment junction nodes are denoted "source" and "target". However,
/// this namimg is just a convention and there is no implied directional
/// meaning.
///
///////////////////////////////////////////////////////////////////////////////
class dbCCSeg : public dbObject
{
  public:
    ///
    /// Adjust the capacitance of this CC segment
    ///
    void adjustCapacitance(float factor);

    ///
    /// Adjust the capacitance of a corner this CC segment
    ///
    void adjustCapacitance(float factor, int corner);

    ///
    /// Get the capacitance of this CC segment for this process corner. Returns value in
    /// femto-fards.
    ///
    double getCapacitance(int corner = 0);

    ///
    /// Set the capacitance of this CC segment for this process corner. Value must be in
    /// femto-fards.
    ///
    void setCapacitance( double cap, int corner = 0 );

    ///
    /// Add the capacitance of this CC segment for this process corner. Value must be in
    /// femto-fards.
    ///
    void addCapacitance( double cap, int corner = 0 );

    ///
    /// Add the capacitance of all corners of this CC segment to *ttcap
    ///
    void accAllCcCap(double *ttcap, double MillerMult);

    ///
    /// Get the capacitance of all corners of this CC segment to *ttcap
    ///
    void getAllCcCap(double *ttcap);

    ///
    /// Set the capacitance of all corners of this CC segment by *ttcap
    ///
    void setAllCcCap(double *ttcap);

    dbCapNode * getSourceCapNode();
    dbCapNode * getTargetCapNode();

    ///
    /// Add capacitance of other CC segment to this CC segment
    ///
    void addCcCapacitance( dbCCSeg *other );

    ///
    /// Change this CC segement's capNode orig to capNode new
    ///
    void swapCapnode(dbCapNode *orig, dbCapNode *newn);
 
    /// 
    /// Get the capNode of this CC segment, other than oneCap
    ///
    dbCapNode *getTheOtherCapn(dbCapNode *oneCap, uint & cid);

    /// Get the rc-network source node of this segment,
    ///
    uint getSourceNodeNum();

    ///
    /// Set the rc-network source node of this segment,
    ///
    //void setSourceNode( uint nodeid );

    ///
    /// Get the rc-network target node of this segment,
    ///
    uint getTargetNodeNum();

    ///
    /// Set the rc-network target node of this segment,
    ///
    //void setTargetNode( uint nodeid );

    ///
    /// Get the source net of this CC-segment.
    ///
    dbNet * getSourceNet();

    ///
    /// Get the target net of this CC-segment.
    ///
    dbNet * getTargetNet();

    ///
    /// Get the infile cnt of this CC-segment.
    ///
    uint getInfileCnt();

    ///
    /// Increment the infile cnt of this CC-segment.
    ///
    void incrInfileCnt();

    ///
    /// Returns the mark flag value. This flag specified that the
    /// CC seg has been marked.
    ///
    bool isMarked();

    ///
    /// Set the mark flag to the specified value.
    ///
    void setMark( bool value );

    ///
    /// print CC's of capn
    ///
    void printCapnCC(uint capn);

    ///
    /// check CC's of capn
    ///
    bool checkCapnCC(uint capn);

    ///
    /// unlink cc from capn
    ///
    void unLink_cc_seg(dbCapNode *capn);

    ///
    /// link cc to capn
    ///
    void Link_cc_seg(dbCapNode *capn, uint cseq);

    ///
    /// relink _cc_tgt_segs of a net
    /// Used in re-reading the CC part of a spef file.
    ///


    // static dbCCSeg * relinkTgtCC (dbNet *net_, dbCCSeg *pseg_, uint src_cap_node, uint tgt_cap_node);

    ///
    /// Returns NULL if not found
    ///
	static dbCCSeg *findCC( dbCapNode * nodeA, dbCapNode * nodeB );

    ///
    /// Create a new cc-segment.
    /// The default values for each process corner is 0.0.
    ///
    static dbCCSeg * create( dbCapNode * nodeA, dbCapNode * nodeB, bool mergeParallel=false);

    ///
    /// Destroy a cc-segment.
    ///
    static void destroy( dbCCSeg * seg );

    ///
    /// simple destroy disconnected cc-segment
    ///
    static void destroyS( dbCCSeg * seg );

    ///
    /// Destroy a cc-segment.
    ///
    static dbSet<dbCCSeg>::iterator destroy( dbSet<dbCCSeg>::iterator & itr );

    ///
    /// Translate a database-id back to a pointer.
    ///
    static dbCCSeg * getCCSeg( dbBlock * block, uint oid );

    ///
    /// disconnect a cc-segment
    ///
    static void disconnect( dbCCSeg *tcc_);

    ///
    /// connect a cc-segment
    ///
    static void connect( dbCCSeg *tcc_);

};

///////////////////////////////////////////////////////////////////////////////
///
/// A Row is the element that represents placement sites.
///
///////////////////////////////////////////////////////////////////////////////
class dbRow : public dbObject
{
  public:
    ///
    /// Get the row name.
    ///
    dbString getName();

    ///
    /// Get the row name.
    ///
    const char *getConstName();

    ///
    /// Get the row site.
    ///
    dbSite * getSite();

    ///
    /// Get the origin of this row
    ///
    void getOrigin( int & x, int & y );

    ///
    /// Get the site-orientation of this row
    ///
    dbOrientType getOrient();

    ///
    /// Get the direction of this row
    ///
    dbRowDir getDirection();

    ///
    /// Get the number of sites in this row.
    ///
    int getSiteCount();

    ///
    /// Get the spacing between sites. The spacing is measured from the
    /// origin of each site.
    ///
    int getSpacing();

    ///
    /// Get the bounding box of this row
    ///
    void getBBox( adsRect & bbox );
    
    ///
    /// Create a new row.
    ///
    static dbRow * create( dbBlock * block,
                           const char * name,
                           dbSite * site,
                           int origin_x, int origin_y,
                           dbOrientType orient,
                           dbRowDir direction,
                           int num_sites,
                           int spacing );

    ///
    /// Destroy a row.
    ///
    static void destroy( dbRow * row );

    ///
    /// Destroy a row.
    ///
    static dbSet<dbRow>::iterator destroy( dbSet<dbRow>::iterator & itr );

    ///
    /// Translate a database-id back to a pointer.
    ///
    static dbRow * getRow( dbBlock * block, uint oid );
};

///////////////////////////////////////////////////////////////////////////////
///
/// A Region is the element that represents a placement region.
///
///////////////////////////////////////////////////////////////////////////////
class dbRegion : public dbObject
{
  public:
    ///
    /// Get the region name.
    ///
    dbString getName();

    ///
    /// Get the region type.
    ///
    dbRegionType getRegionType();

    ///
    /// Set the region type.
    ///
    void setRegionType( dbRegionType type );

    ///
    /// Get the instances of this region.
    ///
    dbSet<dbInst> getRegionInsts();

    ///
    // Set the value of the invalid flag.
    ///
    void setInvalid( bool v );

    ///
    /// Returns true if the invalid flag is set.
    ///
    bool isInvalid();

    ///
    /// Get the boundaries of this region.
    /// A region may have no boundaries. In this case, you may have to check the parents
    /// of this region. This case can occur when reading DEF GROUPS and REGIONS. The result is two levels
    /// of hierarchy with the boundaries on the parent.
    ///
    dbSet<dbBox> getBoundaries();

    ///
    /// Add this instance to the region
    ///
    void addInst( dbInst * inst );

    ///
    /// Remove this instance from the region
    ///
    void removeInst( dbInst * inst );

    ///
    /// Get the parent of this region. Returns null of this region has no parent.
    ///
    dbRegion * getParent();

    ///
    /// Get the children of this region.
    ///
    dbSet<dbRegion> getChildren();

    ///
    /// Add child region to this region.
    ///
    /// This method will do nothing if this child already has a parent.
    ///
    void addChild( dbRegion * region );

    ///
    /// Get the block of this region
    ///
    dbBlock * getBlock();
    
    ///
    /// Create a new region. Returns NULL if a region with this name already exists in the block.
    ///
    static dbRegion * create( dbBlock * block, const char * name );

    ///
    /// Create a new region. The region will become a child region of parent.
    /// Returns NULL if a region with this name already exists in the block.
    ///
    static dbRegion * create( dbRegion * parent, const char * name );

    ///
    /// Destroy a region.
    ///
    static void destroy( dbRegion * region );

    ///
    /// Destroy a region.
    ///
    static dbSet<dbRegion>::iterator destroy( dbSet<dbRegion>::iterator & itr );

    ///
    /// Translate a database-id back to a pointer.
    ///
    static dbRegion * getRegion( dbBlock * block, uint oid );
};

///////////////////////////////////////////////////////////////////////////////
///
/// A Library is the element that represents a collection of library-cells,
/// called Masters.
///
///////////////////////////////////////////////////////////////////////////////
class dbLib : public dbObject
{
  public:
    ///
    /// Get the library name.
    ///
    dbString getName();

    ///
    /// Get the library name.
    ///
    const char *getConstName();

    ///
    /// Get the Database units per micron.
    ///
    int getDbUnitsPerMicron();

    ///
    /// Get the technology of this library
    ///
    dbTech * getTech();

    ///
    /// Get the master-cells of this library
    ///
    dbSet<dbMaster> getMasters();

    ///
    /// Finds a specific master-cell in the library
    /// Returns NULL if the object was not found.
    ///
    dbMaster * findMaster( const char * name );

    ///
    /// Get the sites of this library
    ///
    dbSet<dbSite> getSites();

    ///
    /// Finds a specific site in the library
    /// Returns NULL if the object was not found.
    ///
    dbSite * findSite( const char * name );
    
    ///
    /// Get the LEF units of this technology.
    ///
    int getLefUnits();

    ///
    /// Set the LEF units of this technology.
    ///
    void setLefUnits( int units );

    ///
    /// Get the HierarchyDelimeter.
    /// Returns (0) if the delimeter was not set.
    /// A hierarchy delimeter can only be set at the time
    /// a library is created.
    ///
    char getHierarchyDelimeter();

    ///
    /// Set the Bus name delimeters
    ///
    void setBusDelimeters( char left, char right );

    ///
    /// Get the Bus name delimeters
    /// Left and Right are set to "zero" if the bus delimeters
    /// were not set.
    ///
    void getBusDelimeters( char & left, char & right );

    ///
    /// Create a new library. 
    ///
    static dbLib * create( dbDatabase * db, const char * name, char hierarchy_delimeter = 0);

    ///
    /// Translate a database-id back to a pointer.
    ///
    static dbLib * getLib( dbDatabase * db, uint oid );

    ///
    /// Destroy a library.
    ///
    static void destroy( dbLib * lib );
};

///////////////////////////////////////////////////////////////////////////////
///
/// A Site is the element that represents a placement site for cells in this
/// library.
///
///////////////////////////////////////////////////////////////////////////////
class dbSite : public dbObject
{
  public:
    ///
    /// Get the site name.
    ///
    dbString getName();

    ///
    /// Get the site name.
    ///
    const char *getConstName();

    ///
    /// Get the width of this site
    ///
    uint getWidth();

    ///
    /// Set the width of this site
    ///
    void setWidth( uint width );

    ///
    /// Get the height of this site
    ///
    uint getHeight();

    ///
    /// Set the height of this site
    ///
    void setHeight( uint height );

    ///
    /// Get the class of this site.
    ///
    dbSiteClass getClass();

    ///
    /// Set the class of this site
    ///
    void setClass( dbSiteClass site_class );

    ///
    /// Mark that this site has X-Symmetry
    ///
    void setSymmetryX();

    ///
    /// Returns true if this site has X-Symmetry
    ///
    bool getSymmetryX();

    ///
    /// Mark that this site has Y-Symmetry
    ///
    void setSymmetryY();

    ///
    /// Returns true if this site has Y-Symmetry
    ///
    bool getSymmetryY();
    
    ///
    /// Mark that this site has R90-Symmetry
    ///
    void setSymmetryR90();

    ///
    /// Returns true if this site has R90-Symmetry
    ///
    bool getSymmetryR90();
    
    ///
    /// Get the library of this site.
    ///
    dbLib * getLib();

    ///
    /// Create a new site.
    /// Returns NULL if a site with this name already exists
    ///
    static dbSite * create( dbLib * lib, const char * name );

    ///
    /// Translate a database-id back to a pointer.
    ///
    static dbSite * getSite( dbLib * lib, uint oid );
};

///////////////////////////////////////////////////////////////////////////////
///
/// A Master is the element that represents a master-cell from the library.
///
///////////////////////////////////////////////////////////////////////////////
class dbMaster : public dbObject
{
  public:
    ///
    /// Get the master cell name.
    ///
    dbString getName();

    ///
    /// Get the master cell name.
    ///
    const char *getConstName();

    ///
    /// Get the x,y origin of this master
    ///
    void getOrigin( int & x, int & y );

    ///
    /// Set the x,y origin of this master, default is (0,0)
    ///
    void setOrigin( int x, int y );

    ///
    /// Get the width of this master cell.
    ///
    uint getWidth();

    ///
    /// Set the width of this master cell.
    ///
    void setWidth( uint width );

    ///
    /// Get the height of this master cell.
    ///
    uint getHeight();

    ///
    /// Set the height of this master cell.
    ///
    void setHeight( uint height );

    ///
    /// is filler cell
    ///
    bool isFiller();

    ///
    /// Get the type of this master cell
    ///
    dbMasterType getType();

    ///
    /// Set the type of this master cell
    ///
    void setType( dbMasterType type );
    
    ///
    /// Get the Logical equivalent of this master
    /// Returns NULL if no equivalent was set.
    ///
    dbMaster * getLEQ();

    ///
    /// Set the Logical equivalent of this master
    /// NOTE: When setting the LEQ, the LEQ should be
    /// constructed to form a LEQ ring. The database
    /// does not enforce this. Typically, if the LEQ is
    /// set in the LEF file, than the LEQ's form a ring.
    ///
    void setLEQ( dbMaster * master );
    
    ///
    /// Get the Electical equivalent of this master
    /// Returns NULL if no equivalent was set.
    ///
    dbMaster * getEEQ();

    ///
    /// Set the Electical equivalent of this master
    /// NOTE: When setting the EEQ, the EEQ should be
    /// constructed to form a EEQ ring. The database
    /// does not enforce this. Typically, if the EEQ is
    /// set in the LEF file, than the EEQ's form a ring.
    ///
    void setEEQ( dbMaster * master );

    ///
    /// Mark that this site has X-Symmetry
    ///
    void setSymmetryX();

    ///
    /// Returns true if this site has X-Symmetry
    ///
    bool getSymmetryX();

    ///
    /// Mark that this site has Y-Symmetry
    ///
    void setSymmetryY();

    ///
    /// Returns true if this site has Y-Symmetry
    ///
    bool getSymmetryY();
    
    ///
    /// Mark that this site has R90-Symmetry
    ///
    void setSymmetryR90();

    ///
    /// Returns true if this site has R90-Symmetry
    ///
    bool getSymmetryR90();    

    ///
    /// Get the terminals of this master.
    ///
    dbSet<dbMTerm> getMTerms();

    ///
    /// Find a specific master-terminal
    /// Returns NULL if the object was not found.
    ///
    dbMTerm * findMTerm( const char * name );
    dbMTerm * findMTerm( dbBlock *block, const char * name );

    ///
    /// Get the library of this master.
    ///
    dbLib * getLib();

    ///
    /// Get the obstructions of this master
    ///
    dbSet<dbBox> getObstructions();

    ///
    /// Get the placement bounding box of this master.
    ///
    void getPlacementBoundary( adsRect & r );

    ///
    /// Apply the suppiled transform to the master obsutrctions and pin geometries.
    ///
    void transform( dbTransform & t );
    
    ///
    /// Freeze this master. dbMTerms cannot be added or delete from the master
    /// once it is frozen.
    ///
    void setFrozen();
    
    ///
    /// Returns true if the master is frozen
    ///
    bool isFrozen();
	
	int getOutputIndex();
	void setOutputIndex(int v);
	void setClockedIndex(int v);
	int getClockedIndex();


    ///
    /// Set _sequential of this master.
    ///
    void setSequential(uint v);
    
    ///
    /// Returns _sequential this master
    ///
    bool isSequential();

    ///
    /// Set _mark of this master.
    ///
    void setMark(uint mark);
    
    ///
    /// Returns _mark this master
    ///
    uint isMarked();



    bool isSpecialPower();
    void setSpecialPower(bool v);


    ///
    /// Returns the number of mterms of this master.
    ///
    int getMTermCount();

    ///
    /// Set the site of this master.
    ///h
    void setSite( dbSite * site );

    ///
    /// Set the site of this master.
    /// Returns NULL if no site has been set.
    ///
    dbSite * getSite();

    ///
    /// Returns a database unique id for this master.
    ///
    int getMasterId();

    ///
    /// Create a new master.
    /// Returns NULL if a master with this name already exists
    ///
    static dbMaster * create( dbLib * lib, const char * name );

    ///
    /// Translate a database-id back to a pointer.
    ///
    static dbMaster * getMaster( dbLib * lib, uint oid );
    void *getTimingData();
    void setTimingData(void * ptr);
};

///////////////////////////////////////////////////////////////////////////////
///
/// A MTerm is the element that represents a terminal on a Master.
///
///////////////////////////////////////////////////////////////////////////////
class dbMTerm : public dbObject
{
  public:
    ///
    /// Get the master term name.
    ///
    dbString getName();

    ///
    /// Get the master term name.
    ///
    const char *getConstName();

    ///
    /// Get the master term name. Change lib_bus_del to blk_bus_del if needed
    ///
    char *getName(dbInst *inst, char *ttname);
    char *getName(dbBlock *block, dbMaster *master, char *ttname);

    ///
    /// Get the signal type of this master-terminal.
    ///
    dbSigType getSigType();

    ///
    /// Get the IO direction of this master-terminal.
    ///
    dbIoType getIoType();

    ///
    /// Set mark of this master-terminal.
    ///
	void setMark(uint v);

    ///
    /// get mark of this master-terminal.
    ///
	bool isSetMark();

    ///
    /// Get the master this master-terminal belongs too.
    ///
    dbMaster * getMaster();
        
    ///
    /// Get the physical pins of this terminal.
    ///
    dbSet<dbMPin> getMPins();

    ///
    /// Get the target points of this terminal.
    ///
    dbSet<dbTarget> getTargets();

    ///
    /// Set the timing data. This pointer is not saved.
    ///
    void setTimingData( void * ptr );

    ///
    /// Get the timing data.
    ///
    void * getTimingData();
    
    ///
    /// Add antenna info that is not specific to an oxide model.
    ///
    void addPartialMetalAreaEntry( double inval, dbTechLayer *refly = NULL );
    void addPartialMetalSideAreaEntry( double inval, dbTechLayer *refly = NULL );
    void addPartialCutAreaEntry( double inval, dbTechLayer *refly = NULL );
    void addDiffAreaEntry( double inval, dbTechLayer *refly = NULL );

    ///
    /// Antenna info that is specific to an oxide model.
    ///
    dbTechAntennaPinModel *createDefaultAntennaModel();
    dbTechAntennaPinModel *createOxide2AntennaModel();

    ///
    /// Access and write antenna rule models -- get functions will return NULL 
    /// if model not created.
    ///
    bool hasDefaultAntennaModel() const;
    bool hasOxide2AntennaModel() const;
    dbTechAntennaPinModel *getDefaultAntennaModel() const;
    dbTechAntennaPinModel *getOxide2AntennaModel() const;
    void writeAntennaLef(lefout & writer) const;

    ///
    /// Return the index of this mterm on this master.
    /// PREQ: master must be frozen.
    ///
    int getIndex();

    ///
    /// Create a new master terminal.
    /// Returns NULL if a master terminal with this name already exists
    ///
    static dbMTerm * create( dbMaster * master,
                             const char * name,
                             dbIoType io_type = dbIoType(),
                             dbSigType sig_type = dbSigType() );

    ///
    /// Translate a database-id back to a pointer.
    ///
    static dbMTerm * getMTerm( dbMaster * master, uint oid );
};

///////////////////////////////////////////////////////////////////////////////
///
/// A MPin is the element that represents a physical pin on a master-terminal.
///
///////////////////////////////////////////////////////////////////////////////
class dbMPin : public dbObject
{
  public:
    ///
    /// Get the master-terminal this pin belongs too.
    ///
    dbMTerm * getMTerm();

    ///
    /// Get the master this pin belongs too.
    ///
    dbMaster * getMaster();
        
    ///
    /// Get the geometry of this pin.
    ///
    dbSet<dbBox> getGeometry();
    
    ///
    /// Create a new physical pin.
    ///
    static dbMPin * create( dbMTerm * mterm );

    ///
    /// Translate a database-id back to a pointer.
    ///
    static dbMPin * getMPin( dbMaster * master, uint oid );
};

///////////////////////////////////////////////////////////////////////////////
///
/// A Target is the element that represents a physical target point on a MTerm.
///
///////////////////////////////////////////////////////////////////////////////
class dbTarget : public dbObject
{
  public:
    ///
    /// Get the master this target belongs too.
    ///
    dbMaster * getMaster();

    ///
    /// Get the mterm this target
    ///
    dbMTerm * getMTerm();

    ///
    /// Get the tech-layer this target
    ///
    dbTechLayer * getTechLayer();

    ///
    /// Get the target point of this target.
    ///
    adsPoint getPoint();
        
    ///
    /// Create a new master terminal.
    /// Returns NULL if a master terminal with this name already exists
    ///
    static dbTarget * create( dbMTerm * mterm, dbTechLayer * layer, adsPoint point );

    ///
    /// Destroy a target
    ///
    static void destroy( dbTarget * t );

    ///
    /// Destroy a target
    ///
    static dbSet<dbTarget>::iterator destroy( dbSet<dbTarget>::iterator & itr );

    ///
    /// Translate a database-id back to a pointer.
    ///
    static dbTarget * getTarget( dbMaster * master, uint oid );
};

///////////////////////////////////////////////////////////////////////////////
///
/// A Tech is the element that represents technology specific data.
///
///////////////////////////////////////////////////////////////////////////////
class dbTech : public dbObject
{
  public:
    ///
    /// Set the Database distance units per micron.
    ///
    /// Legal values are 100, 200, 1000, 2000, 10000, 20000
    ///
    void setDbUnitsPerMicron( int value );
    
    ///
    /// Get the Database units per micron.
    ///
    int getDbUnitsPerMicron();

    ///
    /// Get the technolgy layers. The layers are ordered from the
    /// bottom mask number to the top mask number.
    ///
    dbSet<dbTechLayer> getLayers();

    ///
    /// Find the technology layer.
    /// Returns NULL if the object was not found.
    ///
    dbTechLayer * findLayer( const char * name );

    ///
    /// Find the technology layer.
    /// Returns NULL if the object was not found.
    ///
    dbTechLayer * findLayer( int layer_number );

    ///
    /// Find the technology routing layer.
    /// Returns NULL if the object was not found.
    ///
    dbTechLayer * findRoutingLayer( int level_number );

    ///
    /// Get the technolgy vias. This includes non-default-rule-vias.
    ///
    dbSet<dbTechVia> getVias();

    ///
    /// Find the technology via.
    /// Returns NULL if the object was not found.
    ///
    dbTechVia * findVia( const char * name );

    ///
    /// Get the LEF units of this technology.
    ///
    int getLefUnits();

    ///
    /// Set the LEF units of this technology.
    ///
    void setLefUnits( int units );

    ///
    /// Get the LEF version in this technology as a number or as a string.
    ///
    double getLefVersion() const;
    const char *getLefVersionStr() const;

    ///
    /// Set the LEF version of this technology, in both number and string form.
    ///
    void setLefVersion(double inver);

    ///
    ///  Get and set the NOWIREEXTENSIONATPIN construct
    ///
    bool hasNoWireExtAtPin() const;
    dbOnOffType getNoWireExtAtPin() const;
    void setNoWireExtAtPin(dbOnOffType intyp);

    ///
    ///  Get and set the NAMESCASESENSITIVE construct
    ///
    dbOnOffType getNamesCaseSensitive() const;
    void setNamesCaseSensitive(dbOnOffType intyp);

    ///
    /// Handle LEF CLEARANCEMEASURE construct
    ///
    bool hasClearanceMeasure() const;
    dbClMeasureType getClearanceMeasure() const;
    void setClearanceMeasure(dbClMeasureType inmeas);

    ///
    /// Handle LEF USEMINSPACING for pins and obstruction separately.
    ///
    bool hasUseMinSpacingObs() const;
    dbOnOffType getUseMinSpacingObs() const;
    void setUseMinSpacingObs(dbOnOffType inval);

    bool hasUseMinSpacingPin() const;
    dbOnOffType getUseMinSpacingPin() const;
    void setUseMinSpacingPin(dbOnOffType inval);
    
    ///
    ///  Handle MANUFACTURINGGRID construct
    ///  NOTE: Assumes conversion to internal DB units, 
    ///  NOT microns or LEF/DEF units
    ///
    bool hasManufacturingGrid() const;
    int  getManufacturingGrid() const;
    void setManufacturingGrid(int ingrd);

    ///
    /// Get the number of layers in this technology.
    ///
    int getLayerCount();

    /// 
    /// Get the number of routing-layers in this technology.
    ///
    int getRoutingLayerCount();

    ///
    /// Get the number of vias in this technolgy.
    ///
    int getViaCount();
    
    ///
    /// Get the non-default rules
    ///
    dbSet<dbTechNonDefaultRule> getNonDefaultRules();

    ///
    /// Find a specific rule
    ///
    dbTechNonDefaultRule * findNonDefaultRule( const char * rulename );

    ///
    /// Find a specific rule
    /// Returns NULL if no rule exists.
    ///
    dbTechSameNetRule * findSameNetRule( dbTechLayer * l1, dbTechLayer * l2 );
    
    ///
    /// Get the same-net rules of this non-default rule.
    ///
    void getSameNetRules( std::vector<dbTechSameNetRule *> & rules );

    ///
    ///
    ///
    dbSet<dbTechViaRule> getViaRules();

    ///
    ///
    ///
    dbSet<dbTechViaGenerateRule> getViaGenerateRules();

    ///
    ///
    ///
    dbTechViaRule * findViaRule( const char * name );

    ///
    ///
    ///
    dbTechViaGenerateRule * findViaGenerateRule( const char * name );

    ///
    ///
    ///
    void checkLayer( bool typeChk, bool widthChk, bool pitchChk, bool spacingChk);

    ///
    /// Create a new technology.
    /// Returns NULL if a database technology already exists
    ///
    static dbTech * create( dbDatabase * db, int dbu_per_micron = 1000 );

    ///
    /// Translate a database-id back to a pointer.
    ///
    static dbTech * getTech( dbDatabase * db, uint oid );

    ///
    /// Destroy a technology.
    /// TODO: Define what happens to the libs and the chip.
    ///
    static void destroy( dbTech * tech );
};

///////////////////////////////////////////////////////////////////////////////
///
/// A TechLayer is the element that represents a specific process layer in
/// a technolgy.
///
///////////////////////////////////////////////////////////////////////////////
class dbTechLayer : public dbObject
{
  public:
    ///
    /// Get the layer name.
    ///
    dbString getName();

    ///
    /// Get the layer name.
    ///
    const char *getConstName();

    ///
    /// Returns true if this layer has an alias.
    ///
    bool hasAlias();
    
    ///
    /// Get the layer alias.
    ///
    dbString getAlias();

    ///
    /// Set the layer alias.
    ///
    void setAlias( const char * alias );

    ///
    /// Get the minimum path-width.
    ///
    uint getWidth();
    void setWidth( int width );

    ///
    /// Get the minimum object-to-object spacing.
    ///
    int getSpacing();
    void setSpacing( int spacing );

    ///
    /// Get the minimum spacing to a wide line.
    ///
    int getSpacing( int width, int length = 0 );

    ///
    /// Get the low end of the uppermost range for wide wire design rules.
    ///
    void getMaxWideDRCRange( int & owidth, int & olength );
    void getMinWideDRCRange( int & owidth, int & olength );

    /// Get the collection of spacing rules for the object, assuming
    /// coding in LEF 5.4 format.
    /// Return false if rules not encoded in this format.
    /// Contents of sp_rules are undefined if function returns false.
    ///
    bool  getV54SpacingRules( dbSet<dbTechLayerSpacingRule> & sp_rules ) const;

    ///
    /// API for version 5.5 spacing rules, expressed as a 2D matrix with
    /// index tables  LEF 5.4 and 5.5 rules should not co-exist -- although
    /// this is not enforced here.
    /// V5.4 and V5.5 spacing rules are optional -- in this case there is a
    /// single spacing value for all length/width combinations.
    ///
    bool  hasV55SpacingRules() const;
    void  printV55SpacingRules(lefout & writer) const;
    bool  getV55SpacingTable( std::vector< std::vector<uint> > & sptbl )const;

    void  initV55LengthIndex( uint numelems );
    void  addV55LengthEntry( uint length );
    void  initV55WidthIndex( uint numelems );
    void  addV55WidthEntry( uint width );
    void  initV55SpacingTable( uint numrows, uint numcols );
    void  addV55SpacingTableEntry( uint inrow, uint incol, uint spacing );

    bool  getV55InfluenceRules( std::vector<dbTechV55InfluenceEntry *> & inf_tbl );
    dbSet<dbTechV55InfluenceEntry> getV55InfluenceEntries();

    ///
    ///  create container for layer specific antenna rules 
    ///  currently only oxide1 (default) and oxide2 models supported.
    ///
    dbTechLayerAntennaRule *createDefaultAntennaRule();
    dbTechLayerAntennaRule *createOxide2AntennaRule();

    ///
    /// Access and write antenna rule models -- get functions will return NULL 
    /// if model not created.
    ///
    bool hasDefaultAntennaRule() const;
    bool hasOxide2AntennaRule() const;
    dbTechLayerAntennaRule *getDefaultAntennaRule() const;
    dbTechLayerAntennaRule *getOxide2AntennaRule() const;
    void writeAntennaRulesLef(lefout & writer) const;

    ///
    ///
    /// Get collection of minimum cuts, minimum enclosure rules, if exist
    ///
    bool  getMinimumCutRules( std::vector<dbTechMinCutRule *> & cut_rules );
    bool  getMinEnclosureRules( std::vector<dbTechMinEncRule *> & enc_rules );

    dbSet<dbTechMinCutRule> getMinCutRules();
    dbSet<dbTechMinEncRule> getMinEncRules();

    ///
    /// Get/Set the minimum feature size (pitch).
    ///
    int getPitch();
    void setPitch( int pitch );

    ///
    ///  Get THICKNESS in DB units, and return indicator of existence.
    ///  Do not trust value of output parm if return value is false.
    ///
    bool getThickness(uint & inthk) const;
    void setThickness(uint thickness);

    ///
    ///  Get/set AREA parameter.  This interface is used when a
    ///  reasonable default exists.
    ///
    bool hasArea() const;
    double getArea() const;
    void setArea(double area);

    ///
    ///  Get/set MAXWIDTH parameter.  This interface is used when a
    ///  reasonable default exists.
    ///
    bool hasMaxWidth() const;
    uint getMaxWidth() const;
    void setMaxWidth(uint max_width);

    ///
    ///  Get/set MINSTEP parameter.  This interface is used when a
    ///  reasonable default exists.
    ///
    bool hasMinStep() const;
    uint getMinStep() const;
    void setMinStep(uint min_step);

    ///
    ///  Get/set PROTRUSIONWIDTH parameter.  This interface is used when a
    ///  reasonable default exists.
    ///
    bool hasProtrusion() const;
    uint getProtrusionWidth() const;
    uint getProtrusionLength() const;
    uint getProtrusionFromWidth() const;
    void setProtrusion(uint pt_width, uint pt_length, uint pt_from_width);

    /// Get the layer-type
    ///
    dbTechLayerType getType();

    ///
    /// Get/Set the layer-direction
    ///
    dbTechLayerDir getDirection();
    void setDirection( dbTechLayerDir direction );

    ///
    /// Get/Set the resitance per square nm
    ///
    double getResistance();
    void setResistance( double res );

    ///
    /// Get/Set the capacitance per square nm
    ///
    double getCapacitance();
    void setCapacitance( double cap );

    ///
    /// Get/Set the edge capacitance
    ///
    double getEdgeCapacitance();
    void setEdgeCapacitance( double cap );

    ///
    /// Get/Set the edge capacitance
    ///
    uint getWireExtension();
    void setWireExtension( uint ext );

    ///
    /// Get mask-order number of this layer.
    ///
    int getNumber() const;

    ///
    /// Get routing-level of this routing layer. The routing level
    /// is from [1-num_layers]. This function returns 0, if this
    /// layer is not a routing layer.
    ///
    int getRoutingLevel();
    
    ///
    /// Get the layer below this layer.
    /// Returns NULL if at bottom of layer stack.
    ///
    dbTechLayer * getLowerLayer();
    
    ///
    /// Get the layer above this layer.
    /// Returns NULL if at top of layer stack.
    ///
    dbTechLayer * getUpperLayer();

    ///
    /// Get the technology this layer belongs too.
    ///
    dbTech * getTech();
    
    ///
    /// Create a new layer. The mask order is implicit in the create order.
    /// Returns NULL if a layer with this name already exists
    ///
    static dbTechLayer * create( dbTech * tech, const char * name,
                                 dbTechLayerType type );

    ///
    /// Translate a database-id back to a pointer.
    ///
    static dbTechLayer * getTechLayer( dbTech * tech, uint oid );
};

///////////////////////////////////////////////////////////////////////////////
///
/// A TechVia is the element that represents a specific process VIA in
/// a technolgy.
///
///////////////////////////////////////////////////////////////////////////////
class dbTechVia : public dbObject
{
  public:
    ///
    /// Get the via name.
    ///
    dbString getName();

    ///
    /// Get the via name.
    ///
    const char *getConstName();

    ///
    /// Returns true if this via is a default
    ///
    bool isDefault();

    ///
    /// Set the default flag to true.
    ///
    void setDefault();

    ///
    /// Returns true if this via is a top-of-stack
    ///
    bool isTopOfStack();

    ///
    /// Set the top-of-stack flag to true.
    ///
    void setTopOfStack();

    ///
    /// Get the resitance per square nm
    ///
    double getResistance();

    ///
    /// Set the resitance per square nm
    ///
    void setResistance( double res );

    ///
    /// Set the pattern value of this via.
    /// The pattern is ignored if the pattern is already set on this via
    ///
    void setPattern( const char * pattern );

    ///
    /// Get the pattern value of this via.
    /// Returns and empty ("") string if a pattern has not been set.
    ///
    dbString getPattern();

    ///
    /// Set generate rule that was used to genreate this via.
    ///
    void setViaGenerateRule( dbTechViaGenerateRule * rule );

    ///
    /// Get the generate rule that was used to genreate this via.
    ///
    dbTechViaGenerateRule * getViaGenerateRule();

    ///
    /// Returns true if this via has params.
    ///
    bool hasParams();
    
    ///
    /// Set via params to generate this via. This method will create the shapes
    /// of this via. All previous shapes are destroyed.
    ///
    void setViaParams( const dbViaParams & params );

    ///
    /// Get the via params used to generate this via.
    ///
    void getViaParams( dbViaParams & params );

    ///
    /// Get the technology this via belongs too.
    ///
    dbTech * getTech();

    ///
    /// Get the bbox of this via.
    /// Returns NULL if this via has no shapes.
    ///
    dbBox * getBBox();

    ///
    /// Get the boxes of this VIA
    ///
    dbSet<dbBox> getBoxes();

    ///
    /// Get the upper-most layer of this via reaches
    /// Returns NULL if this via has no shapes.
    ///
    dbTechLayer * getTopLayer();

    ///
    /// Get the lower-most layer of this via reaches
    /// Returns NULL if this via has no shapes.
    ///
    dbTechLayer * getBottomLayer();

    ///
    /// Returns the non-default rule this via belongs too.
    /// Returns NULL if this via is not part of a non-default rule.
    ///
    dbTechNonDefaultRule * getNonDefaultRule();
    
    ///
    /// Create a new via.
    /// Returns NULL if a via with this name already exists.
    ///
    static dbTechVia * create( dbTech * tech, const char * name );

    ///
    /// Create a new non-default-rule via.
    /// Returns NULL if a via with this name already exists.
    ///
    static dbTechVia * create( dbTechNonDefaultRule * rule, const char * name );
    ///
    /// Create a new non-default-rule via by cloning an existing via (not
    /// necessarily from the same non-default rule
    /// Returns NULL if a via with this name already exists.
    ///
    static dbTechVia * clone( dbTechNonDefaultRule * rule, dbTechVia * invia_, const char *new_name );

    ///
    /// Translate a database-id back to a pointer.
    ///
    static dbTechVia * getTechVia( dbTech * tech, uint oid );
};

///////////////////////////////////////////////////////////////////////////////
///
/// A TechViaRule is the element that represents a LEF VIARULE
///
///////////////////////////////////////////////////////////////////////////////
class dbTechViaRule : public dbObject
{
  public:
    ///
    /// Get the via-rule name.
    ///
    dbString getName();

    ///
    /// Add this via to this rule
    ///
    void addVia( dbTechVia * via );

    ///
    /// Get the number of vias assigned to this rule
    ///
    uint getViaCount();

    ///
    /// Return the via of this index. The index ranges from [0 ... (viaCount-1)]
    ///
    dbTechVia * getVia( uint indx );

    ///
    /// Get the number of layer-rules assigned to this rule
    ///
    uint getViaLayerRuleCount();

    ///
    /// Return the layer-rule of this index. The index ranges from [0 ... (viaCount-1)]
    ///
    dbTechViaLayerRule * getViaLayerRule( uint indx );

    ///
    /// Create a new via.
    /// Returns NULL if a via-rule with this name already exists.
    ///
    static dbTechViaRule * create( dbTech * tech, const char * name );

    ///
    /// Translate a database-id back to a pointer.
    ///
    static dbTechViaRule * getTechViaRule( dbTech * tech, uint oid );
};

///////////////////////////////////////////////////////////////////////////////
///
/// A TechViaLayerRule is the element that represents a LEF VIARULE LAYER 
///
///////////////////////////////////////////////////////////////////////////////
class dbTechViaLayerRule : public dbObject
{
  public:
    ///
    /// Get the layer
    ///
    dbTechLayer * getLayer();

    ///
    /// Get the rule direction
    ///
    dbTechLayerDir getDirection();

    ///
    /// Set the rule direction
    ///
    void setDirection( dbTechLayerDir dir);

    ///
    /// Returns true if width rule is set
    ///
    bool hasWidth();

    ///
    /// Returns the width rule
    ///
    void getWidth( int & minWidth, int & maxWidth );

    ///
    /// Set the width rule
    ///
    void setWidth( int minWidth, int maxWidth );

    ///
    /// Returns true if the enclosure rule is set.
    ///
    bool hasEnclosure();

    ///
    /// Returns the enclosure rule
    ///
    void getEnclosure( int & overhang1, int & overhang2 );

    ///
    /// Set the enclosure rule
    ///
    void setEnclosure( int overhang1, int overhang2 );

    ///
    /// Returns true if the overhang rule is set.
    ///
    bool hasOverhang();

    ///
    /// Returns the overhang rule
    ///
    int getOverhang();

    ///
    /// Set the overhang rule
    ///
    void setOverhang( int overhang );

    ///
    /// Returns true if the metal-overhang rule is set.
    ///
    bool hasMetalOverhang();

    ///
    /// Returns the overhang rule
    ///
    int getMetalOverhang();

    ///
    /// Set the overhang rule
    ///
    void setMetalOverhang( int overhang );

    ///
    /// returns true if the rect rule is set
    ///
    bool hasRect();

    ///
    /// Get the rect rule
    ///
    void getRect( adsRect & r );

    ///
    /// Set the rect rule
    ///
    void setRect( const adsRect & r );

    ///
    /// returns true if the spacing rule is set
    ///
    bool hasSpacing();

    ///
    /// Get the spacing rule.
    ///
    void getSpacing( int & x_spacing, int & y_spacing );

    ///
    /// Set the spacing rule.
    ///
    void setSpacing( int x_spacing, int y_spacing );

    ///
    /// Returns true if the resistance rule is set.
    ///
    bool hasResistance();

    ///
    /// Set the resistance
    ///
    void setResistance( double r );

    ///
    /// Get the resistance
    ///
    double getResistance();
    
    ///
    /// Create a new via-layer_rule.
    ///
    static dbTechViaLayerRule * create( dbTech * tech,  dbTechViaRule * rule, dbTechLayer * layer );

    ///
    /// Create a new via-layer_rule.
    ///
    static dbTechViaLayerRule * create( dbTech * tech,  dbTechViaGenerateRule * rule, dbTechLayer * layer );

    ///
    /// Translate a database-id back to a pointer.
    ///
    static dbTechViaLayerRule * getTechViaLayerRule( dbTech * tech, uint oid );
};

///////////////////////////////////////////////////////////////////////////////
///
/// A TechViaGenerateRule is the element that represents a LEF VIARULE GENERATE
///
///////////////////////////////////////////////////////////////////////////////
class dbTechViaGenerateRule : public dbObject
{
  public:
    ///
    /// Get the via-rule name.
    ///
    dbString getName();

    ///
    /// Returns true if this is the default rule.
    ///
    bool isDefault();

    ///
    /// Get the number of layer-rules assigned to this rule
    ///
    uint getViaLayerRuleCount();

    ///
    /// Return the layer-rule of this index. The index ranges from [0 ... (viaCount-1)]
    ///
    dbTechViaLayerRule * getViaLayerRule( uint indx );
    
    ///
    /// Create a new via.
    /// Returns NULL if a via-rule with this name already exists.
    ///
    static dbTechViaGenerateRule * create( dbTech * tech, const char * name, bool is_default );

    ///
    /// Translate a database-id back to a pointer.
    ///
    static dbTechViaGenerateRule * getTechViaGenerateRule( dbTech * tech, uint oid );
};

///////////////////////////////////////////////////////////////////////////////
///
/// A TechLayerSpacingRule stores a design rule in LEF V5.4 format.
/// These are bound to layers -- a layer may have several design rules to
/// describe required spacing among different widths and parallel lengths.
///
///////////////////////////////////////////////////////////////////////////////

class dbTechLayerSpacingRule : public dbObject
{
  public:
  /// Combine data and predicates for elements of rule
    bool  isUnconditional() const;
    uint  getSpacing() const;
    bool  getLengthThreshold( uint & threshold ) const;
    bool  getLengthThresholdRange( uint & rmin, uint & rmax ) const;
    bool  getRange( uint & rmin, uint & rmax ) const;
    bool  hasUseLengthThreshold() const;
    bool  getInfluence(uint & influence) const;
    bool  getInfluenceRange( uint & rmin, uint & rmax ) const;
    bool  getRangeRange( uint & rmin, uint & rmax ) const;
    bool  getAdjacentCuts( uint & numcuts, uint & within, uint & spacing ) const;
    bool  getCutLayer4Spacing( dbTechLayer *&outly ) const;
    void  writeLef( lefout & writer ) const;

    void  setLengthThreshold( uint threshold );
    void  setSpacing( uint spacing );
    void  setLengthThresholdRange( uint rmin, uint rmax );
    void  setRange( uint rmin, uint rmax );
    void  setUseLengthThreshold();
    void  setInfluence( uint influence );
    void  setInfluenceRange( uint rmin, uint rmax );
    void  setRangeRange( uint rmin, uint rmax );
    void  setAdjacentCuts( uint numcuts, uint within, uint spacing );
    void  setCutLayer4Spacing( dbTechLayer *cutly );

    ///
    /// Create a new layer spacing rule.
    /// Returns pointer to newly created object
    ///
    static dbTechLayerSpacingRule * create( dbTechLayer * inly );
    static dbTechLayerSpacingRule * getTechLayerSpacingRule( dbTechLayer * inly, uint dbid );

};

///////////////////////////////////////////////////////////////////////////////
///
/// A dbTechMinCutRule stores rules for minimum cuts 
/// in LEF V5.5 format.
/// These are bound to layers -- a layer may have several minimum cut rules to
/// describe required cuts at intersections of  different widths and
/// protrusion lengths.
///
///////////////////////////////////////////////////////////////////////////////

class dbTechMinCutRule : public dbObject
{
  public:

    bool getMinimumCuts( uint & numcuts, uint & width ) const;
    void setMinimumCuts( uint numcuts, uint width, bool above_only, bool below_only);
    bool getLengthForCuts( uint & length, uint & within ) const; 
    void setLengthForCuts( uint length, uint within ); 
    bool isAboveOnly() const;
    bool isBelowOnly() const;
    void writeLef( lefout & writer ) const;
    static dbTechMinCutRule * create( dbTechLayer * inly );
    static dbTechMinCutRule * getMinCutRule( dbTechLayer * inly, uint dbid );
};

///////////////////////////////////////////////////////////////////////////////
///
/// A dbTechMinEncRule stores rules for minimum enclosure area
/// in LEF V5.5 format.
/// These are bound to layers -- a layer may have several minimum enclosure
/// rules to describe connections to wires of different widths
///
///////////////////////////////////////////////////////////////////////////////

class dbTechMinEncRule : public dbObject
{
  public:

    bool getEnclosure( uint & area ) const;
    void setEnclosure( uint area );
    bool getEnclosureWidth( uint & width ) const;
    void setEnclosureWidth( uint width ); 
    void writeLef( lefout & writer ) const;

    static dbTechMinEncRule * create( dbTechLayer * inly );
    static dbTechMinEncRule * getMinEncRule( dbTechLayer * inly, uint dbid );
};

///////////////////////////////////////////////////////////////////////////////
///
/// A dbTechV55InfluenceEntry stores an entry in the table for V5.5 format
/// influence spacing rules.
/// Influence spacing in V5.5 describes the required spacing (_spacing) for
/// any wire within a distance (_within) a wire of width (_width).
/// These are bound to layers.
///
///////////////////////////////////////////////////////////////////////////////

class dbTechV55InfluenceEntry: public dbObject
{
  public:
    bool getV55InfluenceEntry( uint & width, uint & within, uint & spacing ) const;
    void setV55InfluenceEntry( const uint & width, const uint & within, const uint & spacing );
    void writeLef(lefout & writer) const;

    static dbTechV55InfluenceEntry * create( dbTechLayer * inly );
    static dbTechV55InfluenceEntry * getV55InfluenceEntry( dbTechLayer * inly, uint dbid );
};

///////////////////////////////////////////////////////////////////////////////
///
/// A dbTechLayerAntennaRule expresses a single antenna rule for a given layer.
///
///////////////////////////////////////////////////////////////////////////////

class dbTechLayerAntennaRule: public dbObject
{
 public:
  bool isValid() const;
  void writeLef(lefout & writer) const;

  void setAreaFactor(double factor, double diffuse = -1.0);
  void setSideAreaFactor(double factor, double diffuse = -1.0);
  
  void setPAR(double ratio, double diff_ratio = 0.0);
  void setCAR(double ratio, double diff_ratio = 0.0);
  void setPSR(double ratio, double diff_ratio = 0.0);
  void setCSR(double ratio, double diff_ratio = 0.0);

  void setPAR_PWL(const std::vector<double> & diff_idx, const std::vector<double> & ratios);
  void setCAR_PWL(const std::vector<double> & diff_idx, const std::vector<double> & ratios);
  void setPSR_PWL(const std::vector<double> & diff_idx, const std::vector<double> & ratios);
  void setCSR_PWL(const std::vector<double> & diff_idx, const std::vector<double> & ratios);

  static dbTechLayerAntennaRule * getAntennaRule( dbTech * inly, uint dbid );
};


///////////////////////////////////////////////////////////////////////////////
///
/// A dbTechAntennaPinModel contains model specific antenna info for a pin
///
///////////////////////////////////////////////////////////////////////////////

class dbTechAntennaPinModel: public dbObject
{
 public:
  void addGateAreaEntry( double inval, dbTechLayer *refly = NULL );
  void addMaxAreaCAREntry( double inval, dbTechLayer *refly = NULL );
  void addMaxSideAreaCAREntry( double inval, dbTechLayer *refly = NULL );
  void addMaxCutCAREntry( double inval, dbTechLayer *refly = NULL );
  void writeLef(dbTech *tech, lefout & writer) const;

  static dbTechAntennaPinModel * getAntennaPinModel( dbMaster * master, uint dbid );
};

///////////////////////////////////////////////////////////////////////////////
///
/// A NonDefaultRule is the element that represents a Non-default technology
/// rule.
///
///////////////////////////////////////////////////////////////////////////////
class dbTechNonDefaultRule : public dbObject
{
  public:
    ///
    /// Get the rule name.
    ///
    dbString getName();

    ///
    /// Get the rule name.
    ///
    const char *getConstName();

    ///
    /// Returns true if this rule is a block rule
    ///
    bool isBlockRule();

    ///
    /// Find a specific layer-rule.
    /// Returns NULL if there is no layer-rule.
    ///
    dbTechLayerRule * getLayerRule( dbTechLayer * layer );

    ///
    /// Get the vias of this non-default rule.
    ///
    void getLayerRules( std::vector<dbTechLayerRule *> & layer_rules );

    ///
    /// Get the vias of this non-default rule.
    ///
    void getVias( std::vector<dbTechVia *> & vias );

    ///
    /// Find a specific rule
    /// Returns NULL if no rule exists.
    ///
    dbTechSameNetRule * findSameNetRule( dbTechLayer * l1, dbTechLayer * l2 );

    ///
    /// Get the same-net rules of this non-default rule.
    ///
    void getSameNetRules( std::vector<dbTechSameNetRule *> & rules );

    /////////////////////////
    // 5.6 DEF additions
    /////////////////////////

    ///
    /// Set the hard spacing rule.
    ///
    bool getHardSpacing();

    /// 
    /// Get the hard spacing rule.
    /// 
    void setHardSpacing( bool value );

    /// 
    ///  Add a use via.
    /// 
    void addUseVia( dbTechVia * via );

    /// 
    ///  Get vias to use.
    /// 
    void getUseVias( std::vector<dbTechVia *> & vias );

    /// 
    ///  Add a use via.
    /// 
    void addUseViaRule( dbTechViaGenerateRule * rule );

    /// 
    ///  Get vias to use.
    /// 
    void getUseViaRules( std::vector<dbTechViaGenerateRule *> & rules );

    /// 
    /// Assign a minimum number of cuts to this cut-layer
    /// 
    void setMinCuts( dbTechLayer * cut_layer, int count );

    /// 
    /// Get the minimum number of cuts for this cut-layer.
    ///
    /// Returns false if a value has not been specified.
    /// 
    bool getMinCuts( dbTechLayer * cut_layer, int & count );

    ///
    /// Create a new non-default-rule.
    /// Returns NULL if a non-default-rule with this name already exists
    ///
    static dbTechNonDefaultRule * create( dbTech * tech, const char * name );

    ///
    /// Create a new non-default-rule.
    /// Returns NULL if a non-default-rule with this name already exists
    ///
    static dbTechNonDefaultRule * create( dbBlock * block, const char * name );

    ///
    /// Translate a database-id back to a pointer.
    ///
    static dbTechNonDefaultRule * getTechNonDefaultRule( dbTech * tech, uint oid );

    ///
    /// Translate a database-id back to a pointer.
    ///
    static dbTechNonDefaultRule * getTechNonDefaultRule( dbBlock * block, uint oid );
};

///////////////////////////////////////////////////////////////////////////////
///
/// A TechLayerRule is the element that represents a non-default layer
/// rule.
///
///////////////////////////////////////////////////////////////////////////////
class dbTechLayerRule : public dbObject
{
  public:
    ///
    /// Get the layer this rule represents
    ///
    dbTechLayer * getLayer();

    ///
    /// Returns true if this rule is a block rule
    ///
    bool isBlockRule();

    ///
    /// Get the non-default-rule this layer-rule belongs too.
    ///
    dbTechNonDefaultRule * getNonDefaultRule();
    
    ///
    /// Get the minimum path-width.
    ///
    int getWidth();

    ///
    /// Set the minimum path-width.
    ///
    void setWidth( int width );

    ///
    /// Get the minimum object-to-object spacing.
    ///
    int getSpacing();

    ///
    /// Set the minimum object-to-object spacing.
    ///
    void setSpacing( int spacing );

    ///
    /// Get the resitance per square nm
    ///
    double getResistance();

    ///
    /// Set the resitance per square nm
    ///
    void setResistance( double res );

    ///
    /// Get the capacitance per square nm
    ///
    double getCapacitance();

    ///
    /// Set the capacitance per square nm
    ///
    void setCapacitance( double cap );

    ///
    /// Get the edge capacitance
    ///
    double getEdgeCapacitance();

    ///
    /// Set the edge capacitance
    ///
    void setEdgeCapacitance( double cap );

    ///
    /// Get the edge capacitance
    ///
    uint getWireExtension();

    ///
    /// Set the edge capacitance
    ///
    void setWireExtension( uint ext );

    ///
    /// Create a new layer-rule.
    /// Returns NULL if a layer-rule for this layer already exists.
    ///
    static dbTechLayerRule * create( dbTechNonDefaultRule * rule, dbTechLayer * layer );

    ///
    /// Translate a database-id back to a pointer.
    ///
    static dbTechLayerRule * getTechLayerRule( dbTech * tech, uint oid );

    ///
    /// Translate a database-id back to a pointer.
    ///
    static dbTechLayerRule * getTechLayerRule( dbBlock * block, uint oid );
};

///////////////////////////////////////////////////////////////////////////////
///
/// A TechSameNetRule
///
///////////////////////////////////////////////////////////////////////////////
class dbTechSameNetRule : public dbObject
{
  public:
    ///
    /// Get the layer this rule represents
    ///
    dbTechLayer * getLayer1();

    ///
    /// Get the layer this rule represents
    ///
    dbTechLayer * getLayer2();

    ///
    /// Get the minimum net-to-net spacing.
    ///
    int getSpacing();

    ///
    /// Set the minimum net-to-net spacing.
    ///
    void setSpacing( int spacing );

    ///
    /// Set the flag to allow stacked vias, the default value is false.
    ///
    void setAllowStackedVias( bool value );

    ///
    /// Get the allow stacked vias flag.
    ///
    bool getAllowStackedVias();
    
    ///
    /// Create a new default samenet rule.
    /// Returns NULL if a rule already exists between these layers.
    ///
    static dbTechSameNetRule * create( dbTechLayer * layer1, dbTechLayer * layer2 );
    ///
    /// Create a new non-default samenet rule.
    /// Returns NULL if a rule already exists between these layers.
    ///
    static dbTechSameNetRule * create( dbTechNonDefaultRule * rule, dbTechLayer * layer1, dbTechLayer * layer2 );

    ///
    /// Translate a database-id back to a pointer.
    ///
    static dbTechSameNetRule * getTechSameNetRule( dbTech * tech, uint oid );
};

class dbViaParams : private _dbViaParams
//class dbViaParams : public _dbViaParams
{
    friend class dbVia;
    friend class dbTechVia;
    dbTech *  _tech;

    dbViaParams( const _dbViaParams & p );
  public:
    dbViaParams();
    dbViaParams( const dbViaParams & p );
    ~dbViaParams();
    
    int getXCutSize() const;
    int getYCutSize() const;
    int getXCutSpacing() const;
    int getYCutSpacing() const;
    int getXTopEnclosure() const;
    int getYTopEnclosure() const;
    int getXBottomEnclosure() const;
    int getYBottomEnclosure() const;
    int getNumCutRows() const;
    int getNumCutCols() const;
    int getXOrigin() const;
    int getYOrigin() const;
    int getXTopOffset() const;
    int getYTopOffset() const;
    int getXBottomOffset() const;
    int getYBottomOffset() const;
    dbTechLayer * getTopLayer() const;
    dbTechLayer * getCutLayer() const;
    dbTechLayer * getBottomLayer() const;

    void setXCutSize(int value);
    void setYCutSize(int value);
    void setXCutSpacing(int value);
    void setYCutSpacing(int value);
    void setXTopEnclosure(int value);
    void setYTopEnclosure(int value);
    void setXBottomEnclosure(int value);
    void setYBottomEnclosure(int value);
    void setNumCutRows(int value);
    void setNumCutCols(int value);
    void setXOrigin(int value);
    void setYOrigin(int value);
    void setXTopOffset(int value);
    void setYTopOffset(int value);
    void setXBottomOffset(int value);
    void setYBottomOffset(int value);
    void setTopLayer( dbTechLayer * layer );
    void setCutLayer( dbTechLayer * layer );
    void setBottomLayer( dbTechLayer * layer );
};

} // namespace

#endif
