#!C:/Python27/python.exe

#see file:///D:/Python22/Doc/tut/node7.html
#5.3

t = 12345, 54321, 'hello!'
print t[0]
print t

# Tuples may be nested:
u = t, (1, 2, 3, 4, 5)
print u


empty = ()
singleton = 'hello',    # <-- note trailing comma
print len(empty)
print len(singleton)
print singleton

x, y, z = t
print x
print y
print z
