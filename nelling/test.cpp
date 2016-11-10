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


int main (int argc, char* argv[]) {
	string filepath = argv[1];
	string filename = argv[2];
	Parser parser = Parser(filepath, filename);
	vector<vector<Site>> sitemap = parser.parseSitemap();	
	unordered_map<string, Cell> cellMap = parser.parsePlacement();
	unordered_map<string, Net> netMap = parser.parseNetlist();
	
	cout << sitemap[0][0] << endl;
	auto it_cell = cellMap.begin();
	cout << it_cell->first << " : " << it_cell->second << endl;
	auto it_net = netMap.begin();
	cout << it_net->first << " : " << it_net->second << endl;
}

