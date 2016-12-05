#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>

#include "Net.h"
#include "Cell.h"

using namespace std;

Net::Net (string name, vector<string> cellNames) {
	_name = name;
	_cellNames = cellNames;
}


Net::Net (const Net& net) {
	_name = net._name;
	_x_min = net._x_min;
	_x_max = net._x_max;
	_y_min = net._y_min;
	_y_max = net._y_max;
	_cellNames = net._cellNames;
}

string Net::getName () {
	return _name;
}

int Net::getXMin () {
	return _x_min;
}

int Net::getXMax () {
	return _x_max;
}

int Net::getYMin () {
	return _y_min;
}

int Net::getYMax () {
	return _y_max;
}
vector<string> Net::getCellNames () {
	return _cellNames;
}

void Net::setBoundingBox (int x_min, int x_max, int y_min, int y_max) {
	_x_min = x_min;
	_x_max = x_max;
	_y_min = y_min;
	_y_max = y_max;

}


void Net::findBoundingBox (unordered_map<string, Cell>* cellMap) {
	int x_min = INT_MAX;
	int x_max = 0;
	int y_min = INT_MAX;
	int y_max = 0;

	// cout << "\n[" <<  _name << "]\n";
	for (auto it = _cellNames.begin(); it != _cellNames.end(); ++it) {
		string cellName = (*it);
		int x = (*cellMap)[cellName].getX();
		int y = (*cellMap)[cellName].getY();

		x_min = (x < x_min) ? x : x_min;
		x_max = (x > x_max) ? x : x_max;
		y_min = (y < y_min) ? y : y_min;
		y_max = (y > y_max) ? y : y_max;

	}

	_x_min = x_min;
	_x_max = x_max;
	_y_min = y_min;
	_y_max = y_max;
}


// Sum the HPWL from each net in the netlist
int Net::getHPWL () {
	int	HPWL = abs((_x_max - _x_min) + (_y_max - _y_min));
	return HPWL;	
}

ostream& operator<< (ostream& os, const Net& net) {
	os << "{" << net._name;
	vector<string> cellNames = net._cellNames;
	if ( cellNames.empty() ) { os << ", []}"; }
	else {
		os << ", [";
		for (int i = 0; i < cellNames.size(); i++) {
			os << cellNames[i];
			if ((i+1) != cellNames.size()) { os << ", "; }	
		}
		os << "]}";
	}
	return os;
}
