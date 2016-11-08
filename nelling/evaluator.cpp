/*
	Evalutes key parameters of placement, nets, and sitemap files

	[Function]
	Given a netlist (.nets), placement (.pl) file, 
	and sitemap (.sitemap) file: 
	- determine if placement is legal based on sitemap
	- calculate the half-parameter wire length (HPWL)
	- calculate the bin utilization based on slicing placement
		into a 6x6 and 12x12 grid
	
	[Glossary]
	.nets file -- list of nets with cells attached to net
	.pl file -- list of cells with cell type (CLB, DSP, RAM, IO), 
		x & y coordsinates, and whether the cell is movable or fixed   
	.sitemap file -- list of all of the possible (x,y) sites on an 
		FPGA chip, including details about what type of cells 
		can be placed in each location
	
	[Inputs]
	Benchmark Path -- path to benchmark .nets, .pl, and .sitemap files
*/

#include <string>
#include <iostream>
#include <fstream>
using namespace std;


class Sitemap {
	def __init__(self, rows, cols, sitemapList):
		self.rows = rows	// total rows
		self.cols = cols	// total cols
		self.matrix = []	// rows of cols matrix
		
		// Generate sitemap matrix of n rows of m cols
		for row in range(rows):
			rowList = sitemapList[row]
			//pprint.pprint(rowList), exit()
			self.matrix.append(rowList)

	def __repr__(self):
		sitemapGrid = ""
		for row in range(self.rows):
			//sitemapGrid += "{0:<4}".format(row)
			for col in range(self.cols):
				site = self.matrix[row][col]
				if (site.type == "CLB"): siteStr = "C"
				if (site.type == "DSP"): siteStr = "D"
				if (site.type == "RAM"): siteStr = "R"
				if (site.type == "IO"): siteStr = "P"
				if (site.type == "INVALID"): siteStr = "I"
				sitemapGrid += siteStr

			sitemapGrid += "\n"
		return(sitemapGrid)

	def get_row(self, row):
		return (self.matrix[row])		
	
	def get_site(self, row, col):
		return (self.matrix[row][col])

	def check_legality(self, placement):
		for row in range(self.rows):
			
			plRow = placement.get_row(row)	
			for cell in plRow:
				col = cell.x;  site = self.get_site(row,col)
				if (cell.type != site.type):
					print("Placement is illegal! Check if cell & site types match.")
					return
		print("Placement is legal. Cell types match site types.")
}


class Placement {
	def __init__(self, rows, cols, placementList):
		self.rows = rows	// total rows
		self.cols = cols	// total rows
		self.matrix = []	// rows of cols matrix 
		self.validRows = []	// rows with cells on them

		// Generate placement matrix of n rows of m cols
		for row in range(rows):
			rowList = placementList[row]
			self.matrix.append(rowList)							
	
		//validRows = [self.matrix[i] for i in range(rows) if self.matrix[i]]

	def get_row(self, row):
		return (self.matrix[row])		

	def check_overlap(self):
		for row in range(self.rows):
			
			plRow = placement.get_row(row)	
			lastCol = None
			for cell in plRow:
				currCol = cell.x
				if (lastCol == currCol):
					print("Cell overlap found! Legalize placement.")
					return
				lastCol = currCol
		print("No overlap was found between cells.")

	def calculate_hpwl(self, netDict, cellDict):
		HPWL = 0

		for net in netDict.keys():
			x_min = maxColumn; x_max = 0
			y_min = maxRow; y_max = 0
		
			for cellName in (netDict.get(net)).cells:
				cell = cellDict.get(cellName)
				
				// Update x min and max
				if cell.x > x_max:
					x_max = cell.x
				if cell.x < x_min:
					x_min = cell.x
			
				// Update y min and max 
				if cell.y > y_max:
					y_max = cell.y
				if cell.y < y_min:
					y_min = cell.y
		
			// Sum the HPWL from each net in the netlist
			HPWL += (x_max - x_min) + (y_max - y_min)
		
		return HPWL	
}


