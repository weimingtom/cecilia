#!C:/Python27/python.exe

#see https://wiki.python.org/moin/SimplePrograms
#9 lines: Opening files

def sorted(x):
    x2 = x[:]
    x2.sort()
    return x2

# indent your Python code to put into an email
import glob
# glob supports Unix style pathname extensions
python_files = glob.glob('*.py')
for file_name in sorted(python_files):
    print '    ------' + file_name

    try: 
        f = open(file_name)
        for line in f:
            print '    ' + line.rstrip()
    except IOError:
        pass
    else:
        f.close()

    print


