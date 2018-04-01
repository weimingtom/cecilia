#!C:/Python27/python.exe

#see file:///D:/Python22/Doc/tut/node10.html
#8.3

while 1:
    try:
        x = int(raw_input("Please enter a number: "))
        break
    except ValueError:
        print "Oops! That was no valid number.  Try again..."
    except (RuntimeError, TypeError, NameError):
        pass
