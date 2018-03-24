#!C:/Python27/python.exe

#see file:///D:/Python22/Doc/tut/node6.html
#4.7.2

def parrot(voltage, state='a stiff', action='voom', type='Norwegian Blue'):
    print "-- This parrot wouldn't", action,
    print "if you put", voltage, "Volts through it."
    print "-- Lovely plumage, the", type
    print "-- It's", state, "!"

parrot(1000)
parrot(action = 'VOOOOOM', voltage = 1000000)
parrot('a thousand', state = 'pushing up the daisies')
parrot('a million', 'bereft of life', 'jump')

# parrot()                     # required argument missing
# parrot(voltage=5.0, 'dead')  # non-keyword argument following keyword
# parrot(110, voltage=220)     # duplicate value for argument
# parrot(actor='John Cleese')  # unknown keyword


def function(a):
    pass

# function(0, a=0) #TypeError: function() got multiple values for keyword argument 'a'

def cheeseshop(kind, *arguments, **keywords):
    print "-- Do you have any", kind, '?'
    print "-- I'm sorry, we're all out of", kind
    for arg in arguments: print arg
    print '-'*40
    keys = keywords.keys()
    keys.sort()
    for kw in keys: print kw, ':', keywords[kw]

cheeseshop('Limburger', "It's very runny, sir.",
           "It's really very, VERY runny, sir.",
           client='John Cleese',
           shopkeeper='Michael Palin',
           sketch='Cheese Shop Sketch')

  
  
