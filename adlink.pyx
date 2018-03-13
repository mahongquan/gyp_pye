from libc.stdlib cimport atoi
from libc.math cimport sin
cimport adlinklib
cdef double f(double x):
    return sin(x*x)
cdef parse_charptr_to_py_int(char* s):
    assert s is not NULL, "byte string value is NULL"
    return atoi(s)   # note: atoi() has no error detection!
def say_hello_to(name):
    print("Hello %s!" % name)
def sin2(name):
    return f(name)
class Adlink:
    def __init__(self):
        pass
    def link(self):
        cdef unsigned int card_type, card_num
        card_type=21#9111
        card_num=0
        self.card=adlinklib.Register_Card(card_type,card_num)
        return self.card
    def getdi(self):
        cdef unsigned long Value
        cdef unsigned int Port,r
        Port=0
        r=adlinklib.DI_ReadPort(self.card,Port,&Value)
        return(Value)

