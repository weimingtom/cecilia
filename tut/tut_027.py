#!C:/Python27/python.exe

#see file:///D:/Python22/Doc/tut/node7.html
#5.4

tel = {'jack': 4098, 'sape': 4139}
tel['guido'] = 4127
print tel
print tel['jack']
del tel['sape']
tel['irv'] = 4127
print tel
print tel.keys()
print tel.has_key('guido')

print dict([('sape', 4139), ('guido', 4127), ('jack', 4098)])
vec = [2, 4, 6]
print dict([(x, x**2) for x in vec])     # use a list comprehension
