#!C:/Python27/python.exe

#see file:///D:/Python22/Doc/tut/node10.html
#8.3

def this_fails():
    x = 1/0

try:
    this_fails()
except ZeroDivisionError, detail:
    print 'Handling run-time error:', detail

