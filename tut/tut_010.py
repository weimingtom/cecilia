#!C:/Python27/python.exe

#see file:///D:/Python22/Doc/tut/node6.html
#4.2

# Measure some strings:
a = ['cat', 'window', 'defenestrate']
for x in a:
    print x, len(x)

for x in a[:]: # make a slice copy of the entire list
   if len(x) > 6: a.insert(0, x)

print a
