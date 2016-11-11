#include <string>
#include <vector>
#include <unordered_map>

#include "Cell.h"
#include "Net.h"
#include "Site.h"

using namespace std;

class Parser {
public:
	Parser(const string& _filepath, const string& _benchmark);
	void redefineParser(const string& _filepath, const string& _benchmark);
	unordered_map<string, Net> parseNetlist();
	unordered_map<string, Cell> parsePlacement();
	vector<vector<Site>> parseSitemap();
private:
	string _netsPath;
	string _plPath;
	string _smPath;
};
