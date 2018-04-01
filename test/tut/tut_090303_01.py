#!C:/Python27/python.exe

#see file:///D:/Python22/Doc/tut/node11.html
#9.3.3

class MyClass:
    "A simple example class"
    i = 12345
    def f(self):
        return 'hello world'
    def __init__(self):
        self.data = []

x = MyClass()
x.counter = 1
while x.counter < 10:
    x.counter = x.counter * 2
print x.counter
del x.counter
