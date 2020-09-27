class Esp:
    def __init__(self, id = None, angle = None, sen = None, cos = None, tan = None):
        self.id = id
        self.angle = angle
        self.sen = sen
        self.cos = cos
        self.tan = tan

    def updateAll(self, id, angle, sen, cos, tan):
        self.id = id
        self.angle = angle
        self.sen = sen
        self.cos = cos
        self.tan = tan
    
    def printHome(self):
        msg = "id: " + self.id + "\n    ang: " + self.angle + "\n    sen: " + self.sen + "\n    cos: " + self.cos + "\n    tan: " + self.tan + "\n\n"
        return msg