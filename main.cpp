#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <algorithm>
#include <tuple>
#include <ctime>

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




//													//
// Method to Calculate HPWL to Benchmark Algorithms //
//													//
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




//													   //
// Calculate Cost for Prefix Placement at a Given Site //
//													   //
int lp_cost (Cell cell, int x, unordered_map<string, int>* localXMap,
		unordered_map<string, Net>* netMap, unordered_map<string, Cell>* cellMap) {
	int totalCost = 0;
	vector<string> netNames = cell.getNetNames();
/*
	if ( !(*localXMap).empty() ) {
		cout << "\nFor position " << x << "...\n";
		for (auto it = (*localXMap).begin(); it != (*localXMap).end(); ++it) {
			cout << (*cellMap)[it->first] << ", " << it->second << "\n";	
		}
	}
*/
	for (int n = 0; n < netNames.size(); n++) {
		
		Net net = (*netMap)[netNames[n]];
		int cost = 0;


/*		if ( cell.getX() != net.getXMin() && x < net.getXMin() ) {
			cost = net.getXMin() - x;
		}
		else if ( cell.getX() != net.getXMin() && x > net.getXMax() ) {
			cost = x - net.getXMax();
		}
		else if ( (cell.getX() == net.getXMin() && cell.getX() == net.getXMax()) || 
				(cell.getX() == net.getXMin() && x < net.getXMax()) || 
				(cell.getX() == net.getXMax() && x > net.getXMin()) ) {
*/		// Only calculate cost if x is not in the net span	
			
			vector<string> cellNames = net.getCellNames();
			
			int x_max = 0;
			int x_min = INT_MAX;
			//cout << "Net: " << netNames[n] << "\n";
			for (int c = 0; c < cellNames.size(); c++) {
				if ( cellNames[c] != cell.getName() ) {
					int _x;
					auto it = (*localXMap).find(cellNames[c]);
					
					if ( (*localXMap).empty() || it == (*localXMap).end() ) {
						_x = (*cellMap)[cellNames[c]].getX();
					
					} else if ( it != (*localXMap).end() ) {
//						cout << "Using Cell: " << it->first << ", ";
//						cout << it->second << " for Net: " << net.getName() << "\n";
						
						_x = (it->second);	
					}

					x_max = (_x > x_max) ? _x : x_max;
					x_min = (_x < x_min) ? _x : x_min;
				}
			}
			if (x > x_max) { cost = (x - x_max); x_max = x; }
			if (x < x_min) { cost = (x_min - x); x_min = x; }
//		} 

		totalCost += cost;
	}
			
	return totalCost;
}




