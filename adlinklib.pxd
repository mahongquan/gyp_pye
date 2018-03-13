cdef extern from "adlink/Include/Dask.h":
    int Register_Card(int PCI_9111HR,int card_num)
    int DI_ReadPort(int card,int  Port,unsigned long *Value)
