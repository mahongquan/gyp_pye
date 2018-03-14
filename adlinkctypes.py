from ctypes import *
adlinklib=windll.LoadLibrary("adlink/lib/pci-dask.dll")
class Adlink:
    def __init__(self):
        pass
    def link(self):
        card_type=c_uint(21)#9111
        card_num=c_uint(0)
        self.card=adlinklib.Register_Card(card_type,card_num)
        return self.card
    def getdi(self):
        Value=c_ulong(0)
        Port=c_uint(0)
        r=c_uint(0)
        Port=c_uint(0)
        r=adlinklib.DI_ReadPort(self.card,Port,byref(Value))
        return(Value)
    def getdo(self):
        Value=c_ulong(0)
        Port=c_uint(0)
        r=c_uint(0)
        Port=c_uint(0)
        r=adlinklib.DO_ReadPort(self.card,Port,byref(Value))
        return(Value)
a=Adlink()
a.link()
print(a.getdi())
print(a.getdo())

