#!C:/Python27/python.exe

#see file:///D:/Python22/Doc/tut/node6.html
#4.4

for n in range(2, 10):
	for x in range(2, n):
		if n % x == 0:
			print n, 'equals', x, '*', n/x
			break
	else:
		# loop fell through without finding a factor
		print n, 'is a prime number'

