#!C:/Python27/python.exe

#see file:///D:/Python22/Doc/tut/node5.html
#3.1.1

print 2+2
# This is a comment
print 2+2
print 2+2  # and a comment on the same line as code
print (50-5*6)/4
# Integer division returns the floor:
print 7/3
print 7/-3

width = 20
height = 5*9
print width * height
900

x = y = z = 0  # Zero x, y and z
print x
print y
print z

print 3 * 3.75 / 1.5
print 7.0 / 2

print 1j * 1J
print 1j * complex(0,1)
print 3+1j*3
print (3+1j)*3
print (1+2j)/(1+1j)
(1.5+0.5j)

a=1.5+0.5j
print a.real
print a.imag
0.5

a=3.0+4.0j
# print float(a) # throw TypeError
print a.real
print a.imag
print abs(a)  # sqrt(a.real**2 + a.imag**2)

tax = 12.5 / 100
price = 100.50
_ = price * tax
print _
_ = price + _
print _
print round(_, 2)
