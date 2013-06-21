#define BUILDING_NODE_EXTENTION

#include <node.h>

#include "pasori.h"
#include "felica.h"


using namespace v8;

Handle<Value> OpenPasoriSingle(const Arguments & args){
  HandleScope scope;

  if (0 != args.Length()){
    ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
    return scope.Close(Undefined());
  }

  pasori * _pasori = pasori_open();

  if (_pasori == NULL) {
    ThrowException(Exception::TypeError(String::New("pasori open error")));
    return scope.Close(Undefined());
  }

  Handle<Value> argv[0] = { };
  Local<Object> pasoriInstance = Pasori::constructor->NewInstance(0, argv);
  Pasori* pasoriObject = ObjectWrap::Unwrap<Pasori>(pasoriInstance);
  pasoriObject->_pasori = _pasori;
  return scope.Close(pasoriInstance);
}

Handle<Value> OpenPasoriMulti(const Arguments & args){
  HandleScope scope;

  if (0 != args.Length()){
    ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
    return scope.Close(Undefined());
  }

  pasori_devices * _pasori_devices = pasori_open_multi();

  if (_pasori_devices == NULL) {
    ThrowException(Exception::TypeError(String::New("pasori open error")));
    return scope.Close(Undefined());
  }

  if(_pasori_devices->error_code != 0){
    ThrowException(Exception::TypeError(String::New("pasori init error")));
    return scope.Close(Undefined());
  }

  Local<Array> array = Array::New(_pasori_devices->num_devices);
  for (unsigned int i = 0; i < array->Length(); ++i) {

    Handle<Value> argv[0] = { };
    Local<Object> pasoriInstance = Pasori::constructor->NewInstance(0, argv);
    Pasori* pasoriObject = ObjectWrap::Unwrap<Pasori>(pasoriInstance);
    pasoriObject->_pasori = _pasori_devices->pasoris[i];

    array->Set(Int32::New(i), pasoriInstance);
  }
  
  return scope.Close(array);
}

void InitAll(Handle<Object> exports) {
  Felica::Init();
  Pasori::Init();

  exports->Set(String::NewSymbol("open_pasori"), 
               FunctionTemplate::New(OpenPasoriSingle)->GetFunction());
  exports->Set(String::NewSymbol("open_pasori_multi"), 
               FunctionTemplate::New(OpenPasoriMulti)->GetFunction());
}

NODE_MODULE(pafe, InitAll)
