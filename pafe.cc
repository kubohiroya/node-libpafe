#define BUILDING_NODE_EXTENTION

#include <node.h>

#include "pasori.h"
#include "felica.h"

void OpenPasoriSingle(const v8::FunctionCallbackInfo<v8::Value> & args){
  Isolate * isolate = args.GetIsolate();
  if (0 != args.Length()){
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Wrong number of arguments")));
    return;
  }
  Pasori::NewInstance(args);
}

void InitAll(Local<Object> exports, Local<Object> module) {
  Felica::Init(exports);
  Isolate * isolate = exports->GetIsolate();
  Pasori::Init(isolate);
  exports->Set(String::NewFromUtf8(isolate, "open_pasori"), 
               FunctionTemplate::New(isolate, OpenPasoriSingle)->GetFunction());
}

NODE_MODULE(pafe, InitAll)
