#ifndef NET_H
#define NET_H

#include <string>
#include <vector>

#include "Cell.h"

using namespace std;

class Net {
public:
	Net (const string& name, const vector<Cell>& cells);
private:
	const string _name;
	vector<string> _cells;	
};

#endif
