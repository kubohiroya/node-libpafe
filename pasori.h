#ifndef PASORI_OBJECT_H
#define PASORI_OBJECT_H

#if defined(HAVE_LIBPAFE)
extern "C"{
#include <libpafe.h>
}
#elif defined(HAVE_FELICALIB)
extern "C"{
#include <felicalib.h>
#include "pasori_open_multi.h"
}
#else
#error "unknown platform"
#endif

#include <node.h>
#include <node_buffer.h>
#include <v8.h>
#include <stdlib.h>
#include <string>
#include <stdio.h>

#include "felica.h"

using namespace v8;
using namespace node;

class Pasori : public node::ObjectWrap {
 public:
  static void Init(v8::Isolate *isolate);
  static void New(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void NewInstance(const v8::FunctionCallbackInfo<v8::Value>& args);

  static void _open(const v8::FunctionCallbackInfo<v8::Value>& args);

  static void _set_timeout(const v8::FunctionCallbackInfo<v8::Value>& args);

  static void _init(const v8::FunctionCallbackInfo<v8::Value>& args);

  static void _reset(const v8::FunctionCallbackInfo<v8::Value>& args);

  static void _close(const v8::FunctionCallbackInfo<v8::Value>& args);

  static void _polling(const v8::FunctionCallbackInfo<v8::Value>& args);

  static void _get_error_code(const v8::FunctionCallbackInfo<v8::Value>& args);


 private:
  explicit Pasori();
  ~Pasori();
  pasori * _pasori;

  static Persistent<Function> constructor;

};


#endif
