#!C:/Python27/python.exe

#see file:///D:/Python22/Doc/tut/node10.html
#8.3

try:
    spam()
except NameError, x:
    print 'name', x, 'undefined'
