#!C:/Python27/python.exe

#see file:///D:/Python22/Doc/tut/node9.html
#7.2.1

#f=open('/tmp/workfile', 'w')
f=open('./tmp/workfile.txt', 'r')
print f.readline()
print f.readline()
print f.readline()
f.close()

f=open('./tmp/workfile.txt', 'r')
print f.readlines()
f.close()

f=open('./tmp/workfile2.txt', 'w')
f.write('This is a test\n')
f.close()

f=open('./tmp/workfile3.txt', 'r+')
f.write('0123456789abcdef')
f.seek(5)     # Go to the 6th byte in the file
print f.read(1)        
f.seek(-3, 2) # Go to the 3rd byte before the end
print f.read(1)
f.close()
#f.read() #ValueError: I/O operation on closed file
