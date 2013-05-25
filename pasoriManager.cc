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

#include "pasoriManager.h"
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

void PasoriManager::Init(Handle<Object> target){
    Local<FunctionTemplate> t = FunctionTemplate::New(PasoriManager::New);
    NODE_SET_PROTOTYPE_METHOD(t, "open", PasoriManager::_open);
    NODE_SET_PROTOTYPE_METHOD(t, "open_multi", PasoriManager::_open_multi);
    t->InstanceTemplate()->SetInternalFieldCount(1);
    target->Set(String::New("PasoriManager"), t->GetFunction());
  }

PasoriManager::PasoriManager(){
  }

PasoriManager::~PasoriManager(){
}

Handle<Value> PasoriManager::_close(const Arguments & args){
    HandleScope scope;

    if (0 != args.Length()){
      ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
      return scope.Close(Undefined());
    }

    PasoriManager* pasoriManager = ObjectWrap::Unwrap<PasoriManager>(args.This());

    if(pasoriManager->_pasori != NULL){
      free(pasoriManager->_pasori);
    }
    if(pasoriManager->_pasori_devices != NULL){
      free(pasoriManager->_pasori_devices);
    }

    return scope.Close(Undefined());
  }

Handle<Value> PasoriManager::_open(const Arguments & args){
    HandleScope scope;

    if (0 != args.Length()){
      ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
      return scope.Close(Undefined());
    }

    PasoriManager* pasoriManager = ObjectWrap::Unwrap<PasoriManager>(args.This());

    if(pasoriManager->_pasori != NULL){
      ThrowException(Exception::TypeError(String::New("Pasori device is already initialized.")));
      return scope.Close(Undefined());
    }

    pasori * _pasori = pasori_open();

    if (_pasori == NULL) {
      ThrowException(Exception::TypeError(String::New("pasori open error")));
      return scope.Close(Undefined());
    }

    pasoriManager->_pasori = _pasori;

    return scope.Close(Undefined());
  }

Handle<Value> PasoriManager::_open_multi(const Arguments & args){
    HandleScope scope;

    if (0 != args.Length()){
      ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
      return scope.Close(Undefined());
    }

    PasoriManager* pasoriManager = ObjectWrap::Unwrap<PasoriManager>(args.This());

    if(pasoriManager->_pasori_devices != NULL){
      ThrowException(Exception::TypeError(String::New("Pasori device is already initialized.")));
      return scope.Close(Undefined());
    }

    pasori_devices * _pasori_devices = pasori_open_multi();

    if (_pasori_devices == NULL) {
      ThrowException(Exception::TypeError(String::New("pasori open error")));
      return scope.Close(Undefined());
    }

    pasoriManager->_pasori_devices = _pasori_devices;

    return scope.Close(Undefined());
  }


 Handle<Value> PasoriManager::New(const Arguments& args) {
    HandleScope scope;

    PasoriManager* obj = new PasoriManager();
    obj->Wrap(args.This());

    return args.This();
  }

void init(Handle<Object> exports) {
  Felica::Init(exports);
  Pasori::Init(exports);
  PasoriManager::Init(exports);
}


NODE_MODULE(pafe, init)
