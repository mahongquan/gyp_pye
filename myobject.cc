// myobject.cc
#include "myobject.h"


namespace demo
{
    bool BitIsSet(U32 Number, U16 Bit)
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
    Persistent<Function> MyObject::constructor;

    MyObject::MyObject(double value) : value_(value)
    {
        card = -1;
        DO_Value = 0;
        linked = false;
    }

    MyObject::~MyObject()
    {
    }

    // void MyObject::Init(Local<Object> exports)
    // {
    //     Isolate *isolate = exports->GetIsolate();

    //     // Prepare constructor template
    //     Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
    //     tpl->SetClassName(String::NewFromUtf8(isolate, "MyObject"));
    //     tpl->InstanceTemplate()->SetInternalFieldCount(1);

    //     // Prototype
    //     NODE_SET_PROTOTYPE_METHOD(tpl, "plusOne", PlusOne);
    //     NODE_SET_PROTOTYPE_METHOD(tpl, "link", Link);//getchanelVolt
    //     NODE_SET_PROTOTYPE_METHOD(tpl, "getchanelVolt", GetchanelVolt);
    //     NODE_SET_PROTOTYPE_METHOD(tpl, "putchanelVolt", PutchanelVolt);//PutchanelVolt
    //     NODE_SET_PROTOTYPE_METHOD(tpl, "getDO", GetDO);
    //     NODE_SET_PROTOTYPE_METHOD(tpl, "getDI", GetDI);
    //     NODE_SET_PROTOTYPE_METHOD(tpl, "getDOBit", GetDOBit);
    //     NODE_SET_PROTOTYPE_METHOD(tpl, "getDIBit", GetDIBit);
    //     //NODE_SET_PROTOTYPE_METHOD(tpl, "putD", PutD);//PutchanelVolt
    //     NODE_SET_PROTOTYPE_METHOD(tpl, "openVLV", OpenVLV);
    //     NODE_SET_PROTOTYPE_METHOD(tpl, "closeVLV", CloseVLV);//PutchanelVolt
    //     constructor.Reset(isolate, tpl->GetFunction());
    //     exports->Set(String::NewFromUtf8(isolate, "MyObject"),
    //                  tpl->GetFunction());
    // }

    // void MyObject::New(const FunctionCallbackInfo<Value> &args)
    // {
    //     Isolate *isolate = args.GetIsolate();

    //     if (args.IsConstructCall())
    //     {
    //         // Invoked as constructor: `new MyObject(...)`
    //         double value = args[0]->IsUndefined() ? 0 : args[0]->NumberValue();
    //         MyObject *obj = new MyObject(value);
    //         obj->Wrap(args.This());
    //         args.GetReturnValue().Set(args.This());
    //     }
    //     else
    //     {
    //         // Invoked as plain function `MyObject(...)`, turn into construct call.
    //         const int argc = 1;
    //         Local<Value> argv[argc] = { args[0] };
    //         Local<Context> context = isolate->GetCurrentContext();
    //         Local<Function> cons = Local<Function>::New(isolate, constructor);
    //         Local<Object> result =
    //             cons->NewInstance(context, argc, argv).ToLocalChecked();
    //         args.GetReturnValue().Set(result);
    //     }
    // }

    void MyObject::PlusOne()
    {
        this->value_ += 1;

        return this->value_;
    }
    void MyObject::Link()
    {
        MyObject *obj = this;
        I16 card;
        I16  err, card_num = 0;
        //getch();
        printf("card=%d\n", obj->card);
        card = Register_Card (PCI_9111HR, card_num);
        if (card < 0 )
        {
            printf("Register_Card error=%d", card);
            obj->linked = false;
            return;
        }
        else
        {
            printf("card=%d", card);
            obj->card = card;
        }
        err = AI_9111_Config(card, TRIG_INT_PACER, 0, 1024);
        if (err != 0)
        {
            printf("AI_9111_Config error=%d", err);
            obj->linked = false;
            return;
        }
        printf("card2=%d\n", obj->card6208);
        card = Register_Card (PCI_6208V, card_num);
        if (card < 0 )
        {
            printf("Register_Card error=%d", card);
            obj->linked = false;
            return;
        }
        else
        {
            printf("card=%d", card);
            obj->card6208 = card;
        }
        obj->linked = true;
        return(obj->card);
    }
    void MyObject::GetchanelVolt(U16 Channel)
    {
        
        MyObject *obj =this;
        U16 Channel = args[0]->Uint32Value() ;
        printf("card=%d", obj->card);
        printf("Channel=%d\n", Channel);
        F64 voltage;
        U16 AdRange = AD_B_10_V;
        I16 err = AI_VReadChannel(obj->card, Channel, AdRange , &voltage);
        if (err != 0)
        {
            printf("read error=%d\n", err);
            return;
        }
        printf("volt=%f\n", voltage);
        return (voltage);
    }

