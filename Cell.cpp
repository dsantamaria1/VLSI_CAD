#include <string>
#include <iostream>
#include <vector>

#include "Cell.h"

using namespace std;


Cell::Cell (const string& name, const string& type,
		int x, int y, const string& fixed) {
	_name = name;
	_type = type;
	_x = x;
	_y = y;
	_fixed = fixed;
	_netNames = vector<string>();
}

Cell::Cell (const Cell& cell) {
	_name = cell._name;
	_type = cell._type;
	_x = cell._x;
	_y = cell._y;
	_fixed = cell._fixed;
	_netNames = cell._netNames;;
}

string Cell::getName () { return _name; }

string Cell::getType () { return _type; }

int Cell::getX () { return _x; }

void Cell::setX (int newX) { _x = newX;}

int Cell::getY () { return _y; }

bool Cell::isFixed () {
	if (_fixed == "F") { return true; }
	else { return false; }
}

void Cell::addNets (vector<string> netNames) {
	_netNames = netNames;
}

void Cell::addNet (string netName) {
	_netNames.emplace_back(netName);
}

vector<string> Cell::getNetNames () {
	return _netNames;
}


ostream& operator <<(ostream& os, const Cell& cell) {
	os << "{" << cell._name << ", " << cell._type << ", " << cell._x << ", "
		<< cell._y << ", " << cell._fixed;
	vector<string> netNames = cell._netNames;
	if ( netNames.empty() ) { os << ", []}"; }
	else {
		os << ", [";
		for (int i = 0; i < netNames.size(); i++) {
			os << netNames[i];
			if ((i+1) != netNames.size()) { os << ", "; }
		}
		os << "]}";
	}

	return os;
}
