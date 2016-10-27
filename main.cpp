
#include <map>
#include <string>
#include <vector>
#include <stdlib.h>
#include <getopt.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include "Cell.hh"
using namespace std;

void createCellMap(string);
//map<string, Cell> createCellMap(string);

int main(int argc, char* argv[]) {
    int opts;
    int opterr = 0;
    string sitemap;
    string placement;
    string netlist;
    map<string, Cell> cellMap;
    map<string, vector<string> > netMap;

    while((opts = getopt(argc, argv, "s:n:p:")) != -1){
        switch (opts){
            case 's':
                 sitemap = optarg;
                 cout << "sitemap chosen: " << sitemap << endl;
                 break;
            case 'n':
                 netlist = optarg;
                 cout << "netlist chosen: " << netlist << endl;
                 break;
            case 'p':
                 placement = optarg;
                 cout << "placement chosen: " << placement << endl;
                 break;
            default:
                 printf("die");
                 break;
        }
    } // while (getopt)

    createCellMap(placement);
    //cellMap = createCellMap(placement);
    return 0;
}

//map<string, Cell> createCellMap(string placementFile){
void  createCellMap(string placementFile){
    string line;
    string cellName, cellType, x, y, fixed;
    ifstream cellFile;
    cout << placementFile.c_str() <<endl;

    cellFile.open(placementFile.c_str());
    while (getline(cellFile,line)) {
      istringstream iss(line);
      cout << line << endl;
      if(iss >> cellName >> cellType >> x >> y >> fixed){
        int xCoord= atoi(x.c_str());
        int yCoord= atoi(y.c_str());
       
        Cell cell = Cell(cellName, xCoord, yCoord, cellType, fixed);
      }
    }
    // return map;
}
