#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <algorithm>
#include <tuple>

#include "Net.h"
#include "Cell.h"
#include "Site.h"
#include "Placement.h"
#include "Parser.h"

using namespace std;


struct Solution {
	int cost;
	int site;
};


// Helper Functions
int calculateTotalHPWL (unordered_map<string, Cell>* cellMap, 
		unordered_map<string, Net>* netMap) {
	int hpwl = 0;
	
	for (auto it = (*netMap).begin(); it != (*netMap).end(); ++it) {
		vector<string> cellNames = (it->second).getCellNames();
		int x_max = 0;
		int y_max = 0;
		int x_min = INT_MAX;
		int y_min = INT_MAX;	
	
		for (int i = 0; i < cellNames.size(); i++) {
			int x = (*cellMap)[cellNames[i]].getX();
			int y = (*cellMap)[cellNames[i]].getY();
			x_max = (x > x_max) ? x : x_max;
			x_min = (x < x_min) ? x : x_min;
			y_max = (y > y_max) ? y : y_max;
			y_min = (y < y_min) ? y : y_min;
		}
		hpwl += abs(x_max - x_min) + abs(y_max - y_min);
	}
	
	return hpwl;
}


int lp_cost (Cell cell, int x, unordered_map<string, Net>* netMap,
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
		if (x > x_max) { cost = (x - x_max); }
		if (x < x_min) { cost = (x_min - x); }
	
		totalCost += cost;
	}
			
	return totalCost;
}


void lp_algorithm (Placement placement, 
		unordered_map<string, Cell> cellMap, unordered_map<string, Net> netMap) {
	
	// Start by picking an arbitrary row. Let's pick the first one
	int rows = placement.getRows();
	int cols = placement.getCols();
	int row = 0;
	vector<Site> siteRow = placement.getRow(row);
	
	// Collect all cells in the row, agnostic of original placement
	vector<Cell> cells;
	for (int i = 0; i < siteRow.size(); i++) {
		Site site = siteRow.at(i);
		string cellName = site.getCellName();
		
		if ( !cellName.empty() ) { 
			Cell cell = cellMap[cellName];
			cells.emplace_back(cell); 	
		}
	}                			
	
	// Initialize vector of Solution object, with location mincost and 
	// 		best possible site location relative to the given site
	Solution initSol = { INT_MAX/2, -1 };		
	vector<vector<Solution>> solutionMatrix (cells.size(), 
		vector<Solution>(siteRow.size(), initSol));
	
	int numCells = cells.size();

	// Loop through all cells in order due to order requirement
	for (int c = 0; c < numCells; c++) {
		Cell curCell = cells.at(c);
		int cost = 0;
		int fixedSite = 0;	
			
		cout << curCell << endl;
	
		if ( curCell.isFixed() )  {		
			cost = lp_cost( curCell, curCell.getX(), &netMap, &cellMap );			
			fixedSite = curCell.getX();
			Solution fixedSol = {cost, curCell.getX()};	
				
			std::fill (solutionMatrix[c].begin()+fixedSite,
				 solutionMatrix[c].end(), fixedSol);
		} else {
			int numCellsLeft = numCells - c;
			int firstSite = ((fixedSite+1) > c) ? (fixedSite+1) : c;
			for (int s = firstSite; s < (siteRow.size() - numCellsLeft); s++) {
				
				int siteCost = INT_MAX/2;
				if ( siteRow[s].getType() == curCell.getType() ) {
					siteCost = lp_cost( curCell, s, &netMap, &cellMap );
				}
			
				cout << "cost(" << s << "): " << siteCost << endl; 				
				// The first cell considers only its best position
				if ( c == 0 ) {
					// First cell in first location has one option
					if ( s == c ) {	
						solutionMatrix[c][s].cost = siteCost;
						solutionMatrix[c][s].site = s;
						
					// Checks if previous cost is better
					} else {
						int lastCost = solutionMatrix[c][s-1].cost;	

						if (lastCost < siteCost) {
							solutionMatrix[c][s].cost = lastCost;
							solutionMatrix[c][s].site = s-1;
						} else {
							solutionMatrix[c][s].cost = siteCost;
							solutionMatrix[c][s].site = s;
						}

					}
				// If considering a cell after first, we must consider past cells
				} else {
					int thisCost = solutionMatrix[c-1][s-1].cost + siteCost;
					// If we're considering the first site, use past cell first
					//   location and cost of current cell first site
					if ( s == c ) {
						solutionMatrix[c][s].cost = thisCost;
						solutionMatrix[c][s].site = s;
					// General case must check all options
					} else {
						int lastCost = solutionMatrix[c][s-1].cost;	
						
						if (lastCost < thisCost) {
							solutionMatrix[c][s].cost = lastCost;
							solutionMatrix[c][s].site = s-1;
						} else {
							solutionMatrix[c][s].cost = thisCost;
							solutionMatrix[c][s].site = s;
						}
					}
				}
				cout << "minCost: " << solutionMatrix[c][s].cost << endl;
				// end of site loop
			}
			// end of fixed cell conditional
		}		
		// end of cell loop
	}
	// end of lp_algorithm

	
	int c = cells.size();
	int s = siteRow.size();
	int cellsLeft = cells.size();	
	
	// Traverse the solution matrix to determine the best cell locations for a given
	//		siteRow and given initial cell list
//	while (cellsLeft > 0) {
//		int cost = solutionMatrix[c][s].cost;
//		int siteCost = lp_cost( curCell, curCell.getX(), &netMap, &cellMap );			
//		if (cost == solutionMatrix[]) {
//			
//		}		
//	}
}






// Algorithm Driver Function
int main (int argc, char* argv[]) {
	
	// Generate Parser and parse all input files
	string filepath = argv[1];
	string filename = argv[2];
	Parser parser = Parser(filepath, filename);
	vector<vector<Site>> sitemap = parser.parseSitemap();	
	unordered_map<string, Net> netMap = parser.parseNetlist();
	unordered_map<string, Cell> cellMap = parser.parsePlacement();
	
	int rows = sitemap.size();
	int cols = sitemap[0].size();
	
	// Initialize cells in cellMap to include net names
	for (auto it_net = netMap.begin(); it_net != netMap.end(); ++it_net) {
		Net net = (it_net->second);
		vector<string> cellNames = net.getCellNames();
		for (auto it_cell = cellNames.begin(); it_cell != cellNames.end(); 
				++it_cell) {
			string cellName = (*it_cell);
			(cellMap[cellName]).addNet(net.getName());	
		}
	}	
	
	// Initialize placement of FPGA sites
	Placement placement = Placement(rows, cols, sitemap);
	placement.addCells( cellMap.begin(), cellMap.end() );	

	// Check Total HPWL	before algorithm
	int beginHPWL = calculateTotalHPWL(&cellMap, &netMap);
	cout << "Total HPWL: " << beginHPWL << endl; 
	
	// Run Algorithm
	lp_algorithm(placement, cellMap, netMap);	

	// Check Total HPWL	after algorithm
//	int endHPWL = calculateTotalHPWL(&cellMap, &netMap);
//	cout << "Total HPWL: " << endHPWL << endl; 

}

