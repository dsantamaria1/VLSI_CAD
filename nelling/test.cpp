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


void test_parser_objects (vector<vector<Site>> sm, unordered_map<string, Cell> cm,
		unordered_map<string, Net> nm) {
	cout << sm[0][0] << endl;
	auto it_cell = cm.begin();
	cout << it_cell->first << " : " << it_cell->second << endl;
	auto it_net = nm.begin();
	cout << it_net->first << " : " << it_net->second << endl;	
}


int main (int argc, char* argv[]) {
	string filepath = argv[1];
	string filename = argv[2];
	Parser parser = Parser(filepath, filename);
	vector<vector<Site>> sitemap = parser.parseSitemap();	
	unordered_map<string, Cell> cellMap = parser.parsePlacement();
	unordered_map<string, Net> netMap = parser.parseNetlist();
	
	int rows = sitemap.size();
	int cols = sitemap[0].size();
	Placement placement = Placement(rows, cols, sitemap);	
	//placement.addCells(cellMap);

	Site site00 = placement.getSite(0, 0);
	//Cell cell00 = Cell(cellMap[ site00.getCellName() ]);
	//cout << site00 << " : " << cell00;	
}

