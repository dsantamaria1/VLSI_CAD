#include <string>
#include <iostream>
#include <vector>

#include "Cell.h"

using namespace std;


Cell::Cell(const string& name, const string& type, 
		int x, int y, const string& fixed) :
	_name (name),
	_type (type),
	_x (x),
	_y (y),
	_fixed (fixed)
{}

string Cell::getName () { return _name; }

string Cell::getType () { return _type; }
	
int Cell::getX () { return _x; }

int Cell::getY () { return _y; }

bool Cell::isFixed () { 
	if (_fixed == "F") { return true; }
	else { return false; }
}

void Cell::addNeighbors (vector <Cell*> neighbors) {
	
}

//vector <Cell*> Cell::getNeighbors () {
//	
//}


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

/*
int main () {
	string name = "cell1";
	string type = "CLB";
	int x = 0;
	int y = 0;
	string fixed = "M";
	Cell cell1("cell1", type, x, y, fixed);
	Cell cell2("cell2", type, x, y, fixed);
	

	cell1.printData();
}
*/
