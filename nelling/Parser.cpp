#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <boost/algorithm/string.hpp>

#include "Parser.h"
#include "Cell.h"
#include "Site.h"
#include "Net.h"

using namespace std;


Parser::Parser(const string& _filepath, const string& _benchmark) {
	string basePath = _filepath + _benchmark;
	_netsPath = basePath + ".nets";
	_plPath = basePath + ".pl";
	_smPath = basePath + ".sitemap";
}


void Parser::redefine_parser(const string& _filepath, const string& _benchmark) {
	string basePath = _filepath + _benchmark;
	_netsPath = basePath + ".nets";
	_plPath = basePath + ".pl";
	_smPath = basePath + ".sitemap";
}


unordered_map<string, Net> Parser::parse_netlist() {
	unordered_map<string, Net> netMap;
	ifstream file;
	file.exceptions (std::ifstream::failbit | std::ifstream::badbit);
	
	try {
		string line;
		file.open(_netsPath);
		while ( getline(file, line) ) {
			vector<string> tokens;
			boost::split(tokens, line, boost::is_any_of(" ")); 
			string name = tokens[0];
			tokens.erase(tokens.begin());
			Net net = Net(name, tokens);	
			netMap.emplace(name, net);
		}
		file.close();
	} 
	catch (std::ifstream::failure e) {
		cerr << "Exception occured during file handling.";
	}

	return netMap;	
}

/*
unordered_map<string, Cell> Parser::parse_placement() {
	unordered_map<string, Cell> cellMap;
	ifstream file;
	file.exceptions (std::ifstream::failbit | std::ifstream::badbit);
	string line;
	
	try {
		vector<string> tokens;
		file.open(_plPath);
		while ( getline(file, line) ) {
			boost::split(tokens, line, boost::is_any_of(" "));
			string name = tokens[0];
			string type = tokens[1];
			int x = tokens[2];
			int y = tokens[3];
			string fixed = tokens[4];
			cellMap[name] = Cell(name, type, x, y, fixed);
		}
		file.close();
	} 
	catch (std::ifstream::failure e) {
		cerr << "Exception occured during file handling";

	return cellMap;
}
*/
/*
vector<vector<<Site>> Parser::parse_sitemap() {
	vector<vector<Site>> sitemap;
	ifstream file;
	file.exceptions (std::ifstream::failbit | std::ifstream::badbit);
	string line;
	
	try {
		vector<string> tokens;
		int col, rows, x, y;
		string type;
		
		file.open(_smPath);
		getline(file, line);
		boost::split(tokens, line, boost::is_any_of(" "));
		col = tokens[0];
		rows = tokens[1];
		
		vector<Site> siteRow(cols);
		for (int r = 0; r < rows; r++) {

			for (int c = 0; c < cols; c++) {
				boost::split(tokens, line, boost::is_any_of(" "));
				x = blocks[0];
				y = blocks[1];
				type = blocks[2];
				siteRow[c] = Site(x, y, type);
			}
			sitemap.add(siteRow);
		}	
		file.close();
	} 
	catch (std::ifstream::failure e) {
		cerr << "Exception occured during file handling";

	return sitemap;
}
*/
