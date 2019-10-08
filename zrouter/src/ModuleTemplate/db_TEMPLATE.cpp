#include <stdio.h>
#include "db.h"
#include "TEMPLATE.h"

BEGIN_NAMESPACE_ADS
static const char* defOrient(dbOrientType orient)
{
	switch (orient.getValue())
	{
	case dbOrientType::R0:
		return "N";

	case dbOrientType::R90:
		return "W";

	case dbOrientType::R180:
		return "S";

	case dbOrientType::R270:
		return "E";

	case dbOrientType::MY:
		return "FN";

	case dbOrientType::MYR90:
		return "FE";

	case dbOrientType::MX:
		return "FS";

	case dbOrientType::MXR90:
		return "FW";
	}

	return "N";
}
static const char* defSigType(dbSigType type)
{
	return type.getString();
}
static const char* defIoType(dbIoType type)
{
	return type.getString();
}


bool TEMPLATE::writeBlock(dbBlock* block, const char* def_file)
{
	
	_dist_factor = (double)block->getDefUnits() / (double)block->getDbUnitsPerMicron();
	_out = fopen(def_file, "w");

	if (_out == NULL)
	{
		notice(0, "Cannot open file (%s) for writing\n", def_file);
		return false;
	}

	//fprintf( _out, "VERSION 5.5 ;\n");
	// fprintf(_out, "VERSION 5.3 ;\n");
	fprintf(_out, "NAMESCASESENSITIVE ON ;\n");
	char hd = block->getHierarchyDelimeter();

	if (hd == 0)
		hd = '|';

	fprintf(_out, "DIVIDERCHAR \"%c\" ;\n", hd);

	char left_bus, right_bus;
	block->getBusDelimeters(left_bus, right_bus);

	if ((left_bus == 0) || (right_bus == 0))
	{
		left_bus = '[';
		right_bus = ']';
	}
	fprintf(_out, "BUSBITCHARS \"%c%c\" ;\n", left_bus, right_bus);

	dbString bname = block->getName();
	fprintf(_out, "DESIGN %s ;\n", bname.c_str());

	fprintf(_out, "UNITS DISTANCE MICRONS %d ;\n", block->getDefUnits());

	adsRect r;
	block->getDieArea(r);

	int x1 = defdist(r.xMin());
	int y1 = defdist(r.yMin());
	int x2 = defdist(r.xMax());
	int y2 = defdist(r.yMax());

	if ((x1 != 0) || (y1 != 0) || (x2 != 0) || (y2 != 0))
		fprintf(_out, "DIEAREA ( %d %d ) ( %d %d ) ;\n", x1, y1, x2, y2);

	writeRows(block);
	writeInsts(block);
	writeBlockages(block);
	
	fprintf(_out, "END DESIGN\n");
	fclose(_out);

	return true;
}
void TEMPLATE::writeRows(dbBlock* block)
{
	dbSet<dbRow> rows = block->getRows();
	dbSet<dbRow>::iterator itr;

	for (itr = rows.begin(); itr != rows.end(); ++itr)
	{
		dbRow* row = *itr;
		

		dbString n;
		int x, y, s, c;
		n = row->getName();
		row->getOrigin(x, y);
		s = row->getSpacing();
		c = row->getSiteCount();
		dbSite* site = row->getSite();
		dbString sn = site->getName();
		const char* o = defOrient(row->getOrient());

		fprintf(_out, "ROW %s %s %d %d %s ", n.c_str(), sn.c_str(), defdist(x), defdist(y), o);
		/*
		adsRect box;
		row->getBBox(box);
		fprintf(_out, " BOX: %d %d %d %d  ", box.xMin(), box.yMin(), box.xMax(), box.yMax(),
		*/
		if (row->getDirection() == dbRowDir::VERTICAL)
			fprintf(_out, "DO 1 BY %d STEP 0 %d", c, defdist(s));
		else
			fprintf(_out, "DO %d BY 1 STEP %d 0", c, defdist(s));
		/*
		if (hasProperties(row, ROW))
		{
			fprintf(_out, " + PROPERTY ");
			writeProperties(row);
		}
		*/
		fprintf(_out, " ;\n");
	}
}
void TEMPLATE::writeInsts(dbBlock* block)
{
	dbSet<dbInst> insts = block->getInsts();

	fprintf(_out, "\nCOMPONENTS %u ;\n", insts.size());

	dbSet<dbInst>::iterator itr;

	for (itr = insts.begin(); itr != insts.end(); ++itr)
	{
		dbInst* inst = *itr;
		writeInst(inst);
		fprintf(_out, "  iterms: \n");
		writeIterms(inst);
	}

	fprintf(_out, "END COMPONENTS\n");
}
void TEMPLATE::writeInst(dbInst* inst)
{
	dbString iname = inst->getName();
	dbMaster* master = inst->getMaster();
	dbString mname = master->getName();

	dbBox* ibox = inst->getBBox();

	fprintf(_out, "\nCOMP: %s %s", iname.c_str(), mname.c_str());
	fprintf(_out, "   box: %d %d %d %d  dir=%d DX=%d DY=%d \n", ibox->xMin(), ibox->yMin(), ibox->xMax(), ibox->yMax(), ibox->getDir(), ibox->getDX(), ibox->getDY());
	/*
	fprintf(_out, "    ");
	dbSourceType source = inst->getSourceType();

	switch (source.getValue())
	{
	case dbSourceType::NONE:
		break;

	case dbSourceType::NETLIST:
		fprintf(_out, " + SOURCE NETLIST");
		break;

	case dbSourceType::DIST:
		fprintf(_out, " + SOURCE DIST");
		break;

	case dbSourceType::USER:
		fprintf(_out, " + SOURCE USER");
		break;

	case dbSourceType::TIMING:
		fprintf(_out, " + SOURCE TIMING");
		break;

	case dbSourceType::TEST:
		break;
	}
	*/
	int x, y;
	inst->getLocation(x, y);
	x = defdist(x);
	y = defdist(y);

	const char* orient = defOrient(inst->getOrient());
	dbPlacementStatus status = inst->getPlacementStatus();

	switch (status.getValue())
	{
	case dbPlacementStatus::NONE:
		break;

	case dbPlacementStatus::UNPLACED:
	{
		fprintf(_out, " + UNPLACED");
		break;
	}

	case dbPlacementStatus::SUGGESTED:
	case dbPlacementStatus::PLACED:
	{
		fprintf(_out, " + PLACED ( %d %d ) %s", x, y, orient);
		break;
	}

	case dbPlacementStatus::LOCKED:
	case dbPlacementStatus::FIRM:
	{
		fprintf(_out, " + FIXED ( %d %d ) %s", x, y, orient);
		break;
	}

	case dbPlacementStatus::COVER:
	{
		fprintf(_out, " + COVER ( %d %d ) %s", x, y, orient);
		break;
	}
	}

	
	// ibox->create
	if (inst->getWeight() != 0)
		fprintf(_out, " + WEIGHT %d", inst->getWeight());

	dbRegion* region = inst->getRegion();

	if (region)
	{
		if (!region->getBoundaries().empty())
		{
			dbString rname = region->getName();
			fprintf(_out, " + REGION %s", rname.c_str());
		}
	}

	fprintf(_out, " \n");
}
void TEMPLATE::writeIterm(dbITerm* iterm, char * tab)
{

	if (iterm->isSpecial())
		return;

	dbNet* net = iterm->getNet();
	if (net==NULL)
		return;
	dbInst* inst = iterm->getInst();
	dbMTerm* mterm = iterm->getMTerm();

	char ttname[256];
	char* mtname = mterm->getName(inst, &ttname[0]);


	fprintf(_out, "%s %s/%s net= %s\n", tab, inst->getConstName(), mtname, net->getConstName());
}
void TEMPLATE::writeIterms(dbInst* inst)
{
	dbSet<dbITerm> iterms = inst->getITerms();

	char ttname[256];
	int i = 0;
	dbSet<dbITerm>::iterator iterm_itr;

	for (iterm_itr = iterms.begin(); iterm_itr != iterms.end(); ++iterm_itr)
	{
		dbITerm* iterm = *iterm_itr;
		writeIterm(iterm, "\t");

		dbNet* net = iterm->getNet();
		if (net==NULL)
			continue;

		dbSet<dbITerm> net_terms = net->getITerms();
		dbSet<dbITerm>::iterator net_term_itr;

		for (net_term_itr = net_terms.begin(); net_term_itr != net_terms.end(); ++net_term_itr)
		{
			dbITerm* iterm1 = *net_term_itr;
			if (iterm==iterm1)
				continue;

			writeIterm(iterm1, "\t\t");
		}
		writeBTerms(net);
	}
}
void TEMPLATE::writeBTerms(dbNet* net)
{
	dbSet<dbBTerm> bterms = net->getBTerms();

	if (bterms.size() == 0)
		return;

	uint n = 0;

	dbSet<dbBTerm>::iterator itr;
/*
	for (itr = bterms.begin(); itr != bterms.end(); ++itr)
	{
		dbBTerm* bterm = *itr;
		dbNet* net = bterm->getNet();

		dbSet<dbBPin> bpins = bterm->getBPins();
		uint pcnt = bpins.size();

		if (pcnt == 0)
			n += 1;
		else
			n += pcnt;
	}

	fprintf(_out, "PINS %u ;\n", n);
*/

	for (itr = bterms.begin(); itr != bterms.end(); ++itr)
	{
		dbBTerm* bterm = *itr;
		dbNet* net = bterm->getNet();
		writeBTerm(bterm);
	}

//	fprintf(_out, "END PINS\n");
}
void TEMPLATE::writeBTerm(dbBTerm* bterm)
{

	dbSet<dbBPin> bpins = bterm->getBPins();

	if (bpins.size() != 0)
	{
		int cnt = 0;

		dbSet<dbBPin>::iterator itr;

		for (itr = bpins.begin(); itr != bpins.end(); ++itr)
			writeBPin(*itr, cnt++);

		return;
	}

	dbNet* net = bterm->getNet();
	dbString bname = bterm->getName();

	dbString nname = net->getName();
	fprintf(_out, "\t\t%s (Bterm) net=%s", bname.c_str(), nname.c_str());

	if (bterm->isSpecial())
		fprintf(_out, " + SPECIAL");

	fprintf(_out, " + DIRECTION %s", defIoType(bterm->getIoType()));
/*
	if (_version >= defout::DEF_5_6)
	{
		dbBTerm* supply = bterm->getSupplyPin();

		if (supply)
		{
			dbString pname = supply->getName();
			fprintf(_out, " + SUPPLYSENSITIVITY %s", pname.c_str());
		}

		dbBTerm* ground = bterm->getGroundPin();

		if (ground)
		{
			dbString pname = ground->getName();
			fprintf(_out, " + GROUNDSENSITIVITY %s", pname.c_str());
		}

	}
*/
	fprintf(_out, " + USE %s", defSigType(bterm->getSigType()));
	fprintf(_out, " ;\n");
}
void TEMPLATE::writeBlockages(dbBlock* block)
{
	dbSet<dbObstruction> obstructions = block->getObstructions();
	dbSet<dbBlockage> blockages = block->getBlockages();

	int bcnt = obstructions.size() + blockages.size();

	if (bcnt == 0)
		return;

	bool first = true;

	dbSet<dbObstruction>::iterator obs_itr;

	for (obs_itr = obstructions.begin(); obs_itr != obstructions.end(); ++obs_itr)
	{
		dbObstruction* obs = *obs_itr;
		dbInst* inst = obs->getInstance();

		if (first) {
			first = false;
			fprintf(_out, "BLOCKAGES %d ;\n", bcnt);
		}

		dbBox* bbox = obs->getBBox();
		dbTechLayer* layer = bbox->getTechLayer();
		dbString lname;
		lname = layer->getName();

		fprintf(_out, "    - LAYER %s", lname.c_str());

		if (inst)
		{
			dbString iname = inst->getName();
			fprintf(_out, " + COMPONENT %s", iname.c_str());
		}

		if (obs->isSlotObstruction())
			fprintf(_out, " + SLOTS");

		if (obs->isFillObstruction())
			fprintf(_out, " + FILLS");

		if (obs->isPushedDown())
			fprintf(_out, " + PUSHDOWN");
		/*
		if (_version >= defout::DEF_5_6)
		{
			if (obs->hasEffectiveWidth())
			{
				int w = defdist(obs->getEffectiveWidth());
				fprintf(_out, " + DESIGNRULEWIDTH %d", w);
			}
			else if (obs->hasMinSpacing())
			{
				int s = defdist(obs->getMinSpacing());
				fprintf(_out, " + SPACING %d", s);
			}
		}

		int x1 = defdist(bbox->xMin());
		int y1 = defdist(bbox->yMin());
		int x2 = defdist(bbox->xMax());
		int y2 = defdist(bbox->yMax());
		*/
		int x1 = bbox->xMin();
		int y1 = bbox->yMin();
		int x2 = bbox->xMax();
		int y2 = bbox->yMax();
		fprintf(_out, " RECT ( %d %d ) ( %d %d ) ;\n", x1, y1, x2, y2);
	}

	dbSet<dbBlockage>::iterator blk_itr;

	for (blk_itr = blockages.begin(); blk_itr != blockages.end(); ++blk_itr)
	{
		dbBlockage* blk = *blk_itr;
		dbInst* inst = blk->getInstance();

		if (first) {
			first = false;
			fprintf(_out, "BLOCKAGES %d ;\n", bcnt);
		}

		fprintf(_out, "    - PLACEMENT");

		dbString iname = inst->getName();
		fprintf(_out, " + COMPONENT %s", iname.c_str());

		if (blk->isPushedDown())
			fprintf(_out, " + PUSHDOWN");

		dbBox* bbox = blk->getBBox();
		int x1 = defdist(bbox->xMin());
		int y1 = defdist(bbox->yMin());
		int x2 = defdist(bbox->xMax());
		int y2 = defdist(bbox->yMax());

		fprintf(_out, " RECT ( %d %d ) ( %d %d ) ;\n", x1, y1, x2, y2);
	}

	if (!first)
		fprintf(_out, "END BLOCKAGES\n");
}
void TEMPLATE::writeBPin(dbBPin* bpin, int cnt)
{
	dbBTerm* bterm = bpin->getBTerm();
	dbNet* net = bterm->getNet();
	dbString bname = bterm->getName();

	dbString nname = net->getName();
	if (cnt == 0)
		fprintf(_out, "    - %s + NET %s", bname.c_str(), nname.c_str());
	else
		fprintf(_out, "    - %s.extra%d + NET %s", bname.c_str(), cnt, nname.c_str());

	if (bterm->isSpecial())
		fprintf(_out, " + SPECIAL");

	fprintf(_out, " + DIRECTION %s", defIoType(bterm->getIoType()));
/*
	if (_version >= defout::DEF_5_6)
	{
		dbBTerm* supply = bterm->getSupplyPin();

		if (supply)
		{
			dbString pname = supply->getName();
			fprintf(_out, " + SUPPLYSENSITIVITY %s", pname.c_str());
		}

		dbBTerm* ground = bterm->getGroundPin();

		if (ground)
		{
			dbString pname = ground->getName();
			fprintf(_out, " + GROUNDSENSITIVITY %s", pname.c_str());
		}

	}
*/
	fprintf(_out, " + USE %s", defSigType(bterm->getSigType()));

	dbBox* box = bpin->getBox();

	if (box)
	{
		int dw = defdist(int(box->getDX() / 2));
		int dh = defdist(int(box->getDY() / 2));
		int x = defdist(box->xMin()) + dw;
		int y = defdist(box->yMin()) + dh;

		dbPlacementStatus status = bpin->getPlacementStatus();

		switch (status.getValue())
		{
		case dbPlacementStatus::NONE:
		case dbPlacementStatus::UNPLACED:
			break;

		case dbPlacementStatus::SUGGESTED:
		case dbPlacementStatus::PLACED:
		{
			fprintf(_out, " + PLACED ( %d %d ) N", x, y);
			break;
		}

		case dbPlacementStatus::LOCKED:
		case dbPlacementStatus::FIRM:
		{
			fprintf(_out, " + FIXED ( %d %d ) N", x, y);
			break;
		}

		case dbPlacementStatus::COVER:
		{
			fprintf(_out, " + COVER ( %d %d ) N", x, y);
			break;
		}
		}

		dbTechLayer* layer = box->getTechLayer();
		dbString lname;

		lname = layer->getName();
/*
		if (_version == defout::DEF_5_5)
			fprintf(_out, " + LAYER %s ( %d %d ) ( %d %d )", lname.c_str(), -dw, -dh, dw, dh);
		else
		{
			if (bpin->hasEffectiveWidth())
			{
				int w = defdist(bpin->getEffectiveWidth());
				fprintf(_out, " + LAYER %s DESIGNRULEWIDTH %d ( %d %d ) ( %d %d )", lname.c_str(), w, -dw, -dh, dw, dh);
			}
			else if (bpin->hasMinSpacing())
			{
				int s = defdist(bpin->getMinSpacing());
				fprintf(_out, " + LAYER %s SPACING %d ( %d %d ) ( %d %d )", lname.c_str(), s, -dw, -dh, dw, dh);
			}
			else
			{
				fprintf(_out, " + LAYER %s ( %d %d ) ( %d %d )", lname.c_str(), -dw, -dh, dw, dh);
			}
		}
*/
	}

	fprintf(_out, " ;\n");
}
END_NAMESPACE_ADS
