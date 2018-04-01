#!C:/Python27/python.exe

#see https://wiki.python.org/moin/SimplePrograms
#10 lines: Time, conditionals, from..import, for..else

def sorted(x):
    x2 = x[:]
    x2.sort()
    return x2

from time import localtime

activities = {8: 'Sleeping',
              9: 'Commuting',
              17: 'Working',
              18: 'Commuting',
              20: 'Eating',
              22: 'Resting' }

time_now = localtime()
hour = time_now.tm_hour

for activity_time in sorted(activities.keys()):
    if hour < activity_time:
        print activities[activity_time]
        break
else:
    print 'Unknown, AFK or sleeping!'
    

