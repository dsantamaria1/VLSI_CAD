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


//												   //
// Method to Calculate HPWL for all Nets of a Cell //
//												   //
int calculateCellHPWL (Cell cell, unordered_map<string, Cell>* cellMap, 
		unordered_map<string, Net>* netMap) {
	int hpwl = 0;
	
	vector<string> netNames = cell.getNetNames();
	
	for (int n = 0; n < netNames.size(); n++) {
		vector<string> cellNames = (*netMap)[netNames[n]].getCellNames();

		int x_max = 0;
		int y_max = 0;
		int x_min = INT_MAX;
		int y_min = INT_MAX;

		for (int c = 0; c < cellNames.size(); c++) {
			if ( cellNames[c] != cell.getName() ) {
				int x = (*cellMap)[cellNames[c]].getX();
				int y = (*cellMap)[cellNames[c]].getY();
				x_max = (x > x_max) ? x : x_max;
				x_min = (x < x_min) ? x : x_min;
				y_max = (y > y_max) ? y : y_max;
				y_min = (y < y_min) ? y : y_min;
			}
		}
		
		int x_cell = cell.getX();
		int y_cell = cell.getY();
		x_max = (x_cell > x_max) ? x_cell : x_max;
		x_min = (x_cell < x_min) ? x_cell : x_min;
		y_max = (y_cell > y_max) ? y_cell : y_max;
		y_min = (y_cell < y_min) ? y_cell : y_min;
		
		hpwl += abs(x_max - x_min) + abs(y_max - y_min);
	}
	
	return hpwl;
}


//									//
// Calculate Cost for a Global Swap	//
//									//

tuple<int, int, int, int> get_optimal_region (Cell cell, 
		unordered_map<string, Cell>* cellMap, unordered_map<string, Net>* netMap) {
        
	vector<string> netNames = cell.getNetNames();
	vector<int> xBounds;
	vector<int> yBounds;
	int topBound, bottomBound, leftBound, rightBound;	


	for (int n = 0; n < netNames.size(); n++) {
	
		Net net = (*netMap)[netNames[n]];	
		
//		int x_min = net.getXMin();
//		int x_max = net.getXMax();
//		int y_min = net.getYMin();
//		int y_max = net.getYMax();

//		if ( cell.getX() != net.getXMin() && cell.getX() != net.getXMax() 
//				&& cell.getY() != net.getYMin() && cell.getY() != net.getYMax() ) {
	
//		} else {
		// Only calculate cost if x is not in the net span
		
			vector<string> cellNames = net.getCellNames();
			
			int x_min = INT_MAX;
			int x_max = 0;
			int y_min = INT_MAX;
			int y_max = 0;
			
			for (int c = 0; c < cellNames.size(); c++) {
				int _x = (*cellMap)[cellNames[c]].getX();
				int _y = (*cellMap)[cellNames[c]].getY();
				
				if ( cellNames[c] != cell.getName() ) {
					x_min = (_x < x_min) ? _x : x_min;
					x_max = (_x > x_max) ? _x : x_max;
					y_min = (_y < y_min) ? _y : y_min;
					y_max = (_y > y_max) ? _y : y_max;
				}
				
			}
//		}

		xBounds.emplace_back(x_min);
		xBounds.emplace_back(x_max);
		yBounds.emplace_back(y_min);
		yBounds.emplace_back(y_max);
	}
   
	std::sort( xBounds.begin(), xBounds.end() );
	std::sort( yBounds.begin(), yBounds.end() );	

	// Check if N is odd, then pick single median
	int x_mid = xBounds.size() / 2;
	if ( xBounds.size() % 2 == 1 ) {
		leftBound = rightBound = xBounds[x_mid];	
	} else {
		rightBound = xBounds[x_mid];
		leftBound = xBounds[x_mid - 1];
	}

	int y_mid = yBounds.size() / 2;
	if ( yBounds.size() % 2 == 1 ) {
		topBound = bottomBound = yBounds[y_mid];
	} else {
		bottomBound = yBounds[y_mid];
		topBound = yBounds[y_mid - 1];
	}

 
	return std::make_tuple(topBound, bottomBound, leftBound, rightBound);
}




