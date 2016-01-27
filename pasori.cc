#define BUILDING_NODE_EXTENTION

#include <node.h>
#include "pasori.h"

using namespace v8;
using namespace node;

Persistent<Function> Pasori::constructor;

Pasori::Pasori(){
  this->_pasori = NULL;
}

Pasori::~Pasori(){
}
  

void Pasori::Init(Isolate *isolate){
  Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, Pasori::New);
  tpl->SetClassName(String::NewFromUtf8(isolate, "Pasori"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  constructor.Reset(isolate, tpl->GetFunction());

  NODE_SET_PROTOTYPE_METHOD(tpl, "open", Pasori::_open);
  NODE_SET_PROTOTYPE_METHOD(tpl, "set_timeout", Pasori::_set_timeout);
  NODE_SET_PROTOTYPE_METHOD(tpl, "init", Pasori::_init);
  NODE_SET_PROTOTYPE_METHOD(tpl, "reset", Pasori::_reset);
  NODE_SET_PROTOTYPE_METHOD(tpl, "close", Pasori::_close);
  NODE_SET_PROTOTYPE_METHOD(tpl, "polling", Pasori::_polling);
  NODE_SET_PROTOTYPE_METHOD(tpl, "get_error_code", Pasori::_get_error_code);

  //exports->Set(String::NewFromUtf8(isolate, "Pasori"), tpl->GetFunction());
}

void Pasori::New(const v8::FunctionCallbackInfo<v8::Value> &args){
  Pasori* pasoriObject = new Pasori();
  if (args.IsConstructCall()) {
    (pasoriObject)->Wrap(args.This());
    args.GetReturnValue().Set(args.This());
  }else{
    Isolate * isolate = args.GetIsolate();
    const int argc = 0;
    Local<Value> argv[argc] = {};
    Local<Function> cons = Local<Function>::New(isolate, constructor);
    args.GetReturnValue().Set(cons->NewInstance(argc, argv));
  }
}

void Pasori::_open(const v8::FunctionCallbackInfo<v8::Value> &args){
  Isolate * isolate = args.GetIsolate();
  if (0 != args.Length()){
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Wrong number of arguments")));
    return;
  }

  Pasori* pasoriInstance = ObjectWrap::Unwrap<Pasori>(args.This());

  if(pasoriInstance->_pasori != NULL){
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Pasori device is already initialized.")));
    return;
  }

#if defined HAVE_LIBPAFE
  pasori * __pasori = pasori_open();
#elif defined HAVE_FELICALIB
  pasori * __pasori = pasori_open(NULL);
#endif

  if (__pasori == NULL) {
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "pasori open error")));
    return;
  }

  pasoriInstance->_pasori = __pasori;

  return;
}

void Pasori::_set_timeout(const v8::FunctionCallbackInfo<v8::Value> &args){
  Isolate * isolate = args.GetIsolate();
  int timeout = 0;
  if (1 != args.Length()){
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Wrong number of arguments")));
    return;
  }
  if (0 < args.Length()){
    timeout = args[0]->NumberValue();
  }

  Pasori* pasoriInstance = ObjectWrap::Unwrap<Pasori>(args.This());
  if(pasoriInstance->_pasori == NULL){
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Pasori device has not initialized: timeout")));
    return;
  }

#if defined HAVE_LIBPAFE
  pasori_set_timeout(pasoriInstance->_pasori, timeout);
#elif defined HAVE_FELICALIB
  // do nothing
#endif

  return;
}

void Pasori::_init(const v8::FunctionCallbackInfo<v8::Value> &args){
  Isolate * isolate = args.GetIsolate();
  if (0 != args.Length()){
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Wrong number of arguments")));
    return;
  }
  Pasori* pasoriInstance = ObjectWrap::Unwrap<Pasori>(args.This());
  if(pasoriInstance->_pasori == NULL){
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Pasori device has not initialized.")));
    return;
  }

#if defined HAVE_LIBPAFE
  pasori_init(pasoriInstance->_pasori);
#elif defined HAVE_FELICALIB
  pasori_init(pasoriInstance->_pasori);
#endif

  return;
}

void Pasori::_reset(const v8::FunctionCallbackInfo<v8::Value> &args){
  Isolate * isolate = args.GetIsolate();
  if (0 != args.Length()){
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Wrong number of arguments")));
    return;
  }
  Pasori* pasoriInstance = ObjectWrap::Unwrap<Pasori>(args.This());
  if(pasoriInstance->_pasori == NULL){
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Pasori device has not initialized.")));
    return;
  }

#if defined HAVE_LIBPAFE
  pasori_reset(pasoriInstance->_pasori);
#elif defined HAVE_FELICALIB
  // do nothing
#endif

  return;
}

void Pasori::_close(const v8::FunctionCallbackInfo<v8::Value> &args){
  Isolate * isolate = args.GetIsolate();
  if (0 != args.Length()){
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Wrong number of arguments")));
    return;
  }
  Pasori* pasoriInstance = ObjectWrap::Unwrap<Pasori>(args.This());
  if(pasoriInstance->_pasori == NULL){
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Pasori device has not initialized.")));
    return;
  }

#if defined HAVE_LIBPAFE
  pasori_close(pasoriInstance->_pasori);
#elif defined HAVE_FELICALIB
  pasori_close(pasoriInstance->_pasori);
#endif

  return;
}

void Pasori::_polling(const v8::FunctionCallbackInfo<v8::Value> &args){
  uint16 systemcode;
  uint8 timeslot;
  Isolate * isolate = args.GetIsolate();
  if (2 < args.Length() || 0 == args.Length()){
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Wrong number of arguments")));
    return;
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
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Pasori device has not initialized.")));
    return;
  }

#if defined HAVE_LIBPAFE
  felica* _felica = felica_polling(pasoriInstance->_pasori, systemcode, 0, timeslot);
#elif defined HAVE_FELICALIB
  felica* _felica = felica_polling(pasoriInstance->_pasori, systemcode, 0, timeslot);
#endif

  if (_felica == NULL) {
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "FeliCa Polling error")));
    return;
  }
  
  Felica::NewInstance(args, _felica);
  
}

void Pasori::_get_error_code(const v8::FunctionCallbackInfo<v8::Value> &args){
  Isolate * isolate = args.GetIsolate();
  if (0 != args.Length()){
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Wrong number of arguments")));
    return;
  }

  Pasori* pasoriInstance = ObjectWrap::Unwrap<Pasori>(args.This());

  if(pasoriInstance->_pasori == NULL){
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Pasori device has not initialized.")));
    return;
  }

#if defined HAVE_LIBPAFE
  int error_code = pasori_get_error_code(pasoriInstance->_pasori);
  Local<Number> result = Number::New(isolate, error_code);
  return args.GetReturnValue().Set(result);
#elif defined HAVE_FELICALIB
  return;
#endif



}
