#include <string>
#include "Cell.h"

using namespace std;

class Parser {
public:
	Parser(const string& _filepath, const string& _benchmark)
	void redefine_parser(const string& _filepath, const string& _benchmark);
private:
	vector<Net> parse_netlist();
	vector<vector<Cell>> parse_placement();
	vector<vector<Site>> parse_sitemap();
	string netsPath;
	string plPath;
	string smPath;
};
