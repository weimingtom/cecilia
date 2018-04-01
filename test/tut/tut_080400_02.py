#!C:/Python27/python.exe

#see file:///D:/Python22/Doc/tut/node10.html
#8.4

try:
    raise NameError, 'HiThere'
except NameError:
    print 'An exception flew by!'
    raise