    void MyObject::PutchanelVolt(U16 Channel,F64 voltage)
    {
        MyObject *obj = this;
        I16 err = AO_VWriteChannel(obj->card6208, Channel, voltage);
        if (err != 0)
        {
            printf("write error=%d\n", err);
            return;
        }
    }//func
    void MyObject::GetDI()
    {
        printf("GetchanelVolt\n");
        MyObject *obj = this;
        // Check the number of arguments passed.
        U32 Value;
        U16 Port = P9111_CHANNEL_DI;
        I16 err = DI_ReadPort(obj->card, Port, &Value);//U16 CardNumber, U16 Port, U32 *Value
        if (err != 0)
        {
            printf("read error=%d\n", err);
            return;
        }
        printf("after read value=%u\n", Value);
        obj->DI_Value = Value;
        return(obj->DI_Value);
    }//func
    void MyObject::GetDO()
    {
        MyObject *obj = this;
        // Check the number of arguments passed.
        printf("card=%d", obj->card);
        U32 Value;
        printf("DO_Value=%u\n", obj->DO_Value);
        U16 Port = P9111_CHANNEL_DO;
        I16 err = DO_ReadPort(obj->card, Port, &Value);//U16 CardNumber, U16 Port, U32 *Value
        if (err != 0)
        {
            printf("read error=%d\n", err);
            return;
        }
        printf("after read value=%u\n", Value);
        obj->DO_Value = Value;
        return(obj->DO_Value);
    }//func
    void MyObject::outputdo(MyObject *obj)
    {
        U16 Port = P9111_CHANNEL_DO;
        printf("card=%d", obj->card);
        I16 err = DO_WritePort(obj->card, Port, obj->DO_Value); //, U16 Port, U32 Value
        if (err != 0)
        {
            printf("write port error=%d\n", err);
            return;
        }
    }
    // void MyObject::PutD(const FunctionCallbackInfo<Value> &args)
    // {
    //     MyObject *obj = ObjectWrap::Unwrap<MyObject>(args.Holder());
    //     outputdo(obj);
    // }//func
    void MyObject::OpenVLV(U16 Channel)
    {
        printf("OpenVLV\n");
        MyObject *obj = this;
        setChannelOn(&(obj->DO_Value), Channel);
        printf("DO_Value=%u\n", obj->DO_Value);
        outputdo(obj);
    }//func
    void MyObject::CloseVLV(U16 Channel)
    {
        printf("closeVLV\n");
        MyObject *obj = this;
        setChannelOff(&(obj->DO_Value), Channel);
        printf("DO_Value=%u\n", obj->DO_Value);
        outputdo(obj);
    }//func
    void MyObject::GetDIBit(U16 Channel)
    {
        printf("GetDIBit\n");

        MyObject *obj = this;
        bool r=BitIsSet(obj->DI_Value,Channel);
        return r;
    }
    void MyObject::GetDOBit(U16 Channel)
    {
        printf("GetchanelVolt\n");
        MyObject *obj = this;
        bool r=BitIsSet(obj->DO_Value,Channel);
        return(r);
    }
}  // namespace demo
//I16 __stdcall AI_VReadChannel(U16 CardNumber, U16 Channel, U16 AdRange, F64 *voltage);
//dcall AO_VWriteChannel(U16 CardNumber, U16 Channel, F64 Voltage);