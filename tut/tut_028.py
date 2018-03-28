#!C:/Python27/python.exe

#see file:///D:/Python22/Doc/tut/node7.html
#5.5

knights = {'gallahad': 'the pure', 'robin': 'the brave'}
for k, v in knights.items():
    print k, v

questions = ['name', 'quest', 'favorite color']
answers = ['lancelot', 'the holy grail', 'blue']
for q, a in zip(questions, answers):
    print 'What is your %s?  It is %s.' % (q, a)

