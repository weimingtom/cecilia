#!C:/Python27/python.exe

#see https://wiki.python.org/moin/SimplePrograms
#33 lines: "Guess the Number" Game (edited) from http://inventwithpython.com

import random

guesses_made = 0

name = raw_input('Hello! What is your name?\n')

number = random.randint(1, 20)
print 'Well, %s, I am thinking of a number between 1 and 20.' % (name)

while guesses_made < 6:

    guess = int(raw_input('Take a guess: '))

    guesses_made += 1

    if guess < number:
        print 'Your guess is too low.'

    if guess > number:
        print 'Your guess is too high.'

    if guess == number:
        break

if guess == number:
    print 'Good job, %s! You guessed my number in %d guesses!' % (name, guesses_made)
else:
    print 'Nope. The number I was thinking of was %d' % (number)
