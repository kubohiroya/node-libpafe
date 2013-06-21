#define BUILDING_NODE_EXTENTION

#include "felica.h"
#define DATA_LEN 16

using namespace v8;
using namespace node;

Persistent<Function> Felica::constructor;

Felica::Felica(){
  this->_felica = NULL;
}

Felica::~Felica(){
}

void Felica::Init(){
  Local<FunctionTemplate> tpl = FunctionTemplate::New(Felica::New);
  tpl->SetClassName(String::NewSymbol("Felica"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  NODE_SET_PROTOTYPE_METHOD(tpl, "close", Felica::_close);
  NODE_SET_PROTOTYPE_METHOD(tpl, "read_single", Felica::_read_single);
  NODE_SET_PROTOTYPE_METHOD(tpl, "get_idm", Felica::_get_idm);
  NODE_SET_PROTOTYPE_METHOD(tpl, "get_pmm", Felica::_get_pmm);
  constructor = Persistent<Function>::New(tpl->GetFunction());
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

#if defined HAVE_LIBPAFE
  free(felica->_felica);
#elif defined HAVE_FELICALIB
  felica_free(felica->_felica);
#endif

  return scope.Close(Undefined());
}

Handle<Value> Felica::_read_single(const Arguments& args) {
  HandleScope scope;

  int servicecode;
  int mode;
  uint8 addr;
  int datalen = DATA_LEN;
  uint8 data[DATA_LEN];

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

#if defined HAVE_LIBPAFE
  int ret = felica_read_single(felica->_felica, servicecode, mode, addr, data);
#elif defined HAVE_FELICALIB
  int ret = felica_read_without_encryption02(felica->_felica, servicecode, mode, addr, data);
#endif

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

#if defined HAVE_LIBPAFE
  int ret = felica_get_idm(felica->_felica, idm);
#elif defined HAVE_FELICALIB
  int ret = 0;
  felica_getidm(felica->_felica, idm);
#endif

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

#if defined HAVE_LIBPAFE
  int ret = felica_get_pmm(felica->_felica, pmm);
#elif defined HAVE_FELICALIB
  int ret = 0;
  felica_getpmm(felica->_felica, pmm);
#endif

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
  Felica* felicaObject = new Felica();
  felicaObject->Wrap(args.This());
  return args.This();
}

