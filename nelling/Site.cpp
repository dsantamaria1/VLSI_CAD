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

void Site::addCell (string cellName) {
	_cellName = cellName; 	
}


ostream& operator<< (ostream& os, const Site& site) {
	os << "{" << site._x << ", " << site._y << ", " << site._type;
	if (site._cellName != "") { os << ", " << site._cellName << "}"; }
	else { os << ", (empty)}"; }
	return os;
}

