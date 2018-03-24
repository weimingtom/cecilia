#!C:/Python27/python.exe

#see file:///D:/Python22/Doc/tut/node4.html
#2.2.3

import os
filename = os.environ.get('PYTHONSTARTUP')
if filename and os.path.isfile(filename):
	execfile(filename)
