#ifndef PASORI_OBJECT_H
#define PASORI_OBJECT_H

extern "C"{
#include <libpafe/libpafe.h>
}

#include <node.h>
#include <node_buffer.h>
#include <v8.h>
#include <stdlib.h>
#include <string>
#include <stdio.h>

#include "felica.h"

using namespace v8;
using namespace node;

class Pasori : ObjectWrap {
public:
  static void Init();

  Pasori();
  ~Pasori();

  static Persistent<Function> constructor;
  static Handle<Value> NewInstance(const Arguments& args);

  static Handle<Value> _open(const Arguments & args);

  static Handle<Value> _set_timeout(const Arguments & args);

  static Handle<Value> _init(const Arguments & args);

  static Handle<Value> _reset(const Arguments & args);

  static Handle<Value> _close(const Arguments & args);

  static Handle<Value> _polling(const Arguments & args);

  pasori * _pasori;

private:
  static Handle<Value> New(const Arguments& args);

};


#endif
