#!C:/Python27/python.exe

#see file:///D:/Python22/Doc/tut/node7.html
#5.7

print (1, 2, 3) < (1, 2, 4)
print [1, 2, 3] < [1, 2, 4]
print 'ABC' < 'C', 'C' < 'Pascal', 'Pascal' < 'Python'
print (1, 2, 3, 4) < (1, 2, 4)
print (1, 2) < (1, 2, -1)
print (1, 2, 3) == (1.0, 2.0, 3.0)
print (1, 2, ('aa', 'ab')) < (1, 2, ('abc', 'a'), 4)