//							 //
// Linear Placement Function //
//							 //
void lp_algorithm (int pickedRow, Placement* placement, 
		unordered_map<string, Cell>* cellMap, unordered_map<string, Net>* netMap) {
	
	cout << "\n" << "Beginning Linear Placement algorithm..." << "\n" << "\n";
	// Start by picking an arbitrary row. Let's pick the first one
	int rows = (*placement).getRows();
	int cols = (*placement).getCols();
	int row = pickedRow;
	vector<Site> siteRow = (*placement).getRow(row);
/*
	cout << "Sites included in this row include: " << "\n" << "\n";	
	for (int i = 0; i < siteRow.size(); i++) {
		Site site = siteRow.at(i);
		cout << site << "\n";
	}
*/
	cout << "Cells included in this row include: " << "\n";	
	// Collect all cells in the row, agnostic of original placement
	vector<Cell> cells;

	for (int i = 0; i < siteRow.size(); i++) {
		Site site = siteRow.at(i);
		string cellName = site.getCellName();
		
		if ( !cellName.empty() ) { 
			Cell cell = (*cellMap)[cellName];
			cells.emplace_back(cell);
//			cout << cell << "\n"; 	
		}
	}                			
//	cout << "Total Number of Cells: " << cells.size() << "\n\n";
	
	// Initialize vector of Solution object, with location mincost and 
	// 		best possible site location relative to the given site
	int bigInt = INT_MAX/(rows+1);
	Solution initSol = { bigInt, -1 };		
	vector<vector<Solution>> solutionMatrix (cells.size(), 
		vector<Solution>(siteRow.size(), initSol));
	
	int numCells = cells.size();
	clock_t general_t;
	clock_t cost_t;
	int costOverheadTicks = 0;
	unsigned int costFunctionCalls = 0;

//	cout << "Generating solution matrix for cells and sites..." << "\n";
	general_t = clock();	


	// Loop through all cells in order due to order requirement
	for (int c = 0; c < numCells; c++) {
		Cell curCell = cells.at(c);
		int cost = 0;
		int fixedSite = 0;	
		unordered_map<string, int> localXMap;
	
//		cout << "Now updating solution matrix for " << curCell.getName();
//		cout << " (" << (c) << " - " << numCells-1 << ")..." << "\n";

		// Fixed Cell Handler
		if ( curCell.isFixed() )  {		
			cost_t = clock();
			
			cost = lp_cost( curCell, curCell.getX(), &localXMap, netMap, cellMap );
			
			costOverheadTicks += (clock() - cost_t);	
			costFunctionCalls++;

			fixedSite = curCell.getX();
			Solution fixedSol = {cost, curCell.getX()};	
				
			std::fill (solutionMatrix[c].begin() + fixedSite,
				 solutionMatrix[c].end(), fixedSol);
		// General Movable Cell Handler
		} else {
			int numCellsLeft = numCells - c;
			int firstSite = ((fixedSite+1) > c) ? (fixedSite+1) : c;
			for (int s = firstSite; s < (siteRow.size()); s++) {
				
				int siteCost = bigInt;	
	
				// Check for type mismatches	
				if ( siteRow[s].getType() == curCell.getType() ) {
					cost_t = clock();
				
					// Generate accurate cell locations for current best solution
					int lastSite = s-1;		
				
					for (int i = c; i > 0; i--) { 
						string pastName = cells[i-1].getName();
						int pastSite = solutionMatrix[i-1][lastSite].site;
						if (pastSite < 0) { break; }
						localXMap[ pastName ] = pastSite;
						lastSite = pastSite-1;
					}	
					
					siteCost = lp_cost( curCell, s, &localXMap, netMap, cellMap );
					costOverheadTicks += (clock() - cost_t);
					costFunctionCalls++;
				} else {
//					cout << "S: " << s << ", C: " << c << "\n";
					if ( s == c ) {
						solutionMatrix[c][s].cost = siteCost;
						solutionMatrix[c][s].site = s;	
					} else {
						int lastCost = solutionMatrix[c][s-1].cost;	
						int lastSite = solutionMatrix[c][s-1].site;	
						solutionMatrix[c][s].cost = lastCost;
						solutionMatrix[c][s].site = lastSite;
					}
					continue;
				}

	
				//cout << "cost(" << s << "): " << siteCost << "\n"; 		

				// The first cell considers only its best position
				if ( c == 0 ) {
					// First cell in first location has one option
					if ( s == c ) {	
						solutionMatrix[c][s].cost = siteCost;
						solutionMatrix[c][s].site = s;
						
					// Checks if previous cost is better
					} else {
						int lastCost = solutionMatrix[c][s-1].cost;	
						int lastSite = solutionMatrix[c][s-1].site;	
					
						//cout << siteCost << " vs " << lastCost << "\n";	
						// Approaching min point
						if (siteCost < lastCost) {
							solutionMatrix[c][s].cost = siteCost;
							solutionMatrix[c][s].site = s;
						// Extending local region or min point
						} else if (siteCost == lastCost) {
							solutionMatrix[c][s].cost = siteCost;
							solutionMatrix[c][s].site = s;
//							solutionMatrix[c][s].cost = lastCost;
//							solutionMatrix[c][s].site = lastSite;
						} else if (siteCost > lastCost) {
//							solutionMatrix[c][s].cost = lastCost;
//							solutionMatrix[c][s].site = lastSite;
							Solution lastSol = {lastCost, lastSite};
							std::fill (solutionMatrix[c].begin() + s,
							 solutionMatrix[c].end(), lastSol); 
//							cout << "//SNICKERS//" << "\n";
							break;
						}

					}
				// If considering a cell after first, we must consider past cells
				} else {
					int thisCost = solutionMatrix[c-1][s-1].cost + siteCost;
					thisCost = std::min(thisCost, bigInt);				

					// If we're considering the first site, use past cell first
					//   location and cost of current cell first site
					if ( s == c ) {
						solutionMatrix[c][s].cost = thisCost;
						solutionMatrix[c][s].site = s;
					// General case must check all options
					} else {
						int lastCost = solutionMatrix[c][s-1].cost;	
						int lastSite = solutionMatrix[c][s-1].site;	

						if (thisCost < lastCost) {
							solutionMatrix[c][s].cost = thisCost;
							solutionMatrix[c][s].site = s;
						} else if (thisCost == lastCost) {
							solutionMatrix[c][s].cost = thisCost;
							solutionMatrix[c][s].site = s;
//							solutionMatrix[c][s].cost = lastCost;
//							solutionMatrix[c][s].site = lastSite;
						} else if (thisCost > lastCost) {
//							solutionMatrix[c][s].cost = lastCost;
//							solutionMatrix[c][s].site = lastSite;
							Solution lastSol = {lastCost, lastSite};
							std::fill (solutionMatrix[c].begin() + s,
							 solutionMatrix[c].end(), lastSol); 
//							cout << "//SNICKERS//" << "\n";
							break;
						}
					}
				}
			
				//cout << "minCost: " << solutionMatrix[c][s].cost << "\n";
				// end of site loop
			}
			// end of fixed cell conditional
/*		
			cout << "Cost: [ ";
			for (int s = 0; s < solutionMatrix[c].size(); s++) { 
				cout << solutionMatrix[c][s].cost << " "; 
			}
			cout << "]" << "\n";
			
			cout << "Site: [ ";
			for (int s = 0; s < solutionMatrix[c].size(); s++) { 
				cout << solutionMatrix[c][s].site << " "; 
			}
			cout << "]" << "\n" << "\n";
*/
		}		
		// end of cell loop
	}
	// end of lp_algorithm


	div_t matrixTime = div ( (clock() - general_t) / (double) CLOCKS_PER_SEC, 60 );	
	cout << "Linear Placement Matrix Creation Time: ";
	cout << matrixTime.quot << " minutes, " << matrixTime.rem << " seconds" << "\n";

	div_t costTime = div ( costOverheadTicks / (double) CLOCKS_PER_SEC, 60 );	
	double costAvgTime = ( costOverheadTicks / (double) costFunctionCalls / 
		(double) CLOCKS_PER_SEC * 1000.0 );
	cout << "Linear Placement Cost Overhead Time: ";
	cout << costTime.quot << " minutes, " << costTime.rem << " seconds" << "\n";
	
	cout << "Linear Placement Cost Average Time: ";
	cout << costAvgTime << " milliseconds" << "\n";
	cout << "Number of lp_cost() calls: " << costFunctionCalls << "\n";
	
	int c = cells.size() - 1;
	int s = siteRow.size() - 1;
	vector<int> finalCellLocations (cells.size());	


	cout << "Traversing the solution matrix..." << "\n";	
	general_t = clock();
	// Traverse the solution matrix to determine the best cell locations for a given
	//		siteRow and given initial cell list
	while (c >= 0) {
		int cost = solutionMatrix[c][s].cost;
	
		while ( (s != 0) && (cost == solutionMatrix[c][s-1].cost) ) {
/*			cout << "[" << solutionMatrix[c][s].cost << ", "; 
			cout << solutionMatrix[c][s].site << "]" << " vs " << "[";
			cout << solutionMatrix[c][s-1].cost << ", ";
			cout << solutionMatrix[c][s-1].site << "]" << "\n"; 
*/			s--;
		}
	
//		cout << "Cell: " << c << " ; Site: " << s;
//		cout << " <==> " << solutionMatrix[c][siteRow.size()-1].site << "\n";	
		finalCellLocations[c] = s;	
		s--; c--;
	}
/*	
	cout << "\n" << "[ ";
	for (int i = 0; i < cells.size(); i++) { cout << cells[i].getX() << " "; }
	cout << "]" << "\n";
	cout << "==>" << "\n";
	cout << "[ ";
	for (int pos : finalCellLocations) { cout << pos << " "; }
	cout << "]" << "\n";
*/
		
	div_t travTime = div ( (clock() - general_t) / (double) CLOCKS_PER_SEC, 60);	
	cout << "Linear Placement Solution Traversal Time: ";
	cout << travTime.quot << " minutes, " << travTime.rem << " seconds" << "\n";


	cout << "Updating cellMap for HPWL calculations..." << "\n";
	// Updating the cellMap for each cell changed in the siteRow
	for (int i = 0; i < cells.size(); i++) {
		cells[i].setX( finalCellLocations[i] );
		(*cellMap)[ cells[i].getName() ] = cells[i];
	}
	
}




