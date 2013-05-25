#ifndef FELICA_OBJECT_H
#define FELICA_OBJECT_H

extern "C"{
#include <libpafe/libpafe.h>
}

#include <node.h>
#include <node_buffer.h>
#include <v8.h>
#include <stdlib.h>
#include <string>
#include <stdio.h>

#include "pasori.h"

using namespace v8;
using namespace node;

class Felica : ObjectWrap {
public:
  static void Init(Handle<Object> target);
  Felica();
  ~Felica();
  
  void setFelica(felica *felica);
  static Handle<Value> NewInstance(const Arguments & args);
  static Handle<Value> _close(const Arguments & args);
  static Handle<Value> _read_single(const Arguments& args);
  static Handle<Value> _get_idm(const Arguments& args);
  static Handle<Value> _get_pmm(const Arguments& args);
  static Handle<Value> New(const Arguments& args);

private:

  felica * _felica;
};

#endif
