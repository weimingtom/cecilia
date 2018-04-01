#!C:/Python27/python.exe

#see file:///D:/Python22/Doc/tut/node11.html
#9.3.4

class MyClass:
    "A simple example class"
    i = 12345
    def f(self):
        return 'hello world'
    def __init__(self):
        self.data = []

x = MyClass()
print x.f()

xf = x.f
while 1:
    print xf()

