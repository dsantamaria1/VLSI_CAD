#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>

#include "Net.h"
#include "Cell.h"
#include "Site.h"
#include "Placement.h"
#include "Parser.h"
#include <cmath>

#define WINDOW_SIZE 7
#define DIVISOR 8
using namespace std;

struct seq{
	int cost;
	vector<string> newCellList;
};

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
		if(cell.getName().empty()){
			return 0;
		}
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
	int counter = 0;
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
				counter++;
			} else if(!cellName.empty()){
				setB.emplace_back((*cellMap)[cellName]);
				emptyWindow = false;
				counter++;
			} else if(cellName.empty() && i&1){
				setA.emplace_back(Cell());
			} else {
				setB.emplace_back(Cell());
			}
		}
		//cout << "]" << endl;
		//if window has no cells, then move on
		if (emptyWindow) {
		//	cout << "Empty Window" << endl;
			setA.clear();
			setB.clear();
			continue;
		}

		//start interleaving
		vector<Cell> newCellPlacement;
		seq seqInit = {.cost=0};
		vector<vector<seq>> solutionMatrix(setA.size()+1,
		 						vector<seq>(setB.size()+1, seqInit));

	    int i_index,j_index,costA,costB;
		for(int i=0;i<=setA.size();i++){
			for(int j=0;j<=setB.size();j++){
				//cout << "Analyzing sequence " <<i<<","<<j<<endl;
				if(i == 0 && j == 0) { continue; }
				//clamp lower bound to 0
				i_index = (i-1 < 0) ? 0 : i-1;
				j_index = (j-1 < 0) ? 0 : j-1;
				Cell a = setA[i_index];
				Cell b = setB[j_index];


				seq newItem;
				seq prevI = solutionMatrix[i_index][j];
				seq prevJ = solutionMatrix[i][j_index];
				int xLocationA = prevI.newCellList.size()+1+start;
				int xLocationB = prevJ.newCellList.size()+1+start;
				// cout <<"A part is S"<<(i-1)<<","<<j<<"+a"<<i<<endl;
				// cout <<"B part is S"<<i<<","<<(j-1)<<"+b"<<j<<endl;
				// cout <<"siteType at " <<start+i+j-1 << endl;
				int siteIndex = start+i+j-1;
				string siteType = (*v)[siteIndex].getType();

				// cout << "siteType: "<<siteType<<endl;
				// cout << "a: " <<a.getType() << endl;
				// cout << "b: "<<b.getType() << endl;

				if(i == 0){ //non existant entry
						costA = INT_MAX;
				}	else if(siteType == a.getType()){
						if(a.isFixed() && siteIndex !=  a.getX()){
								// cout << "A: matching type but its fixed!!" << endl;
								// cout << "siteIndex="<<siteIndex <<" a.x="<<a.getX()<<endl;
								costA = INT_MAX;
						} else {
								int originalCost = cost(a,a.getX(),netMap,cellMap);
								int temp = cost(a,xLocationA,netMap,cellMap);
								costA = prevI.cost + abs(originalCost - temp);
						}
				} else { //empty cell
						costA = 0;// prevI.cost + INT_MAX/DIVISOR;
				}

				if(j == 0){ //non existant entry
						costB = INT_MAX;
				} else if(siteType == b.getType()){
						if(b.isFixed() && siteIndex != b.getX()){
							  // cout << "B: matching type but its fixed!!" << endl;
								// cout << "siteIndex="<<siteIndex <<" b.x="<<b.getX()<<endl;
								costB = INT_MAX;
						} else {
								int originalCost = cost(b,b.getX(),netMap,cellMap);
								int temp = cost(b,xLocationB,netMap,cellMap);
								costB = prevJ.cost + abs(originalCost - temp);
						}
				} else { //empty cell
						costB = 0;//prevJ.cost + INT_MAX/DIVISOR;
				}

				// if(costA == INT_MAX && costB == INT_MAX){
				// 	cout << "ZOMG i=" <<i<<" j="<<j<< endl;
				// }

				if(a.getName().empty() && b.getName().empty()){
						cout <<"CP #0" << endl;
						if(costA < costB){ //TODO: this can be flipped
							cout <<"Solution for S"<<i<<","<<j<<"= a"<<i<<endl;
							vector<string> newSeq = prevI.newCellList;
							newSeq.push_back(a.getName());
							newItem = {.cost=prevI.cost, .newCellList=newSeq};
						} else {
							cout <<"Solution for S"<<i<<","<<j<<"= b"<<j<<endl;
							vector<string> newSeq = prevJ.newCellList;
							newSeq.push_back(b.getName());
							newItem = {.cost=prevJ.cost, .newCellList=newSeq};
						}
				} else if(!a.getName().empty() && b.getName().empty()){
						cout <<"CP #1" << endl;
						if(costA != INT_MAX){
								cout <<"Solution for S"<<i<<","<<j<<"= a"<<i<<endl;
								vector<string> newSeq = prevI.newCellList;
								newSeq.push_back(a.getName());
								newItem = {.cost=prevI.cost, .newCellList=newSeq};
						} else {
								//INVALID costA == INT_MAX
								cout <<"Solution for S"<<i<<","<<j<<"= b"<<j<<endl;
								vector<string> newSeq = prevJ.newCellList;
								newSeq.push_back(b.getName());
								newItem = {.cost=prevJ.cost, .newCellList=newSeq};
						}
				} else if(a.getName().empty() && !b.getName().empty()){
						cout <<"CP #2" << endl;
						int originalCost = cost(b,b.getX(),netMap,cellMap);
						int temp = cost(b,xLocationB,netMap,cellMap);
						int tempB = prevJ.cost + abs(originalCost - temp);
						cout <<"originalCost="<<originalCost <<endl;
						cout <<"temp= "<<temp <<endl;
						cout <<"prevJ.cost = "<< prevJ.cost <<endl;
						if(costB != INT_MAX){
							cout <<"Solution for S"<<i<<","<<j<<"= b"<<j<<endl;
							vector<string> newSeq = prevJ.newCellList;
							newSeq.push_back(b.getName());
							newItem = {.cost=costB, .newCellList=newSeq};
						} else {
							//INVALID costB == INT_MAX
							cout <<"Solution for S"<<i<<","<<j<<"= a"<<i<<endl;
							vector<string> newSeq = prevI.newCellList;
							newSeq.push_back(a.getName());
							newItem = {.cost=prevI.cost, .newCellList=newSeq};
						}
				} else {
					cout <<"CP #3" << endl;
					if(costA < costB){
						cout <<"Solution for S"<<i<<","<<j<<"= a"<<i<<endl;
						vector<string> newSeq = prevI.newCellList;
						newSeq.push_back(a.getName());
						newItem = {.cost=costA, .newCellList=newSeq};
					} else {
						cout <<"Solution for S"<<i<<","<<j<<"= b"<<j<<endl;
						vector<string> newSeq = prevJ.newCellList;
						newSeq.push_back(b.getName());
						newItem = {.cost=costB, .newCellList=newSeq};
					}
				}

				solutionMatrix[i][j] = newItem;
				cout <<endl;
			}
		}
		//write new location
		seq newPlacement = solutionMatrix[setA.size()][setB.size()];
		//cout <<"new placement is ";
		for (int i=0;i<setA.size()+setB.size();i++){
				string cell = newPlacement.newCellList[i];
				//cout << cell<<",";
				if(!cell.empty()){
					Cell c = (*cellMap)[cell];
					cout <<"Original x for cell:"<<c.getName()<<"="<< c.getX()<<endl;
					c.setX(start+i);
					cout <<"cost: "<< newPlacement.cost <<endl;
					cout <<"New x for cell:"<<c.getName()<<"="<< c.getX()<<endl <<endl;
					(*cellMap)[cell] = c;
 				}
		}
		cout <<endl;

		setA.clear();
		setB.clear();
	}
	cout <<"cell count=" <<counter <<endl;
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

	//for(int i=0; i<placement.getRows(); i++){
	for(int i=0; i<1; i++){
		cout << "Interleaving row " << i << endl;
		vector<Site> v = placement.getRow(37);
		interleave(&v, &cellMap, &netMap);
	}
	hpwl = calcHPWL(&netMap, &cellMap);
	cout << "New HPWL = " << hpwl << endl;
}
