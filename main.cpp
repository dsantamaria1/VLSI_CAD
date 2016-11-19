#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>

#include "Net.h"
#include "Cell.h"
#include "Site.h"
#include "Placement.h"
#include "Parser.h"

#define WINDOW_SIZE 7
using namespace std;


// Helper Functions
int calcHPWL(unordered_map<string, Net>* netMap, unordered_map<string, Cell>* cellMap){

		int hpwl = 0;
		for( auto it = (*netMap).begin(); it != (*netMap).end(); ++it){
				vector<string> cells = (it->second).getCellNames();
				int xMax = 0;
				int yMax = 0;
				int xMin = INT_MAX;
				int yMin = INT_MAX;
				for(int i=0; i<cells.size(); i++){
					int y = (*cellMap)[cells[i]].getX();
					int x = (*cellMap)[cells[i]].getY();
					xMin = (x < xMin) ? x : xMin;
					xMax = (x > xMax) ? x : xMax;
					yMin = (y < yMin) ? y : yMin;
					yMax = (y > yMax) ? y : yMax;
				}
				int hpwl_local = abs(xMax-xMin) + abs(yMax - yMin);
				hpwl += hpwl_local;
		}
		return hpwl;
}

int cost (Cell cell, int x, unordered_map<string, Net>* netMap,
                unordered_map<string, Cell>* cellMap) {
		int totalCost = 0;
    vector<string> netNames = cell.getNetNames();

    for (int n = 0; n < netNames.size(); n++) {
        vector<string> cellNames = (*netMap)[netNames[n]].getCellNames();;
        int cost = 0;
        int x_max = 0;
        int x_min = INT_MAX;
                //cout << "Net: " << netNames[n] << endl;
        for (int c = 0; c < cellNames.size(); c++) {
        	if ( cellNames[c] != cell.getName() ) {
        		Cell cell = (*cellMap)[cellNames[c]];
        		//cout << "Cell: " << cell << endl;
        		int _x = cell.getX();
        		x_max = (_x > x_max) ? _x : x_max;
        		x_min = (_x < x_min) ? _x : x_min;
        	}
        }
        if (x > x_max)
					{ cost = (x - x_max); }
        if (x < x_min)
					{ cost = (x_min - x); }

        totalCost += cost;
    }

        return totalCost;
}

