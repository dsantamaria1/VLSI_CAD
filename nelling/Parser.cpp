#include <string>
#include <vector>
#include <iostream>
#include <boost/algorithm/string.hpp>

#include "Parser.h"
#include "Cell.h"
#include "Site.h"
#include "Net.h"

using namespace std;


Parser::Parser(const string& _filepath, const string& _benchmark) {
	basePath = _filepath + _benchmark;
	netsPath = basePath + ".nets";
	plPath = basePath + ".pl";
	smPath = basePath + ".sitemap"
}

void Parser::redefine_parser(const string& _filepath, const string& _benchmark) {
	basePath = _filepath + _benchmark;
	netsPath = basePath + ".nets";
	plPath = basePath + ".pl";
	smPath = basePath + ".sitemap"
}

vector<Net> Parser::parse_netlist() {
	vector<Net> netlist;
	ifstream file;
	file.exceptions (std::ifstream::failbit | std::ifstream::badbit);
	
	try {
		string line;
		file.open(netsPath);
		while ( getline(file, line) ) {
			vector<string> tokens;
			boost::split(tokens, line, is_any_of(" ")); 
			string name = tokens[0];
			tokens.erase(tokens.begin());
			netlist.add(Net(name, tokens))	
		}
		file.close();
	} 
	catch (std::ifstream::failure e) {
		cerr << "Exception occured during file handling.";
	}

	return netlist;	
}

void Parser::parse_placement() {
	vector<vector<Cell>> placement;
	ifstream file;
	file.exceptions (std::ifstream::failbit | std::ifstream::badbit);
	string line;
	
	try {
		vector<string> tokens;
		file.open(plPath);
		while ( getline(file, line) ) {
			boost::split(tokens, line, is_any_of(" "));
			string name = tokens[0];
			string type = tokens[1];
			int x = tokens[2];
			int y = tokens[3];
			string fixed = tokens[4];
			cells.add(Cell(name, type, x, y, fixed));
		}
		file.close();
	} 
	catch (std::ifstream::failure e) {
		cerr << "Exception occured during file handling";

	return placement;
}

vector<vector<<Site>> Parser::parse_sitemap() {
	vector<vector<Site>> sitemap;
	ifstream file;
	file.exceptions (std::ifstream::failbit | std::ifstream::badbit);
	string line;
	
	try {
		vector<string> tokens;
		int col, rows, x, y;
		string type;
		
		file.open(smPath);
		getline(file, line);
		boost::split(tokens, line, is_any_of(" "));
		col = tokens[0];
		rows = tokens[1];
		
		vector<Site> siteRow(cols);
		for (int r = 0; r < rows; r++) {

			for (int c = 0; c < cols; c++) {
				boost::split(tokens, line, is_any_of(" "));
				x = blocks[0];
				y = blocks[1];
				type = blocks[2];
				siteRow.add(Site(x, y, type))
			}
			sitemap.add(siteRow);
		}	
		file.close();
	} 
	catch (std::ifstream::failure e) {
		cerr << "Exception occured during file handling";

	return sitemap;
}
