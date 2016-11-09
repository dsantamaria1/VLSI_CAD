#include <string>
#include <iostream>
#include <vector>

#include "Cell.h"

using namespace std;


Cell::Cell(const string& name, const string& type, 
		int x, int y, const string& fixed) {
	_name = name;
	_type = type;
	_x = x;
	_y = y;
	_fixed = fixed;
	_netNames = vector<string>();
}

string Cell::getName () { return _name; }

string Cell::getType () { return _type; }
	
int Cell::getX () { return _x; }

int Cell::getY () { return _y; }

bool Cell::isFixed () { 
	if (_fixed == "F") { return true; }
	else { return false; }
}

void Cell::addNets (vector<string> netNames) {
	_netNames = netNames;	
}


void Cell::printData () {
	cout << "Cell {" << endl;	
	cout << "\t" << "name = " << getName() << endl;
	cout << "\t" << "type = " << getType() << endl;
	cout << "\t" << "x = " << getX() << endl;
	cout << "\t" << "y = " << getY() << endl;
	cout << "\t" << "fixed = " << isFixed() << endl;
	cout << "\t" << "neighbors = []" << endl;
	cout << "}" << endl;
}
