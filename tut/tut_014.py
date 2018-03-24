#!C:/Python27/python.exe

#see file:///D:/Python22/Doc/tut/node6.html
#4.6

def fib(n):    # write Fibonacci series up to n
    """Print a Fibonacci series up to n."""
    a, b = 0, 1
    while b < n:
        print b,
        a, b = b, a+b

# Now call the function we just defined:
fib(2000)

print
print fib
f = fib
f(100)
print
print fib(0)

def fib2(n): # return Fibonacci series up to n
    """Return a list containing the Fibonacci series up to n."""
    result = []
    a, b = 0, 1
    while b < n:
        result.append(b)    # see below
        a, b = b, a+b
    return result

f100 = fib2(100)    # call it
print f100                # write the result
