#!C:/Python27/python.exe

#see file:///D:/Python22/Doc/tut/node10.html
#8.3

import sys

for arg in sys.argv[1:]:
    try:
        f = open(arg, 'r')
    except IOError:
        print 'cannot open', arg
    else:
        print arg, 'has', len(f.readlines()), 'lines'
        f.close()
