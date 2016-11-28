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

int Net::calculateHPWL (){
	int HPWL = 0;
	int x_min = _xList[0];
	int x_max = _xList[_xList.size()-1];
	int y_min = _yList[0];
	int y_max = _yList[_yList.size()-1];

	HPWL = (x_max - x_min) + (y_max - y_min);

	return HPWL;
}

void Net::sortXList(){
	sort(_xList.begin(), _xList.end());
}

int Net::getXMin(){
	return _xList[0];
}

int Net::getXMax(){
	return _xList[_xList.size()-1];
}

vector<int> Net::getXList(){
	return _xList;
}

void Net::buildXList(unordered_map<string, Cell>* cellMap){
	_xList.clear();
	for(int i=0; i<_cellNames.size(); i++){
		int x = (*cellMap)[_cellNames[i]].getX();
		_xList.push_back(x);
	}
	sort(_xList.begin(), _xList.end());
}

void Net::buildYList(unordered_map<string, Cell>* cellMap){
	_yList.clear();
	for(int i=0; i<_cellNames.size(); i++){
		int y = (*cellMap)[_cellNames[i]].getY();
		_yList.push_back(y);
	}
	sort(_yList.begin(), _yList.end());
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
