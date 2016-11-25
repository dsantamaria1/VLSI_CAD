import sys, os, math, time
from collections import defaultdict

class Cell:
    def __init__(self, name, x, y, type, fixed):
        self.name = name
        self.x = int(x)
        self.y = int(y)
        self.type = type
        self.netNames = []
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

    def addNetNames(self, netName):
        self.netNames.append(netName)


class Net:
    # attributes: name (Name of Net), cellList (list of cells in net),
    # boundingBox(4 points corresponding to this net's boundingBox)
    def __init__(self, name, cellList):
        self.name = name
        self.cellList = cellList[:]
        self.xr = self.xl = self.yu = self.yl = 0
        self.getBoundingBox()
        self.hpwl = self.getHPWL()

    def __str__(self):
        return "Net Name: %s, Cells: %s" % \
               (self.name, ';    '.join(map(str, self.cellList)))

    def getHPWL(self):

        hpwl = (self.xr-self.xl)+(self.yu-self.yl)
        #print "HPWL is %d" % hpwl
        return hpwl

    def getBoundingBox(self):
        max_X = max_Y = 0
        min_X = min_Y = 1000000

        for cell in self.cellList:
            if cell.x > max_X:
                max_X = cell.x
            if cell.y > max_Y:
                max_Y = cell.y
            if cell.x < min_X:
                min_X = cell.x
            if cell.y < min_Y:
                min_Y = cell.y

        self.xr = max_X
        self.xl = min_X
        self.yl = min_Y
        self.yu = max_Y
        return max_X,min_X,max_Y,min_Y


    def getFixedBBox(self, cell2exclude):
        max_X = max_Y = 0
        min_X = min_Y = 1000000

        for cell in self.cellList:
            if cell.name == cell2exclude:
                continue
            if cell.x > max_X:
                max_X = cell.x
            if cell.y > max_Y:
                max_Y = cell.y
            if cell.x < min_X:
                min_X = cell.x
            if cell.y < min_Y:
                min_Y = cell.y
        return max_X, min_X, max_Y, min_Y


class Site:
    def __init__(self, attributes):
        self.x = int(attributes[0])
        self.y = int(attributes[1])
        self.type = attributes[2]
        self.cellName = ""

    def __str__(self):
        return "site at %d,%d has type %s" % \
               (self.x, self.y, self.type)


def createCellDict(path, name):
    cellDict = {}
    fileName = path + name + ".pl"

    fp = open(fileName, "r")
    for line in fp:
        line = line.rstrip()
        name, type, x, y, fixed = line.split(" ")
        cellDict[name] = Cell(name, x, y, type, fixed)
    fp.close()

    return cellDict


def createNetDict(cellDictionary, path, name):
    netDict = defaultdict(list)
    fileName = path + name + ".nets"

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


def createSiteMap(cellDictionary, path, name):
    fileName = path + name + ".sitemap"
    fp = open(fileName, "r")
    temp = fp.readline()
    temp = temp.rstrip()
    dims = temp.split(" ")
    siteMap = defaultdict(list)
    rows = int(dims[0])

    count = 0
    row = count/rows

    for line in fp:
        line = line.rstrip()
        siteAttributes = line.split(" ")
        currSite = Site(siteAttributes)
        siteMap[row].append(currSite)
        count+=1
        row = count/rows

    return siteMap


def getMedian(ptList):
    listLength = len(ptList)
    if(listLength&1):
        median = listLength/2 +1
    else:
        median = listLength/2 # TODO: can be more accurate
    ptList.sort()
    # print "list length = %d, median index = %d" % (len(ptList), median)
    # print "min=%d,max=%d,median=%d" % (ptList[0], ptList[-1],ptList[median])
    return ptList[median]


