#!C:/Python27/python.exe

#see https://wiki.python.org/moin/SimplePrograms
#8 lines: Command line arguments, exception handling

# This program adds up integers in the command line
import sys
try:
    total = reduce(lambda x, y: x+y, [int(arg) for arg in sys.argv[1:]])
    print 'sum =', total
except (ValueError, TypeError):
    print 'Please supply integer arguments'

