
#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <getopt.h>
#include <fstream>
#include "Cell.h"
using namespace std;

void createCellMap(string);
//std::map<string, Cell> createCellMap(string);

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
                 std::cout << "sitemap chosen: " << sitemap << std::endl;
                 break;
            case 'n':
                 netlist = optarg;
                 std::cout << "netlist chosen: " << netlist << std::endl;
                 break;
            case 'p':
                 placement = optarg;
                 std::cout << "placement chosen: " << placement << std::endl;
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
    ifstream cellFile;
    cout << placementFile.c_str() <<endl;
    
    cellFile.open(placementFile.c_str());
    while (getline(cellFile,line)) {
      std::cout << line << '\n';
    }
    // return map;
}
