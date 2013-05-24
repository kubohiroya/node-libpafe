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

void Felica::Init(Handle<Object> target){
    Local<FunctionTemplate> t = FunctionTemplate::New(Felica::New);
    NODE_SET_PROTOTYPE_METHOD(t, "close", Felica::_close);
    NODE_SET_PROTOTYPE_METHOD(t, "read_single", Felica::_read_single);
    NODE_SET_PROTOTYPE_METHOD(t, "get_idm", Felica::_get_idm);
    NODE_SET_PROTOTYPE_METHOD(t, "get_pmm", Felica::_get_pmm);
    t->InstanceTemplate()->SetInternalFieldCount(1);
    target->Set(String::New("Felica"), t->GetFunction());
  }

Felica::Felica(){
    this->_felica = NULL;
  }

Felica::~Felica(){
  }
  
void Felica::setFelica(felica *__felica){
    if(__felica != NULL){
      this->_felica = __felica;
    }
  }

Handle<Value> Felica::NewInstance(const Arguments & args){
    HandleScope scope;

    Felica* felicaObject = new Felica();
    (felicaObject)->Wrap(args.Holder());
    return scope.Close(args.Holder());
  }

Handle<Value> Felica::_close(const Arguments & args){
    HandleScope scope;
    if (0 != args.Length()){
      ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
      return scope.Close(Undefined());
    }
    Felica* felica = ObjectWrap::Unwrap<Felica>(args.This());

    if(felica->_felica == NULL){
      ThrowException(Exception::TypeError(String::New("felica has not initialized.")));
      return scope.Close(Undefined());
    }

    free(felica->_felica);

    return scope.Close(Undefined());
  }


Handle<Value> Felica::_read_single(const Arguments& args) {
    HandleScope scope;

    int servicecode;
    int mode;
    uint8 addr;
    int datalen = 16;
    uint8 data[datalen];

    if (args.Length() < 1 || 3 < args.Length()){
      ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
      return scope.Close(Undefined());
    }
    if (2 < args.Length()){
      addr = args[2]->NumberValue();
    }else{
      addr = 0;
    }
    if (1 < args.Length()){
      mode = args[1]->NumberValue();
    }else{
      mode = 0;
    }
    if (0 < args.Length()){
      servicecode = args[0]->NumberValue();
    }else{
      servicecode = 0;
    }

    Felica* felica = ObjectWrap::Unwrap<Felica>(args.This());

    if(felica->_felica == NULL){
      ThrowException(Exception::TypeError(String::New("felica has not initialized.")));
      return scope.Close(Undefined());
    }

    int ret = felica_read_single(felica->_felica, servicecode, mode, addr, data);
    if(ret == 0){
      std::string str(data, data+datalen);
      return scope.Close(String::New(str.c_str (), str.length()));
    }else{
      return scope.Close(Undefined());
    }
  }

Handle<Value> Felica::_get_idm(const Arguments& args) {
    HandleScope scope;

    uint8 idm[8];

    if (args.Length() != 0){
      ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
      return scope.Close(Undefined());
    }

    Felica* felica = ObjectWrap::Unwrap<Felica>(args.This());

    if(felica->_felica == NULL){
      ThrowException(Exception::TypeError(String::New("felica has not initialized.")));
      return scope.Close(Undefined());
    }

    int ret = felica_get_idm(felica->_felica, idm);

    if(ret == 0){
      Local<Array> result = Array::New(8);
      for(int i = 0; i < 8; i++){
        result->Set(Number::New(i), Number::New(idm[i]));
      }
      return scope.Close(result);
    }else{
      return scope.Close(Undefined());
    }
  }

Handle<Value> Felica::_get_pmm(const Arguments& args) {
    HandleScope scope;

    uint8 pmm[8];

    if (args.Length() != 0){
      ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
      return scope.Close(Undefined());
    }

    Felica* felica = ObjectWrap::Unwrap<Felica>(args.This());

    if(felica->_felica == NULL){
      ThrowException(Exception::TypeError(String::New("felica has not initialized.")));
      return scope.Close(Undefined());
    }

    int ret = felica_get_pmm(felica->_felica, pmm);
    if(ret == 0){
      Local<Array> result = Array::New(8);
      for(int i = 0; i < 8; i++){
        result->Set(Number::New(i), Number::New(pmm[i]));
      }
      return scope.Close(result);
    }else{
      return scope.Close(Undefined());
    }
  }

Handle<Value> Felica::New(const Arguments& args) {
    HandleScope scope;

    Felica* obj = new Felica();
    obj->Wrap(args.This());

    return args.This();
  }

