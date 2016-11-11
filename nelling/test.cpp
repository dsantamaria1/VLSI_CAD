#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>

#include "Net.h"
#include "Cell.h"
#include "Site.h"
#include "Placement.h"
#include "Parser.h"

using namespace std;


// Helper Functions
void test_parser_objects (vector<vector<Site>> sm, unordered_map<string, Cell> cm,
		unordered_map<string, Net> nm) {
	cout << sm[0][0] << endl;
	auto it_cell = cm.begin();
	cout << it_cell->first << " : " << it_cell->second << endl;
	auto it_net = nm.begin();
	cout << it_net->first << " : " << it_net->second << endl;	
}


void test_site_creation () {
	Site site0 = Site(0, 0, "CLB");
	Cell cell0 = Cell("cell0", "CLB", 0, 0, "M");
	site0.addCell( cell0.getName() );
	
	Cell cell1 = Cell("cell1", "CLB", 0, 1, "M");
	Site site1 = Site(0, 1, "CLB", cell1.getName());

	cout << site0 << endl;
	cout << site1 << endl;
}

void test_placement_creation (Placement placement, 
		unordered_map<string, Cell> cellMap, int x, int y) {
	Site site00 = placement.getSite(x, y);
	string cellName = site00.getCellName();
	Cell cell00 = Cell(cellMap[ cellName ]);
	cout << "Cell Name: " << cellName << endl;
	cout << site00 << " : " << cell00 << endl;		
}


void lp_algorithm (Placement placement, 
		unordered_map<string, Cell> cellMap, unordered_map<string, Net> netMap) {
	// Start by picking an arbitrary row. Let's pick the first one
	int row = 0;
	vector<Site> siteRow = placement.getRow(row);
			
}

int lp_cost () {
	return 0;
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
	cout << cellMap["cell_0"] << endl;

	
	// Initialize placement of FPGA sites
	int rows = sitemap.size();
	int cols = sitemap[0].size();
	Placement placement = Placement(rows, cols, sitemap);
	placement.addCells( cellMap.begin(), cellMap.end() );	

	// Run Algorithm
//	linear_placement_algorithm(placement, cellMap, netMap);
	
}

