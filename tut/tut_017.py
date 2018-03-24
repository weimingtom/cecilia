#!C:/Python27/python.exe

#see file:///D:/Python22/Doc/tut/node6.html
#4.7.3

def fprintf(file, format, *args):
    file.write(format % args)

