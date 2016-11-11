#include <string>
#include <vector>
#include <iostream>

#include "Net.h"

using namespace std;

Net::Net (string name, vector<string> cellNames) {
	_name = name;
	_cellNames = cellNames;
}


Net::Net (const Net& net) {
	_name = net._name;
	_cellNames = net._cellNames;
}


vector<string> Net::getCellNames () {
	return _cellNames;
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

