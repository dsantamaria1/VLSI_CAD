import sys, os, math
from collections import defaultdict

class Cell:
    def __init__(self, name, x, y, type, fixed):
        self.name = name
        self.x = int(x)
        self.y = int(y)
        self.type = type
        if fixed == "F":
            self.fixed = True
        else:
            self.fixed = False

    def __eq__(self, other):
        if isinstance(other, self.__class__):
            return self.name == other.name and \
                   self.type == other.type and \
                   self.x == other.x and \
                   self.y == other.y
        else:
            return False

    def __str__(self):
        return "Cell Name: %s, Cell Type: %s, X location: %d, Y location: %d, Fixed: %r" % \
               (self.name, self.type, self.x, self.y, self.fixed)


class Net:
    # attributes: name (Name of Net), cellList (list of cells in net),
    # boundingBox(4 points corresponding to this net's boundingBox)
    def __init__(self, name, cellList):
        self.name = name
        self.cellList = cellList[:]
        self.boundingBox = self.getBoundingBox()
        self.hpwl = self.getHPWL()

    def __str__(self):
        return "Net Name: %s, Cells: %s" % \
               (self.name, ';    '.join(map(str, self.cellList)))

    def getBoundingBox(self):
        bbox = {}
        max_X = 0
        max_Y = 0

        fileName = os.getcwd() + "/benchmarks/FPGA-example1/FPGA-example1.sitemap"
        fp = open(fileName, "r")
        line1 = fp.readline()
        #print line1
        min_X, min_Y = line1.split(" ")
        fp.close()


        for cell in self.cellList:
            if cell.x > max_X:
                max_X = cell.x
            if cell.y > max_Y:
                max_Y = cell.y
            if cell.x < min_X:
                min_X = cell.x
            if cell.y < min_Y:
                min_Y = cell.y

        bbox["SW"] = (min_X, min_Y)
        bbox["NW"] = (min_X, max_Y)
        bbox["SE"] = (max_X, min_Y)
        bbox["NE"] = (max_X, max_Y)
        return bbox

    def getHPWL(self):
        pt1 = self.boundingBox["SW"]
        pt2 = self.boundingBox["NW"]
        height = self.calcLength(pt1, pt2)
        pt1 = self.boundingBox["SW"]
        pt2 = self.boundingBox["SE"]
        width = self.calcLength(pt1, pt2)
        hpwl = height + width
        #print "HPWL is %d" % hpwl
        return hpwl

    def calcLength(self, pt1, pt2):
        xDiff = pt2[0] - pt1[0]
        yDiff = pt2[1] - pt1[1]
        xSquared = math.pow(xDiff, 2)
        ySquared = math.pow(yDiff, 2)
        distance = math.sqrt(xSquared + ySquared)

        return distance


def getPlacement():
    cellDict = {}
    if len(sys.argv) < 2:
        print "Using default FPGA-example1.pl file"
        fileName = os.getcwd() + "/benchmarks/FPGA-example1/FPGA-example1.pl"
    else:
        fileName = sys.argv[1]

    fp = open(fileName, "r")
    for line in fp:
        line = line.rstrip()
        name, type, x, y, fixed = line.split(" ")
        cellDict[name] = Cell(name, x, y, type, fixed)
    fp.close()

    return cellDict


def getNetlist(cellDictionary):
    netDict = defaultdict(list)
    if len(sys.argv) < 3:
        print "Using default FPGA-example.nets file"
        fileName = os.getcwd() + "/benchmarks/FPGA-example1/FPGA-example1.nets"
    else:
        fileName = sys.argv[2]

    fp = open(fileName, "r")
    for line in fp:
        line = line.rstrip()
        cells = line.split(" ")
        netName = cells.pop(0)
        newCellList = []
        for c in cells:
            cell = cellDictionary[c]
            newCellList.append(cell)

        netDict[netName] = Net(netName, newCellList[:])

    fp.close()
    return netDict


cellDict = getPlacement()
netDict = getNetlist(cellDict)
# for k in netDict.keys():
#     print "HPWL is %d \t for %s" % (netDict[k].hpwl, k)

totalWL = 0.0
for v in netDict.values():
    totalWL += v.hpwl

print "Total Wirelength is %f" % (totalWL)
