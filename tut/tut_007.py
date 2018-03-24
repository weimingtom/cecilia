#!C:/Python27/python.exe

#see file:///D:/Python22/Doc/tut/node5.html
#3.1.4

a = ['spam', 'eggs', 100, 1234]
print a

print a[0]
print a[3]
print a[-2]
print a[1:-1]
print a[:2] + ['bacon', 2*2]
print 3*a[:3] + ['Boe!']

print a
a[2] = a[2] + 23
print a

# Replace some items:
a[0:2] = [1, 12]
print a
# Remove some:
a[0:2] = []
print a
# Insert some:
a[1:1] = ['bletch', 'xyzzy']
print a
a[:0] = a     # Insert (a copy of) itself at the beginning
print a

print len(a)

q = [2, 3]
p = [1, q, 4]
print len(p)
print p[1]
print p[1][0]
p[1].append('xtra')     # See section 5.1
print p
print q
