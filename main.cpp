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


void linear_placement_algorithm (Placement placement,
		unordered_map<string, Cell> cellMap, unordered_map<string, Net> netMap) {

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

	// for(auto it = cellMap.begin(); it != cellMap.end(); ++it){
	// 	cout << it->second << endl;
	// }


//  Run Algorithm
for(){

}
//	linear_placement_algorithm(placement, cellMap, netMap);

}
