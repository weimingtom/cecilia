#!C:/Python27/python.exe

#see file:///D:/Python22/Doc/tut/node5.html
#3.1.3

print u'Hello World !'
print u'Hello\u0020World !'
print ur'Hello\u0020World !'
print ur'Hello\\u0020World !'
print u"abc"
print str(u"abc")
# print u"??จน" # UnicodeError: ASCII encoding error: ordinal not in range(128)
# print str(u"??จน") # UnicodeError: ASCII encoding error: ordinal not in range(128)
print u"??จน".encode('utf-8')
# print unicode('\xc3\xa4\xc3\xb6\xc3\xbc', 'utf-8') #UnicodeError: ASCII encoding error: ordinal not in range(128)
