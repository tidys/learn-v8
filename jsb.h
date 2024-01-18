#ifndef __JSB__
#define __JSB__
#include "stdio.h"
#include "v8.h"
#include "libplatform/libplatform-export.h"
#include "libplatform/libplatform.h"
#include <iostream>
#include <fstream>
#include <string>
#include "cpp.h"
using namespace v8;
char version[100] = "1.0.0";

void CPP_constructor(const FunctionCallbackInfo<Value> &args)
{
    if (args.Length())
    {
        v8::Isolate *isolate = args.GetIsolate();
        int value = args[0].As<v8::Int32>()->Value();
        // args[0].ToNumber(isolate)->Value();
        CPP *cpp = new CPP(value);
        args.This()->SetAlignedPointerInInternalField(0, cpp);
    }
}

void CPP_numberAdd(const FunctionCallbackInfo<Value> &args)
{
    v8::Isolate *isolate = args.GetIsolate();
    CPP *obj = static_cast<CPP *>(args.Holder()->GetAlignedPointerFromInternalField(0));
    obj->numberAdd();
    args.GetReturnValue().Set(v8::Number::New(isolate, -1));
}
void CPP_say(const FunctionCallbackInfo<Value> &args)
{
    v8::Isolate *isolate = args.GetIsolate();
    CPP *obj = static_cast<CPP *>(args.Holder()->GetAlignedPointerFromInternalField(0));
    v8::String::Utf8Value str(isolate, args[0]);
    std::string ss(*str);
    obj->say(ss);
}
void CPP_brother(const FunctionCallbackInfo<Value> &args)
{
    v8::Isolate *isolate = args.GetIsolate();
    CPP *obj = static_cast<CPP *>(args.Holder()->GetAlignedPointerFromInternalField(0));

    v8::Local<v8::Object> obj1 = args[0]->ToObject(isolate->GetCurrentContext()).ToLocalChecked();
    CPP *arg1 = static_cast<CPP *>(obj1->GetAlignedPointerFromInternalField(0));
    obj->brother(arg1);
}

void VersionGetter(Local<String> property, const PropertyCallbackInfo<Value> &info)
{
    info.GetReturnValue().Set(String::NewFromUtf8(info.GetIsolate(), version).ToLocalChecked());
}

void VersionSetter(Local<String> property, Local<Value> value, const PropertyCallbackInfo<void> &info)
{
    String::Utf8Value str(info.GetIsolate(), value);
    const char *result = *str;
    strncpy(version, result, sizeof(version));
}

void log(const FunctionCallbackInfo<Value> &args)
{
    for (int i = 0; i < args.Length(); i++)
    {
        HandleScope handle_scope(args.GetIsolate());
        String::Utf8Value str(args.GetIsolate(), args[i]);
        std::string vvv(*str);
        printf("%s\n", vvv.c_str());
    }
}
#endif
