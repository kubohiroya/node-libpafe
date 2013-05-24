#ifndef PASORIFACTORY_OBJECT_H
#define PASORIFACTORY_OBJECT_H

extern "C"{
#include <libpafe/libpafe.h>
}

#include <node.h>
#include <node_buffer.h>
#include <v8.h>
#include <stdlib.h>
#include <string>
#include <stdio.h>

using namespace v8;
using namespace node;

class PasoriFactory : ObjectWrap {
public:
  static void Init(Handle<Object> target);
  PasoriFactory();
  ~PasoriFactory();

  static Handle<Value> _open_multi(const Arguments & args);

 private:
  static Handle<Value> New(const Arguments& args);
  pasori_devices * _pasori_devices;
};

#endif
