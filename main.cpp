#include "stdio.h"
#include "v8.h"
#include "libplatform/libplatform-export.h"
#include "libplatform/libplatform.h"
#include <iostream>
#include <fstream>
#include <string>
#include "jsb.h"
using namespace v8;

void registerFunction(Isolate *isolate, v8::Local<v8::ObjectTemplate> global)
{
    // 注册全局函数
    global->Set(
        v8::String::NewFromUtf8(isolate, "log", v8::NewStringType::kNormal).ToLocalChecked(),
        v8::FunctionTemplate::New(isolate, log));

    // 注册全局变量
    global->SetAccessor(String::NewFromUtf8(isolate, "version").ToLocalChecked(), VersionGetter, VersionSetter);
}
// 注册类
void registerClass(Isolate *isolate, v8::Local<v8::ObjectTemplate> global)
{
    // 新建一个函数模版
    v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(isolate, CPP_constructor);
    // tpl->SetCallHandler(CPP_constructor);
    //  设置类名
    tpl->SetClassName(v8::String::NewFromUtf8(isolate, "CPP").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);
    // 原型链
    v8::Handle<v8::ObjectTemplate> cpp_proto = tpl->PrototypeTemplate();
    cpp_proto->Set(
        v8::String::NewFromUtf8(isolate, "numberAdd", v8::NewStringType::kNormal).ToLocalChecked(),
        v8::FunctionTemplate::New(isolate, CPP_numberAdd));
    cpp_proto->Set(
        v8::String::NewFromUtf8(isolate, "say", v8::NewStringType::kNormal).ToLocalChecked(),
        v8::FunctionTemplate::New(isolate, CPP_say));
    cpp_proto->Set(
        v8::String::NewFromUtf8(isolate, "brother", v8::NewStringType::kNormal).ToLocalChecked(),
        v8::FunctionTemplate::New(isolate, CPP_brother));
    // 绑定到全局对象上
    global->Set(v8::String::NewFromUtf8(isolate, "CPP").ToLocalChecked(), tpl);
}
void runJS(v8::Isolate *isolate)
{
    v8::Isolate::Scope isolate_scope(isolate);
    // Create a stack-allocated handle scope. HandScope用于垃圾回收机制
    v8::HandleScope handle_scope(isolate);
    v8::Local<v8::ObjectTemplate> global = v8::ObjectTemplate::New(isolate);
    registerFunction(isolate, global);
    registerClass(isolate, global);
    // 创建上下文
    v8::Local<v8::Context> context = v8::Context::New(isolate, nullptr, global);
    // 进入上下文环境，编译运行js脚本
    v8::Context::Scope context_scope(context);
    {
        std::string code = "main.js";
        std::ifstream file(code);
        if (!file.is_open())
        {
            printf("not find file %s", code.c_str());
            return;
        }
        // 读取文件内容
        std::string content((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
        file.close(); // 关闭文件

        // 生成js代码
        v8::Local<v8::String> source = v8::String::NewFromUtf8(isolate, content.c_str()).ToLocalChecked();
        // 编译js代码
        v8::Local<v8::Script> script = v8::Script::Compile(context, source).ToLocalChecked();
        // 运行js代码、并获取到返回值
        v8::Local<v8::Value> result = script->Run(context).ToLocalChecked();
        // 将返回值转换为utf8并打印出来
        v8::String::Utf8Value utf8(isolate, result);
        printf("%s\n", *utf8);
    }
}
void main()
{
    // 初始化v8
    // v8::V8::InitializeICUDefaultLocation(nullptr);
    // v8::V8::InitializeExternalStartupData(nullptr);
    std::unique_ptr<v8::Platform> platform = v8::platform::NewDefaultPlatform();
    v8::V8::InitializePlatform(platform.get());
    v8::V8::Initialize();

    // 创建一个新的隔离区，并将这个隔离区置为当前使用
    v8::Isolate::CreateParams create_params;
    create_params.array_buffer_allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();
    v8::Isolate *isolate = v8::Isolate::New(create_params);
    // 函数是必须的，否则释放v8有问题
    runJS(isolate);
    // Dispose the isolate and tear down V8.
    isolate->Dispose();
    v8::V8::Dispose();
    v8::V8::ShutdownPlatform();
    delete create_params.array_buffer_allocator;
    system("pause");
}