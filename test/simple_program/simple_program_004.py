#!C:/Python27/python.exe

#see https://wiki.python.org/moin/SimplePrograms
#4 lines: Fibonacci, tuple assignment

parents, babies = (1, 1)
while babies < 100:
    print 'This generation has %d babies' % babies
    parents, babies = (babies, parents + babies)