//					    //
// Global Swap Function //
//						//
void global_swap_algorithm (Placement* placement, 
		unordered_map<string, Cell>* cellMap, unordered_map<string, Net>* netMap) {
	
	// Loop through every cell in cell map and try to swap it into its
	//   optimal region	
	int cellsDone = 0;
	int totalCells = (*cellMap).size();
	for (auto it = (*cellMap).begin(); it != (*cellMap).end(); ++it) {
		Cell cell =	(it->second);
		cout << "Running global swap for cell " << cellsDone << " of ";
		cout << totalCells << "\n";
		cellsDone++;
	
		// If cell is fixed, we can't swap it, so move to next cell
		if ( cell.isFixed() ) { continue; }

		// Get bounding box of all nets connected to cell, not including the cell
		//   and find the coordinates of the optimal region for this cell
		int topBound, bottomBound, leftBound, rightBound;
		std::tie(topBound, bottomBound, leftBound, rightBound) 
			= get_optimal_region(cell, cellMap, netMap);
//		cout << "Bounds are X: (" << leftBound << ", " << rightBound << ") & Y: (";
//		cout << topBound << ", " << bottomBound << ")\n"; 

		// Create list of sites to try to swap with in the optimal region
		vector<tuple<int, int>> boxPoints;
		if (topBound == bottomBound && leftBound == rightBound) { 
			boxPoints.emplace_back( make_tuple(leftBound, bottomBound) ); 
		} else if (topBound == bottomBound) {
			for (int x = leftBound; x <= rightBound; x++) {
				boxPoints.emplace_back( make_tuple(x, topBound) ); 
			}
		// Note that larger y values correspond with lower cells in placement
		} else if (leftBound == rightBound) {
			for (int y = topBound; y <= bottomBound; y++) {
				boxPoints.emplace_back( make_tuple(leftBound, y) ); 
			}
		} else {
			for (int x = leftBound; x <= rightBound; x++) {
				for (int y = topBound; y <= bottomBound; y++) {
					boxPoints.emplace_back( make_tuple(x, y) ); 
				}
			}
		}
//		cout << "Points in optimal region: " << boxPoints.size() << "\n";
				
		// Investigate each optimal region point
		// Currently this just checks the corners of the optimal region

		int initCellHPWL = calculateCellHPWL(cell, cellMap, netMap);
		for (int p = 0; p < boxPoints.size(); p++ ) {
			int x_new, y_new;
			std::tie(x_new, y_new) = boxPoints[p];
			Site site = (*placement).getSite(y_new, x_new);
			
			if ( site.getType() != cell.getType() ) {
				// Cells cannot be placed into invalid sites
				continue;	
			}
			
			// Check if cell exists in location
			string otherCellName = site.getCellName();

			if ( otherCellName == "" ) {
				// Determine whether swapping cell into space is worth it
				int initHPWL = initCellHPWL;
				Cell newCell = cell;
				newCell.setX(x_new); newCell.setY(y_new);	
				int endHPWL = calculateCellHPWL(newCell, cellMap, netMap);
	
				// Pick the first location that provides benefit
				if (endHPWL < initHPWL) {
//					cout << "HPWL: " << initHPWL << " => " << endHPWL << "\n";
//					cout << "Moving " << cell.getName() << " from (";
//					cout << cell.getX() << ", " << cell.getY() << ") to (";
//					cout << y_new << ", " << x_new << ")\n";
					(*placement).swapCells( cell.getY(), cell.getX(), 
						newCell.getY(), newCell.getX() );
					(*cellMap)[ newCell.getName() ] = newCell;
					break;
				}	
			} else {
				Cell otherCell = (*cellMap)[otherCellName]; 
				// If a cell exists here, check if fixed
				if ( otherCell.isFixed() ) {
					// Cannot swap with fixed cells
					continue;	
				} else {
/*
					// Determine whether swapping both cells is worth it
					int initHPWL = initCellHPWL
						+ calculateCellHPWL(otherCell, cellMap, netMap);
					
					// Simulate swapping the cells
					Cell newCell = cell; 
					newCell.setX( otherCell.getX() ); 
					newCell.setY( otherCell.getY() );	
					Cell newOtherCell = otherCell;
					newOtherCell.setX( cell.getX() );
					newOtherCell.setY( cell.getY() );	
					
					// Recalculate HPWL for both new cell locations
					int endHPWL = calculateCellHPWL(newCell, cellMap, netMap) 
						+ calculateCellHPWL(newOtherCell, cellMap, netMap);
				
					// Pick the first location that provides benefit
					if (endHPWL < initHPWL) { 
//						cout << "HPWL: " << initHPWL << " => " << endHPWL << "\n";
//						cout << "Swapping " << cell.getName() << " at (";
//						cout << cell.getX() << ", " << cell.getY() << ") with "; 
//						cout << otherCell.getName() << " at (" << otherCell.getX();
//						cout << ", " << otherCell.getY() << ")\n";
						(*placement).swapCells( cell.getY(), cell.getX(), 
							otherCell.getY(), otherCell.getX() );
						(*cellMap)[ newCell.getName() ] = newCell;
						(*cellMap)[ newOtherCell.getName() ] = newOtherCell;
						break;
					}
*/				
				}
			}
			
			// Try next optimal location for a given cell
		}


		// Move on to next cell in cell map
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
	Parser parser = Parser(filepath, filename);
	vector<vector<Site>> sitemap = parser.parseSitemap();	
	unordered_map<string, Net> netMap = parser.parseNetlist();
	unordered_map<string, Cell> cellMap = parser.parsePlacement();
	
	int rows = sitemap.size();
	int cols = sitemap[0].size();
	int cells = cellMap.size();
	int nets = netMap.size();
	int sites = sitemap.size() * sitemap[0].size();

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

//	total_t = clock();	
	//for (int r = init_row; r < end_row; r++) {
		// Run Algorithm
//		cout << "\n" << "Picking Row " << r << " for Linear Placement..." << "\n";

		main_t = clock();
		global_swap_algorithm(&placement, &cellMap, &netMap);	
		div_t algoTime = div ( (clock() - main_t) / (double) CLOCKS_PER_SEC, 60);	
		cout << "\n" << "Current Global Swap Algorithm Time: ";
		cout << algoTime.quot << " minutes, " << algoTime.rem << " seconds\n\n";	
	//}
		
//	div_t totalTime = div ( (clock() - total_t) / (double) CLOCKS_PER_SEC, 60);	
//	cout << "\n" << "Total Linear Placement Algorithm Time: ";
//	cout << totalTime.quot << " minutes, " << totalTime.rem << " seconds" << "\n";

	// Check Placement Validity
	placement.checkValidity( &cellMap );

	// Check Total HPWL	after algorithm
	int endHPWL = calculateTotalHPWL(&cellMap, &netMap);
	double improvement = 100 * (beginHPWL - endHPWL) / (double)(beginHPWL);
	cout << "\n" << "Final HPWL: " << endHPWL << "\n"; 
	cout << "HPWL Improvement: " << improvement << "%" << "\n" << "\n";
}

