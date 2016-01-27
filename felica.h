#ifndef FELICA_OBJECT_H
#define FELICA_OBJECT_H

#if defined(HAVE_LIBPAFE)
extern "C"{
#include <libpafe.h>
}
#elif defined(HAVE_FELICALIB)
extern "C"{
#include <felicalib.h>
}
#else
#error "unknown platform"
#endif


#include <node.h>
#include <node_object_wrap.h>
#include <node_buffer.h>
#include <v8.h>
#include <stdlib.h>
#include <string>
#include <stdio.h>

class Felica : public node::ObjectWrap {
  public:
    static void Init(v8::Local<v8::Object> exports);
    static void NewInstance(const v8::FunctionCallbackInfo<v8::Value>& args, felica* felica);
    static void _close(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void _read_single(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void _get_idm(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void _get_pmm(const v8::FunctionCallbackInfo<v8::Value>& args);
    Felica(felica* felica);

  private:
    ~Felica();

    static void New(const v8::FunctionCallbackInfo<v8::Value>& args);

    static v8::Persistent<v8::Function> constructor;
    felica * _felica;

};

#endif
