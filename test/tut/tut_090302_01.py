#!C:/Python27/python.exe

#see file:///D:/Python22/Doc/tut/node11.html
#9.3.2

class MyClass:
    "A simple example class"
    i = 12345
    def f(self):
        return 'hello world'
    def __init__(self):
        self.data = []

x = MyClass()
print x.f()
print x.i
print x.data

class Complex:
    def __init__(self, realpart, imagpart):
        self.r = realpart
        self.i = imagpart

x = Complex(3.0, -4.5)
print x.r, x.i
