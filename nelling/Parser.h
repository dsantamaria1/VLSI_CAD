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
	void redefine_parser(const string& _filepath, const string& _benchmark);
	unordered_map<string, Net> parse_netlist();
	unordered_map<string, Cell> parse_placement();
	vector<vector<Site>> parse_sitemap();
private:
	string _netsPath;
	string _plPath;
	string _smPath;
};
