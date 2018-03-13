from libc.stdlib cimport atoi
from libc.math cimport sin
cimport cqueue
cdef class Queue:
    cdef cqueue.Queue* _c_queue
    def __cinit__(self):
        self._c_queue = cqueue.queue_new()
        if self._c_queue is NULL:
            raise MemoryError()
    def __dealloc__(self):
        if self._c_queue is not NULL:
            cqueue.queue_free(self._c_queue)
cdef double f(double x):
    return sin(x*x)
cdef parse_charptr_to_py_int(char* s):
    assert s is not NULL, "byte string value is NULL"
    return atoi(s)   # note: atoi() has no error detection!
def say_hello_to(name):
    print("Hello %s!" % name)
def sin2(name):
    return f(name)
