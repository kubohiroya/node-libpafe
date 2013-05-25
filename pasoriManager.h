#ifndef PASORIMANAGER_OBJECT_H
#define PASORIMANAGER_OBJECT_H

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

class PasoriManager : ObjectWrap {
public:
  static void Init(Handle<Object> target);
  PasoriManager();
  ~PasoriManager();

  static Handle<Value> _close(const Arguments & args);
  static Handle<Value> _open(const Arguments & args);
  static Handle<Value> _open_multi(const Arguments & args);

 private:
  static Handle<Value> New(const Arguments& args);
  pasori_devices * _pasori_devices;
  pasori * _pasori;
};


#endif
