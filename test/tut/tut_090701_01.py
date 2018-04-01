#!C:/Python27/python.exe

#see file:///D:/Python22/Doc/tut/node11.html
#9.7.1

#raise Class, instance
#raise instance
#raise instance.__class__, instance

class B:
    pass
class C(B):
    pass
class D(C):
    pass

for c in [B, C, D]:
    try:
        raise c()
    except D:
        print "D"
    except C:
        print "C"
    except B:
        print "B"
        