//							 //
//							 //
// Algorithm Driver Function //
//							 //
//							 //
int main (int argc, char* argv[]) {
	
	// Generate Parser and parse all input files
	string filepath = argv[1];
	string filename = argv[2];
	int init_row = stoi(argv[3]);
	int end_row = stoi(argv[4]);
	if (end_row - init_row < 0) { cout << "Bad row range" << "\n"; abort(); }
	Parser parser = Parser(filepath, filename);
	vector<vector<Site>> sitemap = parser.parseSitemap();	
	unordered_map<string, Net> netMap = parser.parseNetlist();
	unordered_map<string, Cell> cellMap = parser.parsePlacement();
	
	int rows = sitemap.size();
	int cols = sitemap[0].size();

	// Initialize nets with bounding box parameters
	for (auto it_net = netMap.begin(); it_net != netMap.end(); ++it_net) {
 		string name = (it_net->first);
		Net net = (it_net->second);
		net.setBoundingBox (&cellMap);
		netMap[name] = net;
	}	

	
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
	cout << "\n" << "Initial HPWL: " << beginHPWL << "\n"; 

	
	clock_t main_t, total_t;

	total_t = clock();	
	for (int r = init_row; r < end_row; r++) {
		// Run Algorithm
		//int r = pickedRow;
		cout << "\n" << "Picking Row " << r << " for Linear Placement..." << "\n";

		main_t = clock();
		lp_algorithm(r, &placement, &cellMap, &netMap);	
		div_t algoTime = div ( (clock() - main_t) / (double) CLOCKS_PER_SEC, 60);	
		cout << "\n" << "Current Linear Placement Algorithm Time: ";
		cout << algoTime.quot << " minutes, " << algoTime.rem << " seconds" << "\n";
	
	}
		
	div_t totalTime = div ( (clock() - total_t) / (double) CLOCKS_PER_SEC, 60);	
	cout << "\n" << "Total Linear Placement Algorithm Time: ";
	cout << totalTime.quot << " minutes, " << totalTime.rem << " seconds" << "\n";


	// Check Total HPWL	after algorithm
	int endHPWL = calculateTotalHPWL(&cellMap, &netMap);
	double improvement = 100 * (beginHPWL - endHPWL) / (double)(beginHPWL);
	cout << "\n" << "Final HPWL: " << endHPWL << "\n"; 
	cout << "HPWL Improvement: " << improvement << "%" << "\n" << "\n";
}