////======================////
////======================////
////                      ////
//// Benchmark Evaluation ////
////                      ////
////======================////
////======================////
// Separate classes and script core functionality
if (__name__ == "__main__"):	
	
	print "=" * 72
	
	//==================//
	// Handle pathnames //
	//==================//
	// TODO: Convert hard-code paths to flexible paths with argument handling
	
	// inputPath = sys.argv[1]
	// scriptDir = os.path.dirname(os.path.realpath('__file__'))
	// localDir = os.getcwd()
	// 
	// if os.path.isabs(inputPath):
	// 	benchmarkPath = inputPath
	// else:	
	// 	benchmarkPath = os.path.join(localDir, inputPath)
	// 
	// //print("Path Name: " + benchmarkPath)
	// benchmark = os.path.basename(benchmarkPath)
	// //print("Benchmark: " + benchmark)
	// exit()
	// 
	// if not(os.path.isdir(benchmarkPath)): 
	// 	raise NotADirectoryError('Input a directory path, not a file path.')
	// 
	
	benchmarksPath = '/Users/nathanelling/Dropbox/School/Classes/Fall2016/EE382M-21/Project/Examples/benchmarks'
	benchmark = 'FPGA-example1'
	benchmarkDir = os.path.join(benchmarksPath, benchmark)
	
	
	//=====================//
	// Parse .sitemap file //
	//=====================//
	sitemapFile = os.path.join(benchmarkDir, benchmark + ".sitemap")
	print("Parsing .sitemap file: {}".format(sitemapFile))
	
	with open(sitemapFile, 'r') as f:
			dimensions = str.split(f.readline())
			maxColumn = int(dimensions[0])
			maxRow = int(dimensions[1])
			siteMapList = []
	        
			for row in range(maxRow):
					siteMapRow = []
	                
					for col in range(maxColumn):
							values = str.split(f.readline())
							siteType = values[2]
							siteMapRow.append(Site(col, row, siteType))
	
					siteMapList.append(siteMapRow)
			
			sitemap = Sitemap(maxRow, maxColumn, siteMapList)		
	
	
	//================//
	// Parse .pl file //
	//================//
	plFile = os.path.join(benchmarkDir, benchmark + ".pl")
	print("Parsing .pl file: {}".format(plFile))
	
	with open(plFile, 'r') as f:
		cellList = []
	
		for cellDef in f:
			values = str.split(cellDef)
			(name, type, x, y, fixed) = values
			cellList.append(Cell(name, type, int(x), int(y), fixed))	
	
	
	cellDict = {cell.name: cell for cell in cellList}	
	sortedCellList = sorted(cellList, key=attrgetter('y', 'x'), reverse=True)
	
	placementList = [[] for i in range(maxRow)]
	for row in range(maxRow):
		while (len(sortedCellList) and sortedCellList[-1].y == row):
			placementList[row].append(sortedCellList.pop())
		if (len(sortedCellList) == 0): break
	
	
	//pprint.pprint(placementList)
	placement = Placement(maxRow, maxColumn, placementList)
	
	
	//==================//
	// Parse .nets file //
	//==================//
	netsFile = os.path.join(benchmarkDir, benchmark + ".nets") 
	print("Parsing .nets file: {}".format(netsFile))
	
	with open(netsFile, 'r') as f:
		netList = []
		
		for netDef in f:
			values = str.split(netDef)
			(name, cells) = (values[0], values[1:])
			netList.append(Net(name, cells))
	
	netDict = {net.name: net for net in netList}	
	//pprint.pprint(netList)
	print
	
	
	//===========================//
	// Verify placement legality //
	//===========================//
	placement.check_overlap()
	sitemap.check_legality(placement)
	print
	
	
	//================//
	// Calculate HPWL //
	//================//
	HPWL = placement.calculate_hpwl(netDict, cellDict)
	print("HPWL: {}".format(HPWL))
	print "=" * 72	


	//===============//
	// General Tests //
	//===============//
	print(placement.validRows) 
	print sitemap	

