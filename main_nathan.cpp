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




//									//
// Calculate Cost for a Global Swap	//
//									//
int global_swap_cost (Cell cell, int x, unordered_map<string, int>* localXMap,
		unordered_map<string, Net>* netMap, unordered_map<string, Cell>* cellMap) {
	int totalCost = 0;
	vector<string> netNames = cell.getNetNames();

}




//					    //
// Global Swap Function //
//						//
void global_swap_algorithm (int pickedRow, Placement* placement, 
		unordered_map<string, Cell>* cellMap, unordered_map<string, Net>* netMap) {

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
//	cout << "\n" << "Initial HPWL: " << beginHPWL << "\n"; 

	
	clock_t main_t, total_t;

	total_t = clock();	
	//for (int r = init_row; r < end_row; r++) {
		// Run Algorithm
//		cout << "\n" << "Picking Row " << r << " for Linear Placement..." << "\n";

		main_t = clock();
		global_swap_algorithm(&placement, &cellMap, &netMap);	
		div_t algoTime = div ( (clock() - main_t) / (double) CLOCKS_PER_SEC, 60);	
//		cout << "\n" << "Current Linear Placement Algorithm Time: ";
//		cout << algoTime.quot << " minutes, " << algoTime.rem << " seconds" << "\n";
	
	//}
		
	div_t totalTime = div ( (clock() - total_t) / (double) CLOCKS_PER_SEC, 60);	
//	cout << "\n" << "Total Linear Placement Algorithm Time: ";
//	cout << totalTime.quot << " minutes, " << totalTime.rem << " seconds" << "\n";


	// Check Total HPWL	after algorithm
	int endHPWL = calculateTotalHPWL(&cellMap, &netMap);
	double improvement = 100 * (beginHPWL - endHPWL) / (double)(beginHPWL);
//	cout << "\n" << "Final HPWL: " << endHPWL << "\n"; 
//	cout << "HPWL Improvement: " << improvement << "%" << "\n" << "\n";
}

