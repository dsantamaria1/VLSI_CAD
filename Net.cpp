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
	_cellNames = net._cellNames;
}

string Net::getName () {
	return _name;
}

vector<string> Net::getCellNames () {
	return _cellNames;
}

int Net::calculateHPWL (unordered_map<string, Cell>* cellMap) {
	int HPWL = 0;
	int x_min = INT_MAX; 
	int x_max = 0;
	int y_min = INT_MAX; 
	int y_max = 0;
	
	for (auto it = _cellNames.begin(); it != _cellNames.end(); ++it) {
		string cellName = (*it);
		int x = (*cellMap)[cellName].getX();
		int y = (*cellMap)[cellName].getY();
			
		x_max = (x > x_max) ? x : x_max;
		x_min = (x < x_min) ? x : x_min;
		y_max = (y > y_max) ? y : y_max;
		y_min = (y < y_min) ? y : y_min;
	}
	
	// Sum the HPWL from each net in the netlist
	HPWL = (x_max - x_min) + (y_max - y_min);
		
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

