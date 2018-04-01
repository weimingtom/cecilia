#!C:/Python27/python.exe

#see file:///D:/Python22/Doc/tut/node10.html
#8.5

class MyError(Exception):
    def __init__(self, value):
        self.value = value
    def __str__(self):
        return `self.value`

try:
    raise MyError(2*2)
except MyError, e:
    print 'My exception occurred, value:', e.value

raise MyError, 'oops!'
