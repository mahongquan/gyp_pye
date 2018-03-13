#include <Python.h>
#include <windows.h>
#include "dask.h"
static PyObject *adlinkError;
static int value_;
static I16 obj_card;
static I16 obj_card6208;
static boolean obj_linked;

boolean BitIsSet(U32 Number, U16 Bit)
{
    return (Number >> Bit & 0x01);
}
void setChannelOff(U32 *DO_Value, U16 channel)
{
    if(channel > 15) return;
    U16 v = 1 << channel;
    *DO_Value = (v ^ 0xFFFFFFFF) & (*DO_Value);
}
void setChannelOn(U32 *DO_Value, U16 channel)
{
    if(channel > 15) return;
    U16 v = 1 << channel;
    *DO_Value = (*DO_Value) | (v);
}
I16 PutchanelVolt(U16 Channel, F64 voltage)
{
    I16 err = AO_VWriteChannel(obj_card6208, Channel, voltage);
    if (err != 0)
    {
        printf("write error=%d\n", err);
        return err;
    }
    return 0;
}//fun
F64 GetchanelVolt(U16 Channel)
{

    printf("Channel=%d\n", Channel);
    F64 voltage;
    U16 AdRange = AD_B_10_V;
    I16 err = AI_VReadChannel(obj_card, Channel, AdRange , &voltage);
    if (err != 0)
    {
        printf("read error=%d\n", err);
        return -1;
    }
    printf("volt=%f\n", voltage);
    return (voltage);
}
U32 GetDI()
{
    U32 Value;
    U16 Port = P9111_CHANNEL_DI;
    I16 err = DI_ReadPort(obj_card, Port, &Value);//U16 CardNumber, U16 Port, U32 *Value
    if (err != 0)
    {
        printf("read error=%d\n", err);
        return 0;
    }
    return(Value);
}//func
U32 GetDO()
{
    U32 Value;
    U16 Port = P9111_CHANNEL_DO;
    I16 err = DO_ReadPort(obj_card, Port, &Value);//U16 CardNumber, U16 Port, U32 *Value
    if (err != 0)
    {
        printf("read error=%d\n", err);
        return 0;
    }
    return(Value);
}//func
static   I16 Link()
{
    I16  err, card_num = 0;
    printf("card=%d\n", obj_card);
    obj_card = Register_Card (PCI_9111HR, card_num);
    if (obj_card < 0 )
    {
        printf("Register_Card error=%d", obj_card);
        //obj_linked = false;
        return obj_card;
    }
    else
    {
        printf("card=%d\n", obj_card);
    }
    err = AI_9111_Config(obj_card, TRIG_INT_PACER, 0, 1024);
    if (err != 0)
    {
        printf("AI_9111_Config error=%d", err);
        //obj_linked = false;
        return err;
    }
    printf("card2=%d\n", obj_card6208);
    obj_card6208 = Register_Card (PCI_6208V, card_num);
    if (obj_card6208 < 0 )
    {
        printf("Register_Card error=%d", obj_card6208);
        //obj_linked = false;
        return obj_card6208;
    }
    else
    {
        printf("card=%d", obj_card6208);
    }
    //obj_linked = true;
    return(obj_card);
}

static PyObject *adlink_system(PyObject *self, PyObject *args)
{
    const char *command;
    int sts;

    if (!PyArg_ParseTuple(args, "s", &command))
        return NULL;
    sts = system(command);
    return PyLong_FromLong(sts);
}
static PyObject *adlink_plusOne(PyObject *self, PyObject *args)
{
    const char *command;
    int para2 = 0;
    int sts;

    if (!PyArg_ParseTuple(args, "si", &command, &para2))
        return NULL;
    printf("%d\n", para2);
    value_ += para2;
    return PyLong_FromLong(value_);
}
static PyObject *adlink_GetchanelVolt (PyObject *self, PyObject *args)
{
    int para2 = 0;
    if (!PyArg_ParseTuple(args, "i", &para2))
        return NULL;
    F64 value_ = GetchanelVolt(para2);
    return PyFloat_FromDouble(value_);
}
static PyObject *adlink_PutchanelVolt (PyObject *self, PyObject *args)
{
    int para2 = 0;
    double out=0;
    if (!PyArg_ParseTuple(args, "id", &para2,&out))
        return NULL;
    I16 value_ = PutchanelVolt(para2,out);
    return PyLong_FromLong(value_);
}

static PyObject *adlink_getDI(PyObject *self, PyObject *args)
{
    U32 value = GetDI();
    return PyLong_FromLong(value);
}

static PyObject *adlink_getDO(PyObject *self, PyObject *args)
{
    U32 value = GetDO();
    return PyLong_FromLong(value);
}

static PyObject *adlink_link(PyObject *self, PyObject *args)
{
    //value_+=para2;
    U16 r = Link();
    return PyLong_FromLong(r);
}

static PyMethodDef adlinkMethods[] =
{
    {"system",  adlink_system, METH_VARARGS,  "Execute a shell command."},
    {"link",  adlink_link, METH_VARARGS,  "link."},
    {"getDO",  adlink_getDO, METH_VARARGS,  "GetDO."},
    {"getDI",  adlink_getDI, METH_VARARGS,  "GetDI."},
    {"getchanelVolt",  adlink_GetchanelVolt, METH_VARARGS,  "GetchanelVolt" },
    {"putchanelVolt",  adlink_PutchanelVolt, METH_VARARGS,  "PutchanelVolt" },
    {"plusOne",  adlink_plusOne, METH_VARARGS,  "plus one."},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};
static struct PyModuleDef adlinkmodule =
{
    PyModuleDef_HEAD_INIT,
    "adlink",   /* name of module */
    NULL, /* module documentation, may be NULL */
    -1,       /* size of per-interpreter state of the module,
                 or -1 if the module keeps state in global variables. */
    adlinkMethods
};
PyMODINIT_FUNC PyInit_adlink(void)
{
    PyObject *m;
    value_ = 0;
    obj_card = -1;
    obj_card6208 = -1;
    m = PyModule_Create(&adlinkmodule);
    if (m == NULL)
        return NULL;

    adlinkError = PyErr_NewException("adlink.error", NULL, NULL);
    Py_INCREF(adlinkError);
    PyModule_AddObject(m, "error", adlinkError);
    return m;
}


