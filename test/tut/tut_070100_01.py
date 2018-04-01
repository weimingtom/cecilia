#!C:/Python27/python.exe

#see file:///D:/Python22/Doc/tut/node9.html
#7.1

s = 'Hello, world.'
print str(s)
print `s`
print str(0.1)
print `0.1`
x = 10 * 3.25
y = 200 * 200
s = 'The value of x is ' + `x` + ', and y is ' + `y` + '...'
print s
# Reverse quotes work on other types besides numbers:
p = [x, y]
ps = repr(p)
print ps
# Converting a string adds string quotes and backslashes:
hello = 'hello, world\n'
hellos = `hello`
print hellos
# The argument of reverse quotes may be a tuple:
print `x, y, ('spam', 'eggs')`

import string
for x in range(1, 11):
    print string.rjust(`x`, 2), string.rjust(`x*x`, 3),
    # Note trailing comma on previous line
    print string.rjust(`x*x*x`, 4)

import string
print string.zfill('12', 5)
print string.zfill('-3.14', 7)
print string.zfill('3.14159265359', 5)

import math
print 'The value of PI is approximately %5.3f.' % math.pi
table = {'Sjoerd': 4127, 'Jack': 4098, 'Dcab': 7678}
for name, phone in table.items():
    print '%-10s ==> %10d' % (name, phone)

table = {'Sjoerd': 4127, 'Jack': 4098, 'Dcab': 8637678}
print 'Jack: %(Jack)d; Sjoerd: %(Sjoerd)d; Dcab: %(Dcab)d' % table
