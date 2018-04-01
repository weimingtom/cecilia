#!C:/Python27/python.exe

#see file:///D:/Python22/Doc/tut/node6.html
#4.1

x = int(raw_input("Please enter an integer: "))
if x < 0:
    x = 0
    print 'Negative changed to zero'
elif x == 0:
    print 'Zero'
elif x == 1:
    print 'Single'
else:
    print 'More'