void interleave(vector<Site>* v, unordered_map<string, Cell>* cellMap,
                unordered_map<string, Net>* netMap){
	int length = v->size();
	int start =0; int end = WINDOW_SIZE;
	int numWindows = (length/WINDOW_SIZE) + 1; //+1 for remainder
	int remainderSites = length%WINDOW_SIZE;
	vector<Cell> setA,setB;

	for(int j=0;j<numWindows;j++){ //TODO: fix index
		if(j*WINDOW_SIZE >= length){
			start = j*WINDOW_SIZE;
			end = start + remainderSites;
		} else {
			start = j*WINDOW_SIZE;
			end = start+WINDOW_SIZE;
		}
		//cout << "window= "<<j << " start:end = "<< start<< ":"<<end <<endl;

		bool emptyWindow = true;
		//cout << "Current Placement [";
		for(int i=start; i<end; i++){
			string cellName = (*v)[i].getCellName();
			//cout << cellName << ",";
			if(!cellName.empty() && i&1){
				setA.emplace_back((*cellMap)[cellName]);
				emptyWindow = false;
			} else if(!cellName.empty()){
				setB.emplace_back((*cellMap)[cellName]);
				emptyWindow = false;
			} else if(cellName.empty() && i&1){
				setA.emplace_back(Cell());
			} else {
				setB.emplace_back(Cell());
			}
		}
		//cout << "]" << endl;
		//if window has no cells, then move on
		if (emptyWindow) {
			//cout << "Found empty Window" << endl;
			setA.clear();
			setB.clear();
			continue;
		}
		// } else {
		// 	cout << "SetA.size() = " <<setA.size() <<
		// 	" SetB.size() = " << setB.size() << endl;
		// }
		//start interleaving
		vector<Cell> newCellPlacement;
		for(int i=start; i<end; i++){
			int costA, costB;
			string siteType = (*v)[i].getType();
			if(!setA.empty() && !setB.empty()){
				Cell a = setA.at(0);
				Cell b = setB.at(0);

				if(a.getName().empty() && b.getName().empty()){ // two empty cells
					if(setA.size() >= setB.size()){
						//cout << "placing A" << a <<" on site " << siteType<<endl;
						newCellPlacement.emplace_back(a);
						setA.erase(setA.begin());
					} else {
						//cout << "placing B " << b << " on site " << siteType<<endl;
						newCellPlacement.emplace_back(b);
						setB.erase(setB.begin());
					}

				} else if(a.getName().empty() && !b.getName().empty()){ //b is real cell
					costB = cost(b,i,netMap,cellMap);
					int temp = cost(b,i+1,netMap,cellMap); //place empty cell first or second?
					//if(costB <= temp && siteType == b.getType()){
					if(siteType == b.getType()){
						//cout << "placing B " << b << " on site " << siteType<<endl;
						newCellPlacement.emplace_back(b);
						setB.erase(setB.begin());
					} else {
						//cout << "placing A" << a <<" on site " << siteType<<endl;
						newCellPlacement.emplace_back(a);
						setA.erase(setA.begin());
					}

				} else if(!a.getName().empty() && b.getName().empty()) { //a is real cell
					costA = cost(a,i,netMap,cellMap);
					int temp = cost(a,i+1,netMap,cellMap);
					//if(costA <= temp && siteType == a.getType()){
					if(siteType == a.getType()){
						//cout << "placing A" << a <<" on site " << siteType<<endl;
						newCellPlacement.emplace_back(a);
						setA.erase(setA.begin());
					} else {
						//cout << "placing B " << b << " on site " << siteType<<endl;
						newCellPlacement.emplace_back(b);
						setB.erase(setB.begin());
					}

				} else { //Got two real cells
					costA = cost(a,i,netMap,cellMap);
					costB = cost(b,i,netMap,cellMap);
					if(costA <= costB && siteType == a.getType()){
						//cout << "placing A" << a <<" on site " << siteType<<endl;
						newCellPlacement.emplace_back(a);
						setA.erase(setA.begin());
					} else if(siteType == b.getType()){
						//cout << "placing B " << b << " on site " << siteType<<endl;
						newCellPlacement.emplace_back(b);
						setB.erase(setB.begin());
					} else {
						cout << endl << endl;
						cout << "BOTH CELL OPTIONS DID NOT MATCH SITE TYPE!!!!" << endl;
						cout << "siteType = " <<siteType<<" cellTypeA = " << a.getType()
						<<" cellTypeB = " << b.getType()<< endl;
						cout << endl;
						for(int k=0; k<setA.size(); k++){
							cout << "Trace: SetA["<<k<<"]=" << setA[k] << endl;
						}
						for(int k=0; k<setB.size(); k++){
							cout << "Trace: SetB["<<k<<"]=" << setB[k] << endl;
						}
					}
				}
				// cout << "SetA.size() = " <<setA.size() <<	" SetB.size() = "
				// 			<< setB.size() << endl;

			} else if(setA.empty() && !setB.empty()){ // can only choose from B
				Cell a;
				Cell b = setB.at(0);

				if(siteType == b.getType() || b.getType().empty()){
					//cout << "placing B " << b << " on site " << siteType<<endl;
					newCellPlacement.emplace_back(b);
					setB.erase(setB.begin());
				} else {
					cout << endl << endl;
					cout << "CELL OPTION FROM B DID NOT MATCH SITE TYPE!!!!" << endl;
					cout << "siteType = " <<siteType<<" cellTypeB = " << b.getType() << endl;
					cout << endl;
				}

				// cout << "SetA.size() = " <<setA.size() <<	" SetB.size() = "
				// 			<< setB.size() << endl;
			} else if(!setA.empty() && setB.empty()){ // can only choose from A
				Cell a = setA.at(0);
				Cell b;

				if(siteType == a.getType() || a.getType().empty()){
					//cout << "placing A" << a <<" on site " << siteType<<endl;
					newCellPlacement.emplace_back(a);
					setA.erase(setA.begin());
				} else {
					cout << endl << endl;
					cout << "CELL OPTION FROM A DID NOT MATCH SITE TYPE!!!!" << endl;
					cout << "siteType = " <<siteType<<" cellTypeA = " << a.getType() << endl;
					cout << endl;
				}

				// cout << "SetA.size() = " <<setA.size() <<	" SetB.size() = "
				// 			<< setB.size() << endl;
			} else {
				cout << "Finished interleaving window!!!" << endl;
			}
		}
		// cout <<"New Placement [";
		// for (int k=0; k<newCellPlacement.size(); k++){
		// 	cout << newCellPlacement[k].getName() << ",";
		// }
		// cout << "]" <<endl;

		for (int k=0; k<newCellPlacement.size(); k++){
			string cellName = newCellPlacement[k].getName();
			if(!cellName.empty()){
				Cell c = (*cellMap)[cellName];
				c.setX(start+k);
				(*cellMap)[cellName] = c;
			}
		}

		setA.clear();
		setB.clear();
	}
}

// Algorithm Driver Function
int main (int argc, char* argv[]) {
	string filepath = argv[1];
	string filename = argv[2];
	Parser parser = Parser(filepath, filename);
	vector<string> cellNames;
	vector<string> temp;
	Cell c;
	vector<vector<Site>> sitemap = parser.parseSitemap();
	unordered_map<string, Cell> cellMap = parser.parsePlacement();
	unordered_map<string, Net> netMap = parser.parseNetlist();

	int rows = sitemap.size();
	int cols = sitemap[0].size();

	Placement placement = Placement(rows, cols, sitemap);
	placement.addCells( cellMap.begin(), cellMap.end() );

  for(auto it = netMap.begin(); it != netMap.end(); ++it){
		cellNames = it->second.getCellNames();
		for(int i =0; i < cellNames.size(); i++){
			cellMap[cellNames[i]].addNet(it->first);
		}
	}

	//  Run Algorithm
	int hpwl = calcHPWL(&netMap, &cellMap);
	cout << "HPWL = " << hpwl << endl;

	for(int i=0; i<placement.getRows(); i++){
		cout << "Interleaving row " << i << endl;
		vector<Site> v = placement.getRow(i);
		interleave(&v, &cellMap, &netMap);
	}
	hpwl = calcHPWL(&netMap, &cellMap);
	cout << "New HPWL = " << hpwl << endl;
}
