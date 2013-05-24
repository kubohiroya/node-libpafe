#define BUILDING_NODE_EXTENTION

#define MACOSX

extern "C"{
#include <libpafe/libpafe.h>
}

#include <node.h>
#include <node_buffer.h>
#include <v8.h>
#include <stdlib.h>
#include <string>
#include <stdio.h>

#include "pasoriFactory.h"
#include "pasori.h"
#include "felica.h"


using namespace v8;
using namespace node;

/*
void wrap_pointer_cb(char *data, void *hint) {
  //fprintf(stderr, "wrap_pointer_cb\n");
}

Handle<Value> WrapPointer(char *ptr, size_t length) {
  void *user_data = NULL;
  Buffer *buf = Buffer::New(ptr, length, wrap_pointer_cb, user_data);
  return buf->handle_;
}

Handle<Value> WrapPointer(void *ptr) {
  size_t size = 0;
  return WrapPointer(static_cast<char*>(ptr), size);
}
*/

void PasoriFactory::Init(Handle<Object> target){
    Local<FunctionTemplate> t = FunctionTemplate::New(PasoriFactory::New);
    //NODE_SET_PROTOTYPE_METHOD(t, "open_single", PasoriFactory::_open_single);
    NODE_SET_PROTOTYPE_METHOD(t, "open_multi", PasoriFactory::_open_multi);
    t->InstanceTemplate()->SetInternalFieldCount(1);
    target->Set(String::New("PasoriFactory"), t->GetFunction());
  }

PasoriFactory::PasoriFactory(){
  }

PasoriFactory::~PasoriFactory(){
  }

Handle<Value> PasoriFactory::_open_multi(const Arguments & args){
    HandleScope scope;

    if (0 != args.Length()){
      ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
      return scope.Close(Undefined());
    }

    PasoriFactory* pasoriFactory = ObjectWrap::Unwrap<PasoriFactory>(args.This());

    if(pasoriFactory->_pasori_devices != NULL){
      ThrowException(Exception::TypeError(String::New("Pasori device is already initialized.")));
      return scope.Close(Undefined());
    }

    pasori_devices * _pasori_devices = pasori_open_multi();

    if (_pasori_devices == NULL) {
      ThrowException(Exception::TypeError(String::New("pasori open error")));
      return scope.Close(Undefined());
    }

    pasoriFactory->_pasori_devices = _pasori_devices;

    return scope.Close(Undefined());
  }


 Handle<Value> PasoriFactory::New(const Arguments& args) {
    HandleScope scope;

    PasoriFactory* obj = new PasoriFactory();
    obj->Wrap(args.This());

    return args.This();
  }

void init(Handle<Object> exports) {
  Felica::Init(exports);
  Pasori::Init(exports);
  PasoriFactory::Init(exports);
}


NODE_MODULE(pafe, init)
