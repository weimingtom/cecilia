#!C:/Python27/python.exe

#see file:///D:/Python22/Doc/tut/node5.html
#3.1.2

print 'spam eggs'
print 'doesn\'t'
print "doesn't"
print '"Yes," he said.'
print "\"Yes,\" he said."
print '"Isn\'t," she said.'
'"Isn\'t," she said.'

hello = "This is a rather long string containing\n\
several lines of text just as you would do in C.\n\
    Note that whitespace at the beginning of the line is\
 significant."

print hello

hello = r"This is a rather long string containing\n\
several lines of text much as you would do in C."

print hello

print """
Usage: thingy [OPTIONS] 
     -h                        Display this usage message
     -H hostname               Hostname to connect to
"""

word = 'Help' + 'A'
print word
print '<' + word*5 + '>'

import string
print 'str' 'ing'                   #  <-  This is ok
print string.strip('str') + 'ing'   #  <-  This is ok
# print string.strip('str') 'ing'     #  <-  This is invalid

print word[4]
print word[0:2]
print word[2:4]

#word[0] = 'x' # TypeError
#word[:1] = 'Splat' #TypeError

print 'x' + word[1:]
print 'Splat' + word[4]

print word[:2]    # The first two characters
print word[2:]    # All but the first two characters

print word[:2] + word[2:]
print word[:3] + word[3:]

print word[1:100]
print word[10:]
print word[2:1]

print word[-1]     # The last character
print word[-2]     # The last-but-one character
print word[-2:]    # The last two characters
print word[:-2]    # All but the last two characters

print word[-0]     # (since -0 equals 0)

print word[-100:]
#print word[-10]    # error #IndexError: string index out of range

s = 'supercalifragilisticexpialidocious'
print len(s)
