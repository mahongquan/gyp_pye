from ctypes import *
adlinklib=windll.LoadLibrary("adlink/lib/pci-dask.dll")
AD_B_10_V=1
PCI_9111HR=21
PCI_6208V=1
def BitIsSet(Number,Bit):
    return (Number >> Bit & 0x01);
def setBitOff(DO_Value,channel):
    if(channel > 15):
        return
    v =int(1 << channel)
    DO_Value=DO_Value.value
    DO_Value = (v ^ 0xFFFFFFFF) & (DO_Value)
    return c_uint(DO_Value)
def setBitOn(DO_Value,channel):
    if(channel > 15):
        return
    v = int(1 << channel);
    DO_Value=DO_Value.value
    DO_Value = DO_Value | v;
    return c_uint(DO_Value)
adlinklib.Register_Card.restype=c_short
class Adlink:
    def __init__(self):
        self.DO_Value=c_uint(0)
        self.link()
        print(self.card)
        pass
    def link(self):
        card_type=c_ushort(PCI_9111HR)#9111
        card_num=c_ushort(0)
        self.card=adlinklib.Register_Card(card_type,card_num)
        card_type=c_ushort(PCI_6208V)
        self.card6208=adlinklib.Register_Card(card_type,card_num)
        return self.card
    def getdi(self):
        Value=c_uint(0)
        Port=c_ushort(0)
        r=c_ushort(0)
        Port=c_ushort(0)
        r=adlinklib.DI_ReadPort(self.card,Port,byref(Value))
        return(Value)
    def getdo(self):
        Value=c_uint(0)
        Port=c_ushort(0)
        r=c_ushort(0)
        Port=c_ushort(0)
        r=adlinklib.DO_ReadPort(self.card,Port,byref(Value))
        return(Value)
    def outputDO(self):
        Port =c_ushort(0)# P9111_CHANNEL_DO;
        err = adlinklib.DO_WritePort(self.card, Port, self.DO_Value)#, U16 Port, U32 Value
        if (err != 0):
            print("write port error="+str(err))
    def openVlV(self,channel):
        self.DO_Value=setBitOn(self.DO_Value,channel)
        self.outputDO()
    def closeVlV(self,channel):
        self.DO_Value=setBitOff(self.DO_Value,channel)
        self.outputDO()
    def getVolt(self,channel):
        Channel = c_ushort(channel)
        voltage=c_double(0)
        AdRange = AD_B_10_V
        err = adlinklib.AI_VReadChannel(self.card, Channel, AdRange ,byref(voltage))
        return voltage
    def putVolt(self,channel,voltage):
        Channel = c_ushort(channel)
        Voltage=c_double(voltage)
        err = adlinklib.AO_VWriteChannel(self.card6208, Channel, Voltage)
a=Adlink()
# print(a.getdi())
# print(a.getdo())
# a.DO_Value=setBitOn(a.DO_Value,0)
# a.DO_Value=setBitOn(a.DO_Value,2)
# a.DO_Value=setBitOff(a.DO_Value,2)
# print(a.DO_Value)
#a.closeVlV(1)
print(a.getVolt(0))
#a.putVolt(0,-10)

