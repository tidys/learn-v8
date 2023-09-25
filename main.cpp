#include "stdio.h"
#include "v8.h"
#include "libplatform/libplatform-export.h"
#include "libplatform/libplatform.h"
using namespace v8;
char version[100] = "1.0.0";
void VersionGetter(Local<String> property,    const PropertyCallbackInfo<Value>& info) {
    info.GetReturnValue().Set(String::NewFromUtf8(info.GetIsolate(), version).ToLocalChecked());
}

void VersionSetter(Local<String> property, Local<Value> value, const PropertyCallbackInfo<void>& info) {
    String::Utf8Value str(info.GetIsolate(), value);
    const char* result = *str;
    strncpy(version, result, sizeof(version));
}

void log(const FunctionCallbackInfo <Value>& args)
{
    for (int i = 0; i < args.Length(); i++)
    {
        HandleScope handle_scope(args.GetIsolate());
        String::Utf8Value str(args.GetIsolate(), args[i]);
        std::string vvv(*str);
        printf("%s\n", vvv.c_str());
    }
}
const char* code = "log(version); version='2.0'; log(version); log('aaa','b','c');'hello world';";
void registerFunction(Isolate* isolate, v8::Local<v8::ObjectTemplate> global){
    global->Set(
        v8::String::NewFromUtf8(isolate, "log").ToLocalChecked(),
        v8::FunctionTemplate::New(isolate, log)
    );
    global->SetAccessor(String::NewFromUtf8(isolate, "version").ToLocalChecked(), VersionGetter, VersionSetter);
}

void registerClass(Isolate* isolate){
    v8::Local<v8::ObjectTemplate> point = v8::ObjectTemplate::New(isolate);
    point->SetInternalFieldCount(1);

}
void runJS(v8::Isolate* isolate)
{
    v8::Isolate::Scope isolate_scope(isolate);
    // Create a stack-allocated handle scope. HandScope�����������ջ���
    v8::HandleScope handle_scope(isolate);
    v8::Local<v8::ObjectTemplate> global = v8::ObjectTemplate::New(isolate);
    registerFunction(isolate, global);
    registerClass(isolate);
    // ����������
    v8::Local<v8::Context> context = v8::Context::New(isolate, nullptr, global);
    // ���������Ļ�������������js�ű�
    v8::Context::Scope context_scope(context);
    {
        // ����js����
        v8::Local<v8::String> source = v8::String::NewFromUtf8(isolate, code).ToLocalChecked();
        // ����js����
        v8::Local<v8::Script> script = v8::Script::Compile(context, source).ToLocalChecked();
        // ����js���롢����ȡ������ֵ
        v8::Local<v8::Value> result = script->Run(context).ToLocalChecked();
        // ������ֵת��Ϊutf8����ӡ����
        v8::String::Utf8Value utf8(isolate, result);
        printf("%s\n", *utf8);
    }
}
void main()
{
    // ��ʼ��v8
    //v8::V8::InitializeICUDefaultLocation(nullptr);
    //v8::V8::InitializeExternalStartupData(nullptr);
    std::unique_ptr<v8::Platform> platform = v8::platform::NewDefaultPlatform();
    v8::V8::InitializePlatform(platform.get());
    v8::V8::Initialize();

    // ����һ���µĸ����������������������Ϊ��ǰʹ��
    v8::Isolate::CreateParams create_params;
    create_params.array_buffer_allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();
    v8::Isolate* isolate = v8::Isolate::New(create_params);
    // �����Ǳ���ģ������ͷ�v8������
    runJS(isolate);
    // Dispose the isolate and tear down V8.
    isolate->Dispose();
    v8::V8::Dispose();
    v8::V8::ShutdownPlatform();
    delete create_params.array_buffer_allocator;
    system("pause");
}