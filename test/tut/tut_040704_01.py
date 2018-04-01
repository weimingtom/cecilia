#!C:/Python27/python.exe

#see file:///D:/Python22/Doc/tut/node6.html
#4.7.4

def make_incrementor(n):
    return lambda x: x + n

f = make_incrementor(42)
print f(0)
print f(1)
