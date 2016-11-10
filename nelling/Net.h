#ifndef NET_H
#define NET_H

#include <string>
#include <vector>
#include <iostream>

using namespace std;

class Net {
public:
	Net (string name, vector<string> cellNames);
	Net (const Net& net);
	friend ostream& operator<< (ostream& os, const Net& net);
private:
	string _name;
	vector<string> _cellNames;	
};

#endif
