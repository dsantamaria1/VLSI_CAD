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


void Parser::redefineParser(const string& _filepath, const string& _benchmark) {
	string basePath = _filepath + _benchmark;
	_netsPath = basePath + ".nets";
	_plPath = basePath + ".pl";
	_smPath = basePath + ".sitemap";
}


unordered_map<string, Net> Parser::parseNetlist() {
	unordered_map<string, Net> netMap;
	ifstream file;
	
	try {
		string line;
		vector<string> tokens;
		file.open(_netsPath);
		while ( getline(file, line) ) {
			boost::split(tokens, line, boost::is_any_of(" ")); 
			string name = tokens[0];
			tokens.erase(tokens.begin());
			netMap.emplace(name, Net(name,tokens));
		}
		file.close();
	} 
	catch (std::ifstream::failure e) {
		cerr << "Exception occurred during file handling." << endl;
	}

	return netMap;	
}


unordered_map<string, Cell> Parser::parsePlacement() {
	unordered_map<string, Cell> cellMap;
	ifstream file;
	
	try {
		string line;	
		vector<string> tokens;
		file.open(_plPath);
		while ( getline(file, line) ) {
			boost::split(tokens, line, boost::is_any_of(" "));
			string name = tokens[0];
			string type = tokens[1];
			int x = stoi(tokens[2]);
			int y = stoi(tokens[3]);
			string fixed = tokens[4];
			cellMap.emplace(name, Cell(name, type, x, y, fixed));
		}
		file.close();
	} 
	catch (std::ifstream::failure e) {
		cerr << "Exception occurred during file handling" << endl;
	}

	return cellMap;
}


vector<vector<Site>> Parser::parseSitemap() {
	vector<vector<Site>> sitemap;
	ifstream file;
	
	try {
		string line;
		vector<string> tokens;
		
		int cols, rows, x, y;
		string type;
		
		file.open(_smPath);
		getline(file, line);
		boost::split(tokens, line, boost::is_any_of(" "));
		cols = stoi(tokens[0]);
		rows = stoi(tokens[1]);
		
		for (int r = 0; r < rows; r++) {
			vector<Site> siteRow;
			for (int c = 0; c < cols; c++) {
				getline(file, line);
				boost::split(tokens, line, boost::is_any_of(" "));
				x = stoi(tokens[0]);
				y = stoi(tokens[1]);
				type = tokens[2];
				siteRow.emplace_back(Site(x, y, type));
			}
			sitemap.emplace_back(siteRow);
		}	
		file.close();
	} 
	catch (std::ifstream::failure e) {
		cerr << "Exception occurred during file handling" << endl;
	}	

	return sitemap;
}
