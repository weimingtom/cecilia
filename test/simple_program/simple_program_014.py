#!C:/Python27/python.exe

#see https://wiki.python.org/moin/SimplePrograms
#14 lines: Doctest-based testing

def sorted(x):
    x2 = x[:]
    x2.sort()
    return x2

def median(pool):
    '''Statistical median to demonstrate doctest.
    >>> median([2, 9, 9, 7, 9, 2, 4, 5, 8])
    7
    '''
    copy = sorted(pool)
    size = len(copy)
    if size % 2 == 1:
        return copy[(size - 1) / 2]
    else:
        return (copy[size/2 - 1] + copy[size/2]) / 2
if __name__ == '__main__':
    import doctest, sys
    doctest.testmod(sys.modules[__name__]) 
    #see https://stackoverflow.com/questions/990422/how-to-get-a-reference-to-current-modules-attributes-in-python
