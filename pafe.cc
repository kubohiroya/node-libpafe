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

  return scope.Close(Undefined());
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
