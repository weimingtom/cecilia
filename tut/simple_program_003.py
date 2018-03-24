#!C:/Python27/python.exe

#see https://wiki.python.org/moin/SimplePrograms
#3 lines: For loop, built-in enumerate function, new style formatting

friends = ['john', 'pat', 'gary', 'michael']
for i, name in enumerate(friends):
    print "iteration {iteration} is {name}".format(iteration=i, name=name)

