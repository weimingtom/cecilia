#!C:/Python27/python.exe

#see https://wiki.python.org/moin/SimplePrograms
#3 lines: For loop, built-in enumerate function, new style formatting

friends = ['john', 'pat', 'gary', 'michael']
for i in range(len(friends)):
	name = friends[i]
	print "iteration %d is %s" % (i, name)

