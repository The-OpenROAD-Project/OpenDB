%template(vector_str) std::vector<std::string>;


// DB specital types
%typemap(out) odb::dbString {
    Tcl_Obj *obj = Tcl_NewStringObj($1.c_str(), $1.length());
    Tcl_SetObjResult(interp, obj);
}
%typemap(out) odb::dbStringProperty {
    Tcl_Obj *obj = Tcl_NewStringObj($1.getValue().c_str(), $1.getValue().length());
    Tcl_SetObjResult(interp, obj);
}
%typemap(out) odb::dbStringProperty {
    Tcl_Obj *obj = Tcl_NewStringObj($1.getValue().c_str(), $1.getValue().length());
    Tcl_SetObjResult(interp, obj);
}

%typemap(out) odb::dbOnOffType, dbOnOffType {
    Tcl_Obj *obj;
    if ($1.getValue() == odb::dbOnOffType::Value::ON) {
        obj = Tcl_NewStringObj("true", -1);
    } else if ($1.getValue() == odb::dbOnOffType::Value::OFF) {
        obj = Tcl_NewStringObj("false", -1);
    }
    Tcl_SetObjResult(interp, obj);
}

%typemap(out) odb::dbTechLayerType, dbTechLayerType {
    Tcl_Obj *obj;
    if ($1.getValue() == odb::dbTechLayerType::Value::ROUTING) {
        obj = Tcl_NewStringObj("ROUTING", -1);
    } else if ($1.getValue() == odb::dbTechLayerType::Value::CUT) {
        obj = Tcl_NewStringObj("CUT", -1);
    } else if ($1.getValue() == odb::dbTechLayerType::Value::MASTERSLICE) {
        obj = Tcl_NewStringObj("MASTERSLICE", -1);
    } else if ($1.getValue() == odb::dbTechLayerType::Value::OVERLAP) {
        obj = Tcl_NewStringObj("OVERLAP", -1);
    } else if ($1.getValue() == odb::dbTechLayerType::Value::IMPLANT) {
        obj = Tcl_NewStringObj("IMPLANT", -1);
    } else if ($1.getValue() == odb::dbTechLayerType::Value::NONE) {
        obj = Tcl_NewStringObj("NONE", -1);
    }
    Tcl_SetObjResult(interp, obj);
}

%typemap(out) odb::dbTechLayerDir, dbTechLayerDir {
    Tcl_Obj *obj;
    if ($1.getValue() == odb::dbTechLayerDir::Value::NONE) {
        obj = Tcl_NewStringObj("NONE", -1);
    } else if ($1.getValue() == odb::dbTechLayerDir::Value::HORIZONTAL) {
        obj = Tcl_NewStringObj("HORIZONTAL", -1);
    } else if ($1.getValue() == odb::dbTechLayerDir::Value::VERTICAL) {
        obj = Tcl_NewStringObj("VERTICAL", -1);
    }
    Tcl_SetObjResult(interp, obj);
}

%typemap(out) odb::adsPoint, adsPoint {
    Tcl_Obj *list = Tcl_NewListObj(0, nullptr);
    Tcl_Obj *x = Tcl_NewIntObj($1.getX());
    Tcl_Obj *y = Tcl_NewIntObj($1.getY());
    Tcl_ListObjAppendElement(interp, list, x);
    Tcl_ListObjAppendElement(interp, list, y);
    Tcl_SetObjResult(interp, list);
}


// Wrapper for dbSet, dbVector...etc
%define WRAP_DB_CONTAINER(T) 
%typemap(out) dbSet< T >, dbVector< T >{
    Tcl_Obj *list = Tcl_NewListObj(0, nullptr);
    swig_type_info *tf = SWIG_TypeQuery("T" "*");
    for (dbSet< T >::iterator itr = $1.begin(); itr != $1.end(); ++itr)
    {
        Tcl_Obj *obj = SWIG_NewInstanceObj(*itr, tf, 0);
        Tcl_ListObjAppendElement(interp, list, obj);
    }
    Tcl_SetObjResult(interp, list);
}
%enddef

%define WRAP_OBJECT_RETURN_REF(T, A) 
%typemap(in, numinputs=0) T &OUTPUT (T temp) {
   $1 = new T(temp);
}

%typemap(argout) T &OUTPUT {
  swig_type_info *tf = SWIG_TypeQuery("T" "*");
  Tcl_Obj *obj = SWIG_NewInstanceObj($1, tf, 0);
  Tcl_ListObjAppendElement(interp, Tcl_GetObjResult(interp), obj);
}

%apply T &OUTPUT { T & A };

%enddef





// Handle return by ref.
%apply int &OUTPUT { int & overhang1, int & overhang2 };
%apply int &OUTPUT { int & x_spacing, int & y_spacing };
WRAP_OBJECT_RETURN_REF(odb::adsRect, r)


// Wrap containers
WRAP_DB_CONTAINER(odb::dbProperty)
WRAP_DB_CONTAINER(odb::dbLib)
WRAP_DB_CONTAINER(odb::dbChip)
WRAP_DB_CONTAINER(odb::dbBlock)
WRAP_DB_CONTAINER(odb::dbBTerm)
WRAP_DB_CONTAINER(odb::dbITerm)
WRAP_DB_CONTAINER(odb::dbInst)
WRAP_DB_CONTAINER(odb::dbObstruction)
WRAP_DB_CONTAINER(odb::dbBlockage)
WRAP_DB_CONTAINER(odb::dbNet)
WRAP_DB_CONTAINER(odb::dbCapNode)
WRAP_DB_CONTAINER(odb::dbRSeg)
WRAP_DB_CONTAINER(odb::dbVia)
WRAP_DB_CONTAINER(odb::dbTrackGrid)
WRAP_DB_CONTAINER(odb::dbRow)
WRAP_DB_CONTAINER(odb::dbCCSeg)
WRAP_DB_CONTAINER(odb::dbRegion)
WRAP_DB_CONTAINER(odb::dbTechNonDefaultRule)
WRAP_DB_CONTAINER(odb::dbBPin)
WRAP_DB_CONTAINER(odb::dbSWire)
WRAP_DB_CONTAINER(odb::dbBox)
WRAP_DB_CONTAINER(odb::dbSBox)
WRAP_DB_CONTAINER(odb::dbMaster)
WRAP_DB_CONTAINER(odb::dbSite)
WRAP_DB_CONTAINER(odb::dbMTerm)
WRAP_DB_CONTAINER(odb::dbMPin)
WRAP_DB_CONTAINER(odb::dbTarget)
WRAP_DB_CONTAINER(odb::dbTechLayer)
WRAP_DB_CONTAINER(odb::dbTechVia)
WRAP_DB_CONTAINER(odb::dbTechViaRule)
WRAP_DB_CONTAINER(odb::dbTechViaGenerateRule)
WRAP_DB_CONTAINER(odb::dbTechLayerSpacingRule)
WRAP_DB_CONTAINER(odb::dbTechV55InfluenceEntry)
WRAP_DB_CONTAINER(odb::dbTechMinCutRule)
WRAP_DB_CONTAINER(odb::dbTechMinEncRule)