def testSwap(x, y, cell, netDict):
    oldHPWL = 0
    newHPWL = 0
    oldX = cell.x
    oldY = cell.y
    cell.x = x
    cell.y = y
    for n in cell.netNames:
        net = netDict[n]
        oldHPWL += net.getHPWL()
        net.getBoundingBox()
        newHPWL += net.getHPWL()

    if oldHPWL <= newHPWL:
        # undo swap
        cell.x = oldX
        cell.y = oldY
        for n in cell.netNames:
            net = netDict[n]
            net.getBoundingBox()
    # else:
    #     print "found reduction!!!"


def globalSwap(cellDict, netDict, siteMap):
    xl_list = []
    xr_list = []
    yu_list = []
    yl_list = []
    xMedians = []
    yMedians = []
    for c in cellDict.keys():
        currCell = cellDict[c]
        #print "%s at (%d,%d)" % (currCell.name,currCell.x,currCell.y)
        if(currCell.fixed):
            continue
        for n in currCell.netNames:
            net = netDict[n]
            # net.getBoundingBox() # Potential bug, something could update at wrong time
            # oldHPWL += net.getHPWL()
            currNetxr, currNetxl, currNetyu, currNetyl = net.getFixedBBox(c)
            xl_list.append(currNetxl)
            xr_list.append(currNetxr)
            yl_list.append(currNetyl)
            yu_list.append(currNetyu)
            # print "oldHPWL = %d" %oldHPWL
            # print "fixed HPWL = %d" % ((currNetxr - currNetxl)+(currNetyu-currNetyl))
        # find optimal region
        xMedians.append(getMedian(xl_list))
        xMedians.append(getMedian(xr_list))
        yMedians.append(getMedian(yu_list))
        yMedians.append(getMedian(yl_list))

        for x in xMedians:
            for y in yMedians:
                if siteMap[y][x].type != currCell.type:
                    continue
                if siteMap[y][x].cellName == "":
                    testSwap(x,y,currCell,netDict) # TODO: add an actual swap


        #check if new site is compatible with cell
        #set new cell x,y
        #get new hpwl
        #reverse cell x,y if new hpwl >= old hpwl
        #print "Should change %s at (%d,%d)" % (currCell.name, currCell.x, currCell.y)
        xl_list = []
        xr_list = []
        yu_list = []
        yl_list = []
        xMedians = []
        yMedians = []

################# Main #################
def main():
    start_time = time.time()
    if len(sys.argv) > 1:
        filePath = sys.argv[1]
        fileName = sys.argv[2]
    else:
        print "Exiting ..."
        sys.exit()

    cellDict = createCellDict(filePath, fileName)
    netDict = createNetDict(cellDict, filePath, fileName)
    siteMap = createSiteMap(cellDict,filePath,fileName)

    for k in cellDict.keys():
        x = cellDict[k].x
        y = cellDict[k].y
        cellType = cellDict[k].type

        if cellType != siteMap[y][x].type:
            print "not a match!!!!"
        else:
            # print "cell type: %s, site type: %s" % (cellType, siteMap[y][x].type)
            siteMap[y][x].cellName = k

    for n in netDict.keys():
        net = netDict[n]
        for c in net.cellList:
            c.addNetNames(n)

    totalWL = 0
    for k in netDict.keys():
        net = netDict[k]
        totalWL += net.getHPWL()
    print "Total Wirelength is %d" % totalWL

    globalSwap(cellDict, netDict, siteMap)

    totalWL = 0
    for k in netDict.keys():
        net = netDict[k]
        net.getBoundingBox()
        totalWL += net.getHPWL()
    print "Total Wirelength after global swap is %d" % totalWL

    newFileName = fileName+"_new.pl"
    target = open(newFileName, 'w')
    for c in cellDict.keys():
        cellData = cellDict[c]
        if cellData.fixed:
            movable = "F"
        else:
            movable = "M"
        wrData = cellData.name + " " + cellData.type + " " + str(cellData.x)+ " "\
                    + str(cellData.y) + " " + movable
        target.write(wrData)
        target.write("\n")
    target.close()

    end_time = time.time()
    print "Execution time: %s seconds" % (end_time - start_time)

if __name__ == "__main__":
    main()
