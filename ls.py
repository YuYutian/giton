class LineString:
    def __init__(self):
        self.__point_list = []
    
    def addPoint(self, x, y):
        self.__point_list.append((x, y))
    
    def getPointList(self, ):
        return self.__point_list
        
    def getLength(self, ):
        import math
        index = 0
        start = None
        length = 0
        for item in self.__point_list:
            if index == 0:
                start = item
            else:
                length = length + math.sqrt((item[0]-start[0])**2 + (item[1]-start[1])**2)
                start = item
            index = index + 1
        return length
        
    def getPointNum(self, ):
        return len(self.__point_list)
        
    def isCoincident(self, ls):
        flag = True
        if ls.getPointNum() != self.getPointNum():
            return False
        for i in range(0, ls.getPointNum()):
            if (ls.getPointList()[i][0] != self.__point_list[i][0]) or (ls.getPointList()[i][1] != self.__point_list[i][1]):
                return False
        return True

        
    
a = LineString()
b = LineString()
a.addPoint(1, 2)
b.addPoint(1, 2)
a.addPoint(3, 4)
b.addPoint(3, 4)
print(a.getLength())
print(b.getPointList())
print(a.getPointNum())
print(b.isCoincident(a))
b.addPoint(4, 4)
print(b.isCoincident(a))
print(b.getLength())
