#!C:/Python27/python.exe

#see file:///D:/Python22/Doc/tut/node9.html
#7.2.2

import pickle

x = [1, 2, 3]
f=open('./tmp/workfile4.txt', 'w')
pickle.dump(x, f)
f.close()

f=open('./tmp/workfile4.txt', 'r')
x = pickle.load(f)
print x
