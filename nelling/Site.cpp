#include <string>
#include <vector>
#include <iostream>

#include "Site.h"
#include "Cell.h"

using namespace std;


Site::Site (int x, int y, const string& type) {
	_x = x;
	_y = y;
	_type = type;
	_cellName = "";
}

Site::Site (const Site& site) {
	_x = site._x;
	_y = site._y;
	_type = site._type;
	_cellName = site._cellName;
}

void Site::addCell (string cellName) {
	_cellName = cellName; 	
}

string Site::getCellName () {
	return _cellName;
}


ostream& operator<< (ostream& os, const Site& site) {
	os << "{" << site._x << ", " << site._y << ", " << site._type;
	if (site._cellName != "") { os << ", " << site._cellName << "}"; }
	else { os << ", (empty)}"; }
	return os;
}

