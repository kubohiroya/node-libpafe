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
#include <node_buffer.h>
#include <v8.h>
#include <stdlib.h>
#include <string>
#include <stdio.h>

using namespace v8;
using namespace node;

class Felica : ObjectWrap {
public:
  static void Init();
  Felica();
  ~Felica();
  static Persistent<Function> constructor;
  static Handle<Value> NewInstance(const Arguments & args);

  static Handle<Value> _close(const Arguments & args);
  static Handle<Value> _read_single(const Arguments& args);
  static Handle<Value> _get_idm(const Arguments& args);
  static Handle<Value> _get_pmm(const Arguments& args);

  felica * _felica;

private:
  static Handle<Value> New(const Arguments& args);
};

#endif
