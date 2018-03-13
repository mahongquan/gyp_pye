from libc.stdlib cimport atoi
from libc.math cimport sin
cdef double f(double x):
    return sin(x*x)
cdef parse_charptr_to_py_int(char* s):
    assert s is not NULL, "byte string value is NULL"
    return atoi(s)   # note: atoi() has no error detection!
def say_hello_to(name):
    print("Hello %s!" % name)
def sin2(name):
    return f(name)
