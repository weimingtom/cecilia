#!C:/Python27/python.exe

#see file:///D:/Python22/Doc/tut/node7.html
#5.1.3

def f(x): return x % 2 != 0 and x % 3 != 0
print filter(f, range(2, 25))

def cube(x): return x*x*x
print map(cube, range(1, 11))

seq = range(8)
def square(x): return x*x
print map(None, seq, map(square, seq))

def add(x,y): return x+y
print reduce(add, range(1, 11))

def sum(seq):
    def add(x,y): return x+y
    return reduce(add, seq, 0)
print sum(range(1, 11))
print sum([])
