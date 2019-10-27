
%typemap(out) odb::dbOrientType, dbOrientType {
	Tcl_Obj *obj;
	if ($1.getValue() == odb::dbOrientType::Value::R0) {
		obj = Tcl_NewStringObj("R0", -1);
	} else if ($1.getValue() == odb::dbOrientType::Value::R90) {
		obj = Tcl_NewStringObj("R90", -1);
	} else if ($1.getValue() == odb::dbOrientType::Value::R180) {
		obj = Tcl_NewStringObj("R180", -1);
	} else if ($1.getValue() == odb::dbOrientType::Value::R270) {
		obj = Tcl_NewStringObj("R270", -1);
	} else if ($1.getValue() == odb::dbOrientType::Value::MY) {
		obj = Tcl_NewStringObj("MY", -1);
	} else if ($1.getValue() == odb::dbOrientType::Value::MYR90) {
		obj = Tcl_NewStringObj("MYR90", -1);
	} else if ($1.getValue() == odb::dbOrientType::Value::MX) {
		obj = Tcl_NewStringObj("MX", -1);
	} else if ($1.getValue() == odb::dbOrientType::Value::MXR90) {
		obj = Tcl_NewStringObj("MXR90", -1);
	}
	Tcl_SetObjResult(interp, obj);
}
%typemap(out) odb::dbSigType, dbSigType {
	Tcl_Obj *obj;
	if ($1.getValue() == odb::dbSigType::Value::SIGNAL) {
		obj = Tcl_NewStringObj("SIGNAL", -1);
	} else if ($1.getValue() == odb::dbSigType::Value::POWER) {
		obj = Tcl_NewStringObj("POWER", -1);
	} else if ($1.getValue() == odb::dbSigType::Value::GROUND) {
		obj = Tcl_NewStringObj("GROUND", -1);
	} else if ($1.getValue() == odb::dbSigType::Value::CLOCK) {
		obj = Tcl_NewStringObj("CLOCK", -1);
	} else if ($1.getValue() == odb::dbSigType::Value::ANALOG) {
		obj = Tcl_NewStringObj("ANALOG", -1);
	} else if ($1.getValue() == odb::dbSigType::Value::RESET) {
		obj = Tcl_NewStringObj("RESET", -1);
	} else if ($1.getValue() == odb::dbSigType::Value::SCAN) {
		obj = Tcl_NewStringObj("SCAN", -1);
	} else if ($1.getValue() == odb::dbSigType::Value::TIEOFF) {
		obj = Tcl_NewStringObj("TIEOFF", -1);
	}
	Tcl_SetObjResult(interp, obj);
}
%typemap(out) odb::dbIoType, dbIoType {
	Tcl_Obj *obj;
	if ($1.getValue() == odb::dbIoType::Value::INPUT) {
		obj = Tcl_NewStringObj("INPUT", -1);
	} else if ($1.getValue() == odb::dbIoType::Value::OUTPUT) {
		obj = Tcl_NewStringObj("OUTPUT", -1);
	} else if ($1.getValue() == odb::dbIoType::Value::INOUT) {
		obj = Tcl_NewStringObj("INOUT", -1);
	} else if ($1.getValue() == odb::dbIoType::Value::FEEDTHRU) {
		obj = Tcl_NewStringObj("FEEDTHRU", -1);
	}
	Tcl_SetObjResult(interp, obj);
}
%typemap(out) odb::dbPlacementStatus, dbPlacementStatus {
	Tcl_Obj *obj;
	if ($1.getValue() == odb::dbPlacementStatus::Value::NONE) {
		obj = Tcl_NewStringObj("NONE", -1);
	} else if ($1.getValue() == odb::dbPlacementStatus::Value::UNPLACED) {
		obj = Tcl_NewStringObj("UNPLACED", -1);
	} else if ($1.getValue() == odb::dbPlacementStatus::Value::SUGGESTED) {
		obj = Tcl_NewStringObj("SUGGESTED", -1);
	} else if ($1.getValue() == odb::dbPlacementStatus::Value::PLACED) {
		obj = Tcl_NewStringObj("PLACED", -1);
	} else if ($1.getValue() == odb::dbPlacementStatus::Value::LOCKED) {
		obj = Tcl_NewStringObj("LOCKED", -1);
	} else if ($1.getValue() == odb::dbPlacementStatus::Value::FIRM) {
		obj = Tcl_NewStringObj("FIRM", -1);
	} else if ($1.getValue() == odb::dbPlacementStatus::Value::COVER) {
		obj = Tcl_NewStringObj("COVER", -1);
	}
	Tcl_SetObjResult(interp, obj);
}
%typemap(out) odb::dbMasterType, dbMasterType {
	Tcl_Obj *obj;
	if ($1.getValue() == odb::dbMasterType::Value::NONE) {
		obj = Tcl_NewStringObj("NONE", -1);
	} else if ($1.getValue() == odb::dbMasterType::Value::COVER) {
		obj = Tcl_NewStringObj("COVER", -1);
	} else if ($1.getValue() == odb::dbMasterType::Value::RING) {
		obj = Tcl_NewStringObj("RING", -1);
	} else if ($1.getValue() == odb::dbMasterType::Value::BLOCK) {
		obj = Tcl_NewStringObj("BLOCK", -1);
	} else if ($1.getValue() == odb::dbMasterType::Value::PAD) {
		obj = Tcl_NewStringObj("PAD", -1);
	} else if ($1.getValue() == odb::dbMasterType::Value::PAD_INPUT) {
		obj = Tcl_NewStringObj("PAD_INPUT", -1);
	} else if ($1.getValue() == odb::dbMasterType::Value::PAD_OUTPUT) {
		obj = Tcl_NewStringObj("PAD_OUTPUT", -1);
	} else if ($1.getValue() == odb::dbMasterType::Value::PAD_INOUT) {
		obj = Tcl_NewStringObj("PAD_INOUT", -1);
	} else if ($1.getValue() == odb::dbMasterType::Value::PAD_POWER) {
		obj = Tcl_NewStringObj("PAD_POWER", -1);
	} else if ($1.getValue() == odb::dbMasterType::Value::PAD_SPACER) {
		obj = Tcl_NewStringObj("PAD_SPACER", -1);
	} else if ($1.getValue() == odb::dbMasterType::Value::CORE) {
		obj = Tcl_NewStringObj("CORE", -1);
	} else if ($1.getValue() == odb::dbMasterType::Value::CORE_FEEDTHRU) {
		obj = Tcl_NewStringObj("CORE_FEEDTHRU", -1);
	} else if ($1.getValue() == odb::dbMasterType::Value::CORE_TIEHIGH) {
		obj = Tcl_NewStringObj("CORE_TIEHIGH", -1);
	} else if ($1.getValue() == odb::dbMasterType::Value::CORE_TIELOW) {
		obj = Tcl_NewStringObj("CORE_TIELOW", -1);
	} else if ($1.getValue() == odb::dbMasterType::Value::CORE_SPACER) {
		obj = Tcl_NewStringObj("CORE_SPACER", -1);
	} else if ($1.getValue() == odb::dbMasterType::Value::ENDCAP) {
		obj = Tcl_NewStringObj("ENDCAP", -1);
	} else if ($1.getValue() == odb::dbMasterType::Value::ENDCAP_PRE) {
		obj = Tcl_NewStringObj("ENDCAP_PRE", -1);
	} else if ($1.getValue() == odb::dbMasterType::Value::ENDCAP_POST) {
		obj = Tcl_NewStringObj("ENDCAP_POST", -1);
	} else if ($1.getValue() == odb::dbMasterType::Value::ENDCAP_TOPLEFT) {
		obj = Tcl_NewStringObj("ENDCAP_TOPLEFT", -1);
	} else if ($1.getValue() == odb::dbMasterType::Value::ENDCAP_TOPRIGHT) {
		obj = Tcl_NewStringObj("ENDCAP_TOPRIGHT", -1);
	} else if ($1.getValue() == odb::dbMasterType::Value::ENDCAP_BOTTOMLEFT) {
		obj = Tcl_NewStringObj("ENDCAP_BOTTOMLEFT", -1);
	} else if ($1.getValue() == odb::dbMasterType::Value::ENDCAP_BOTTOMRIGHT) {
		obj = Tcl_NewStringObj("ENDCAP_BOTTOMRIGHT", -1);
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
%typemap(out) odb::dbRowDir, dbRowDir {
	Tcl_Obj *obj;
	if ($1.getValue() == odb::dbRowDir::Value::HORIZONTAL) {
		obj = Tcl_NewStringObj("HORIZONTAL", -1);
	} else if ($1.getValue() == odb::dbRowDir::Value::VERTICAL) {
		obj = Tcl_NewStringObj("VERTICAL", -1);
	}
	Tcl_SetObjResult(interp, obj);
}
%typemap(out) odb::dbBoxOwner, dbBoxOwner {
	Tcl_Obj *obj;
	if ($1.getValue() == odb::dbBoxOwner::Value::UNKNOWN) {
		obj = Tcl_NewStringObj("UNKNOWN", -1);
	} else if ($1.getValue() == odb::dbBoxOwner::Value::BLOCK) {
		obj = Tcl_NewStringObj("BLOCK", -1);
	} else if ($1.getValue() == odb::dbBoxOwner::Value::INST) {
		obj = Tcl_NewStringObj("INST", -1);
	} else if ($1.getValue() == odb::dbBoxOwner::Value::BTERM) {
		obj = Tcl_NewStringObj("BTERM", -1);
	} else if ($1.getValue() == odb::dbBoxOwner::Value::VIA) {
		obj = Tcl_NewStringObj("VIA", -1);
	} else if ($1.getValue() == odb::dbBoxOwner::Value::OBSTRUCTION) {
		obj = Tcl_NewStringObj("OBSTRUCTION", -1);
	} else if ($1.getValue() == odb::dbBoxOwner::Value::SWIRE) {
		obj = Tcl_NewStringObj("SWIRE", -1);
	} else if ($1.getValue() == odb::dbBoxOwner::Value::BLOCKAGE) {
		obj = Tcl_NewStringObj("BLOCKAGE", -1);
	} else if ($1.getValue() == odb::dbBoxOwner::Value::MASTER) {
		obj = Tcl_NewStringObj("MASTER", -1);
	} else if ($1.getValue() == odb::dbBoxOwner::Value::MPIN) {
		obj = Tcl_NewStringObj("MPIN", -1);
	} else if ($1.getValue() == odb::dbBoxOwner::Value::TECH_VIA) {
		obj = Tcl_NewStringObj("TECH_VIA", -1);
	} else if ($1.getValue() == odb::dbBoxOwner::Value::REGION) {
		obj = Tcl_NewStringObj("REGION", -1);
	} else if ($1.getValue() == odb::dbBoxOwner::Value::BPIN) {
		obj = Tcl_NewStringObj("BPIN", -1);
	}
	Tcl_SetObjResult(interp, obj);
}
%typemap(out) odb::dbPolygonOwner, dbPolygonOwner {
	Tcl_Obj *obj;
	if ($1.getValue() == odb::dbPolygonOwner::Value::UNKNOWN) {
		obj = Tcl_NewStringObj("UNKNOWN", -1);
	} else if ($1.getValue() == odb::dbPolygonOwner::Value::BPIN) {
		obj = Tcl_NewStringObj("BPIN", -1);
	} else if ($1.getValue() == odb::dbPolygonOwner::Value::OBSTRUCTION) {
		obj = Tcl_NewStringObj("OBSTRUCTION", -1);
	} else if ($1.getValue() == odb::dbPolygonOwner::Value::SWIRE) {
		obj = Tcl_NewStringObj("SWIRE", -1);
	}
	Tcl_SetObjResult(interp, obj);
}
%typemap(out) odb::dbWireType, dbWireType {
	Tcl_Obj *obj;
	if ($1.getValue() == odb::dbWireType::Value::NONE) {
		obj = Tcl_NewStringObj("NONE", -1);
	} else if ($1.getValue() == odb::dbWireType::Value::COVER) {
		obj = Tcl_NewStringObj("COVER", -1);
	} else if ($1.getValue() == odb::dbWireType::Value::FIXED) {
		obj = Tcl_NewStringObj("FIXED", -1);
	} else if ($1.getValue() == odb::dbWireType::Value::ROUTED) {
		obj = Tcl_NewStringObj("ROUTED", -1);
	} else if ($1.getValue() == odb::dbWireType::Value::SHIELD) {
		obj = Tcl_NewStringObj("SHIELD", -1);
	} else if ($1.getValue() == odb::dbWireType::Value::NOSHIELD) {
		obj = Tcl_NewStringObj("NOSHIELD", -1);
	}
	Tcl_SetObjResult(interp, obj);
}
%typemap(out) odb::dbWireShapeType, dbWireShapeType {
	Tcl_Obj *obj;
	if ($1.getValue() == odb::dbWireShapeType::Value::NONE) {
		obj = Tcl_NewStringObj("NONE", -1);
	} else if ($1.getValue() == odb::dbWireShapeType::Value::RING) {
		obj = Tcl_NewStringObj("RING", -1);
	} else if ($1.getValue() == odb::dbWireShapeType::Value::PADRING) {
		obj = Tcl_NewStringObj("PADRING", -1);
	} else if ($1.getValue() == odb::dbWireShapeType::Value::BLOCKRING) {
		obj = Tcl_NewStringObj("BLOCKRING", -1);
	} else if ($1.getValue() == odb::dbWireShapeType::Value::STRIPE) {
		obj = Tcl_NewStringObj("STRIPE", -1);
	} else if ($1.getValue() == odb::dbWireShapeType::Value::FOLLOWPIN) {
		obj = Tcl_NewStringObj("FOLLOWPIN", -1);
	} else if ($1.getValue() == odb::dbWireShapeType::Value::IOWIRE) {
		obj = Tcl_NewStringObj("IOWIRE", -1);
	} else if ($1.getValue() == odb::dbWireShapeType::Value::COREWIRE) {
		obj = Tcl_NewStringObj("COREWIRE", -1);
	} else if ($1.getValue() == odb::dbWireShapeType::Value::BLOCKWIRE) {
		obj = Tcl_NewStringObj("BLOCKWIRE", -1);
	} else if ($1.getValue() == odb::dbWireShapeType::Value::BLOCKAGEWIRE) {
		obj = Tcl_NewStringObj("BLOCKAGEWIRE", -1);
	} else if ($1.getValue() == odb::dbWireShapeType::Value::FILLWIRE) {
		obj = Tcl_NewStringObj("FILLWIRE", -1);
	} else if ($1.getValue() == odb::dbWireShapeType::Value::DRCFILL) {
		obj = Tcl_NewStringObj("DRCFILL", -1);
	}
	Tcl_SetObjResult(interp, obj);
}
%typemap(out) odb::dbSiteClass, dbSiteClass {
	Tcl_Obj *obj;
	if ($1.getValue() == odb::dbSiteClass::Value::NONE) {
		obj = Tcl_NewStringObj("NONE", -1);
	} else if ($1.getValue() == odb::dbSiteClass::Value::PAD) {
		obj = Tcl_NewStringObj("PAD", -1);
	} else if ($1.getValue() == odb::dbSiteClass::Value::CORE) {
		obj = Tcl_NewStringObj("CORE", -1);
	}
	Tcl_SetObjResult(interp, obj);
}
%typemap(out) odb::dbOnOffType, dbOnOffType {
	Tcl_Obj *obj;
	if ($1.getValue() == odb::dbOnOffType::Value::OFF) {
		obj = Tcl_NewStringObj("OFF", -1);
	} else if ($1.getValue() == odb::dbOnOffType::Value::ON) {
		obj = Tcl_NewStringObj("ON", -1);
	}
	Tcl_SetObjResult(interp, obj);
}
%typemap(out) odb::dbClMeasureType, dbClMeasureType {
	Tcl_Obj *obj;
	if ($1.getValue() == odb::dbClMeasureType::Value::EUCLIDEAN) {
		obj = Tcl_NewStringObj("EUCLIDEAN", -1);
	} else if ($1.getValue() == odb::dbClMeasureType::Value::MAXXY) {
		obj = Tcl_NewStringObj("MAXXY", -1);
	}
	Tcl_SetObjResult(interp, obj);
}
%typemap(out) odb::dbJournalEntryType, dbJournalEntryType {
	Tcl_Obj *obj;
	if ($1.getValue() == odb::dbJournalEntryType::Value::NONE) {
		obj = Tcl_NewStringObj("NONE", -1);
	} else if ($1.getValue() == odb::dbJournalEntryType::Value::OWNER) {
		obj = Tcl_NewStringObj("OWNER", -1);
	} else if ($1.getValue() == odb::dbJournalEntryType::Value::ADD) {
		obj = Tcl_NewStringObj("ADD", -1);
	} else if ($1.getValue() == odb::dbJournalEntryType::Value::DESTROY) {
		obj = Tcl_NewStringObj("DESTROY", -1);
	}
	Tcl_SetObjResult(interp, obj);
}
%typemap(out) odb::dbDirection, dbDirection {
	Tcl_Obj *obj;
	if ($1.getValue() == odb::dbDirection::Value::NONE) {
		obj = Tcl_NewStringObj("NONE", -1);
	} else if ($1.getValue() == odb::dbDirection::Value::NORTH) {
		obj = Tcl_NewStringObj("NORTH", -1);
	} else if ($1.getValue() == odb::dbDirection::Value::EAST) {
		obj = Tcl_NewStringObj("EAST", -1);
	} else if ($1.getValue() == odb::dbDirection::Value::SOUTH) {
		obj = Tcl_NewStringObj("SOUTH", -1);
	} else if ($1.getValue() == odb::dbDirection::Value::WEST) {
		obj = Tcl_NewStringObj("WEST", -1);
	}
	Tcl_SetObjResult(interp, obj);
}
%typemap(out) odb::dbRegionType, dbRegionType {
	Tcl_Obj *obj;
	if ($1.getValue() == odb::dbRegionType::Value::INCLUSIVE) {
		obj = Tcl_NewStringObj("INCLUSIVE", -1);
	} else if ($1.getValue() == odb::dbRegionType::Value::EXCLUSIVE) {
		obj = Tcl_NewStringObj("EXCLUSIVE", -1);
	} else if ($1.getValue() == odb::dbRegionType::Value::SUGGESTED) {
		obj = Tcl_NewStringObj("SUGGESTED", -1);
	}
	Tcl_SetObjResult(interp, obj);
}
%typemap(out) odb::dbSourceType, dbSourceType {
	Tcl_Obj *obj;
	if ($1.getValue() == odb::dbSourceType::Value::NONE) {
		obj = Tcl_NewStringObj("NONE", -1);
	} else if ($1.getValue() == odb::dbSourceType::Value::NETLIST) {
		obj = Tcl_NewStringObj("NETLIST", -1);
	} else if ($1.getValue() == odb::dbSourceType::Value::DIST) {
		obj = Tcl_NewStringObj("DIST", -1);
	} else if ($1.getValue() == odb::dbSourceType::Value::USER) {
		obj = Tcl_NewStringObj("USER", -1);
	} else if ($1.getValue() == odb::dbSourceType::Value::TIMING) {
		obj = Tcl_NewStringObj("TIMING", -1);
	} else if ($1.getValue() == odb::dbSourceType::Value::TEST) {
		obj = Tcl_NewStringObj("TEST", -1);
	}
	Tcl_SetObjResult(interp, obj);
}