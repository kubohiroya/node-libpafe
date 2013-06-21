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
  

void Pasori::Init(){
 
  Local<FunctionTemplate> tpl = FunctionTemplate::New(Pasori::New);
  tpl->SetClassName(String::NewSymbol("Pasori"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  NODE_SET_PROTOTYPE_METHOD(tpl, "open", Pasori::_open);
  NODE_SET_PROTOTYPE_METHOD(tpl, "set_timeout", Pasori::_set_timeout);
  NODE_SET_PROTOTYPE_METHOD(tpl, "init", Pasori::_init);
  NODE_SET_PROTOTYPE_METHOD(tpl, "reset", Pasori::_reset);
  NODE_SET_PROTOTYPE_METHOD(tpl, "close", Pasori::_close);
  NODE_SET_PROTOTYPE_METHOD(tpl, "polling", Pasori::_polling);
  NODE_SET_PROTOTYPE_METHOD(tpl, "get_error_code", Pasori::_get_error_code);

  constructor = Persistent<Function>::New(tpl->GetFunction());
}

Handle<Value> Pasori::New(const Arguments & args){
  HandleScope scope;
  Pasori* pasoriObject = new Pasori();
  (pasoriObject)->Wrap(args.This());
  return args.This();
}

Handle<Value> Pasori::NewInstance(const Arguments & args){
  HandleScope scope;
  Handle<Value> argv[1] = {args[0]};
  Local<Object> instance = constructor->NewInstance(1, argv);
  return scope.Close(instance);
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

#if defined HAVE_LIBPAFE
  pasori * __pasori = pasori_open();
#elif defined HAVE_FELICALIB
  pasori * __pasori = pasori_open(NULL);
#endif

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
    ThrowException(Exception::TypeError(String::New("Pasori device has not initialized: timeout")));
    return scope.Close(Undefined());
  }

#if defined HAVE_LIBPAFE
  pasori_set_timeout(pasoriInstance->_pasori, timeout);
#elif defined HAVE_FELICALIB
  // do nothing
#endif

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

#if defined HAVE_LIBPAFE
  pasori_init(pasoriInstance->_pasori);
#elif defined HAVE_FELICALIB
  pasori_init(pasoriInstance->_pasori);
#endif

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

#if defined HAVE_LIBPAFE
  pasori_reset(pasoriInstance->_pasori);
#elif defined HAVE_FELICALIB
  // do nothing
#endif

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

#if defined HAVE_LIBPAFE
  pasori_close(pasoriInstance->_pasori);
#elif defined HAVE_FELICALIB
  pasori_close(pasoriInstance->_pasori);
#endif

  return scope.Close(Undefined());
}

Handle<Value> Pasori::_polling(const Arguments & args){
  HandleScope scope;
  uint16 systemcode;
  uint8 timeslot;

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

#if defined HAVE_LIBPAFE
  felica* _felica = felica_polling(pasoriInstance->_pasori, systemcode, 0, timeslot);
#elif defined HAVE_FELICALIB
  felica* _felica = felica_polling(pasoriInstance->_pasori, systemcode, 0, timeslot);
#endif

  if (_felica == NULL) {
    ThrowException(Exception::TypeError(String::New("FeliCa Polling error")));
    return scope.Close(Undefined());
  }

  //Handle<Value> argv[0] = { };
  Local<Object> felicaInstance = Felica::constructor->NewInstance(0, NULL);
  Felica* felicaObject = ObjectWrap::Unwrap<Felica>(felicaInstance);
  felicaObject->_felica = _felica;
  return scope.Close(felicaInstance);

}

Handle<Value> Pasori::_get_error_code(const Arguments & args){
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

#if defined HAVE_LIBPAFE
  int error_code = pasori_get_error_code(pasoriInstance->_pasori);
  Local<Number> result = Number::New(error_code);
  return scope.Close(result);
#elif defined HAVE_FELICALIB
  return scope.Close(Undefined());
#endif



}
