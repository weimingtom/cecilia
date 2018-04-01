#!C:/Python27/python.exe

#see https://wiki.python.org/moin/SimplePrograms
#7 lines: Dictionaries, generator expressions

prices = {'apple': 0.40, 'banana': 0.50}
my_purchase = {
    'apple': 1,
    'banana': 6}
grocery_bill = reduce(lambda x, y: x+y, [prices[fruit] * my_purchase[fruit] for fruit in my_purchase])
print 'I owe the grocer $%.2f' % grocery_bill
