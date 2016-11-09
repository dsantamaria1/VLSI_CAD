#include <string>
#include <vector>

#include "Net.h"

using namespace std;

Net::Net (string name, vector<string> cells) {
	_name = name;
	_cells = cells;
}


Net::Net (const Net& net) {
	_name = net._name;
	_cells = net._cells;
}
