%template(vector_str) std::vector<std::string>;

%typemap(out) odb::dbString {
    Tcl_Obj *obj = Tcl_NewStringObj($1.c_str(), $1.length());
    Tcl_SetObjResult(interp, obj);
}
%typemap(out) odb::dbStringProperty {
    Tcl_Obj *obj = Tcl_NewStringObj($1.getValue().c_str(), $1.getValue().length());
    Tcl_SetObjResult(interp, obj);
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
