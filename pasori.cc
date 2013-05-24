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

#include "pasori.h"
#include "felica.h"

using namespace v8;
using namespace node;

void Pasori::Init(Handle<Object> target){
    Local<FunctionTemplate> t = FunctionTemplate::New(Pasori::New);
    NODE_SET_PROTOTYPE_METHOD(t, "open", Pasori::_open);
    NODE_SET_PROTOTYPE_METHOD(t, "set_timeout", Pasori::_set_timeout);
    NODE_SET_PROTOTYPE_METHOD(t, "init", Pasori::_init);
    NODE_SET_PROTOTYPE_METHOD(t, "reset", Pasori::_reset);
    NODE_SET_PROTOTYPE_METHOD(t, "close", Pasori::_close);
    NODE_SET_PROTOTYPE_METHOD(t, "polling", Pasori::_polling);
    t->InstanceTemplate()->SetInternalFieldCount(1);
    target->Set(String::New("Pasori"), t->GetFunction());
  }

Pasori::Pasori(){
    this->_pasori = NULL;
  }

Pasori::~Pasori(){
  }
  

Handle<Value> Pasori::_open(const Arguments & args){
    HandleScope scope;

    if (0 != args.Length()){
      ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
      return scope.Close(Undefined());
    }

    Pasori* pasoriInstance = ObjectWrap::Unwrap<Pasori>(args.This());

    if(pasoriInstance->_pasori != NULL){
      ThrowException(Exception::TypeError(String::New("Pasori device is already initialized.")));
      return scope.Close(Undefined());
    }

    pasori * __pasori = pasori_open();

    if (__pasori == NULL) {
      ThrowException(Exception::TypeError(String::New("pasori open error")));
      return scope.Close(Undefined());
    }

    pasoriInstance->_pasori = __pasori;

    return scope.Close(Undefined());
  }

Handle<Value> Pasori::_set_timeout(const Arguments & args){
    HandleScope scope;
    int timeout = 0;
    if (1 != args.Length()){
      ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
      return scope.Close(Undefined());
    }
    if (0 < args.Length()){
      timeout = args[0]->NumberValue();
    }

    Pasori* pasoriInstance = ObjectWrap::Unwrap<Pasori>(args.This());
    if(pasoriInstance->_pasori == NULL){
      ThrowException(Exception::TypeError(String::New("Pasori device has not initialized.")));
      return scope.Close(Undefined());
    }
    pasori_set_timeout(pasoriInstance->_pasori, timeout);

    return scope.Close(Undefined());
  }

Handle<Value> Pasori::_init(const Arguments & args){
    HandleScope scope;
    if (0 != args.Length()){
      ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
      return scope.Close(Undefined());
    }
    Pasori* pasoriInstance = ObjectWrap::Unwrap<Pasori>(args.This());
    if(pasoriInstance->_pasori == NULL){
      ThrowException(Exception::TypeError(String::New("Pasori device has not initialized.")));
      return scope.Close(Undefined());
    }
    pasori_init(pasoriInstance->_pasori);
    return scope.Close(Undefined());
  }

Handle<Value> Pasori::_reset(const Arguments & args){
    HandleScope scope;
    if (0 != args.Length()){
      ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
      return scope.Close(Undefined());
    }
    Pasori* pasoriInstance = ObjectWrap::Unwrap<Pasori>(args.This());
    if(pasoriInstance->_pasori == NULL){
      ThrowException(Exception::TypeError(String::New("Pasori device has not initialized.")));
      return scope.Close(Undefined());
    }
    pasori_reset(pasoriInstance->_pasori);
    return scope.Close(Undefined());
  }

Handle<Value> Pasori::_close(const Arguments & args){
    HandleScope scope;
    if (0 != args.Length()){
      ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
      return scope.Close(Undefined());
    }
    Pasori* pasoriInstance = ObjectWrap::Unwrap<Pasori>(args.This());
    if(pasoriInstance->_pasori == NULL){
      ThrowException(Exception::TypeError(String::New("Pasori device has not initialized.")));
      return scope.Close(Undefined());
    }
    pasori_close(pasoriInstance->_pasori);
    return scope.Close(Undefined());
  }

Handle<Value> Pasori::_polling(const Arguments & args){
    HandleScope scope;
    uint8 timeslot;
    uint16 systemcode;

    if (2 < args.Length() || 0 == args.Length()){
      ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
      return scope.Close(Undefined());
    }
    if (1 < args.Length()){
      timeslot = args[1]->NumberValue();
    }else{
      timeslot = 0;
    }
    if (0 < args.Length()) {
      systemcode = args[0]->NumberValue();
    }

    Pasori* pasoriInstance = ObjectWrap::Unwrap<Pasori>(args.This());

    if(pasoriInstance->_pasori == NULL){
      ThrowException(Exception::TypeError(String::New("Pasori device has not initialized.")));
      return scope.Close(Undefined());
    }

    felica * _felica = felica_polling(pasoriInstance->_pasori, systemcode, 0, timeslot);
    
    if (_felica == NULL) {
      return scope.Close(Undefined());
    }

    Handle<Value> felicaInstance = Felica::NewInstance(args);
    Felica* felicaObject = ObjectWrap::Unwrap<Felica>(args.This());
    felicaObject->setFelica(_felica);

    return felicaInstance;
  }


Handle<Value> Pasori::New(const Arguments & args){
    HandleScope scope;
    if (!args.IsConstructCall()){
      return args.Callee()->NewInstance();
    }

    Pasori* pasoriObject = new Pasori();
    (pasoriObject)->Wrap(args.Holder());
    return scope.Close(args.Holder());
  }



