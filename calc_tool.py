import math

def cook(p0, p1024):
    R0 = p0*131072
    R1024 = p1024*131072
    print('k = '+str(round(R0*8)))
    print('g = '+str(round(4*R1024 - R0*4)))

def specheat(i):
    print('s = '+str(round(1024 / i)))

def size(x):
    print(str(round(math.log(x, 2), 2)) + ' bits')
