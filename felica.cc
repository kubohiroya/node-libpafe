#define BUILDING_NODE_EXTENTION

#include <node.h>
#include "felica.h"
#define DATA_LEN 16



  using v8::FunctionCallbackInfo;
  using v8::Isolate;
  using v8::Local;
  using v8::Object;
  using v8::Number;
  using v8::String;
  using v8::Array;
  using v8::Value;
  using v8::Persistent;
  using v8::Function;
  using v8::FunctionTemplate;
  using v8::Exception;

  Persistent<Function> Felica::constructor;

  Felica::Felica(felica *felica){
    this->_felica = felica;
  }
  
  Felica::~Felica(){
  }
  
  void Felica::Init(v8::Local<Object> exports){
    Isolate* isolate = exports->GetIsolate();
 
    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
    tpl->SetClassName(String::NewFromUtf8(isolate, "Felica"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    NODE_SET_PROTOTYPE_METHOD(tpl, "close", _close);
    NODE_SET_PROTOTYPE_METHOD(tpl, "read_single", _read_single);
    NODE_SET_PROTOTYPE_METHOD(tpl, "get_idm", _get_idm);
    NODE_SET_PROTOTYPE_METHOD(tpl, "get_pmm", _get_pmm);

    constructor.Reset(isolate, tpl->GetFunction());
  }

void Felica::NewInstance(const v8::FunctionCallbackInfo<v8::Value> & args, felica* felica){
  Felica* felicaObject = new Felica(felica);
  (felicaObject)->Wrap(args.This());
  args.GetReturnValue().Set(args.This());
}



  void Felica::_close(const v8::FunctionCallbackInfo<v8::Value> & args){
    Isolate * isolate = args.GetIsolate();
    if (0 != args.Length()){
      isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Wrong number of arguments")));
      return;
    }
    Felica* felica = ObjectWrap::Unwrap<Felica>(args.This());

    if(felica->_felica == NULL){
      isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "felica has not initialized.")));
      return;
    }

#if defined HAVE_LIBPAFE
    free(felica->_felica);
#elif defined HAVE_FELICALIB
    felica_free(felica->_felica);
#endif

    return;
  }

  void Felica::_read_single(const v8::FunctionCallbackInfo<v8::Value> & args){
    int servicecode;
    int mode;
    uint8 addr;
    int datalen = DATA_LEN;
    uint8 data[DATA_LEN];

    Isolate * isolate = args.GetIsolate();
    if (args.Length() < 1 || 3 < args.Length()){
      isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Wrong number of arguments")));
      return;
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
      isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "felica has not initialized.")));
      return;
    }

#if defined HAVE_LIBPAFE
    int ret = felica_read_single(felica->_felica, servicecode, mode, addr, data);
#elif defined HAVE_FELICALIB
    int ret = felica_read_without_encryption02(felica->_felica, servicecode, mode, addr, data);
#endif

    if(ret == 0){
      std::string str(data, data+datalen);
      return;
    }else{
      return;
    }
  }

  void Felica::_get_idm(const v8::FunctionCallbackInfo<v8::Value> & args){

    uint8 idm[8];

    Isolate * isolate = args.GetIsolate();
    if (args.Length() != 0){
      isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Wrong number of arguments")));
      return;
    }

    Felica* felica = ObjectWrap::Unwrap<Felica>(args.This());

    if(felica->_felica == NULL){
      isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "felica has not initialized.")));
      return;
    }

#if defined HAVE_LIBPAFE
    int ret = felica_get_idm(felica->_felica, idm);
#elif defined HAVE_FELICALIB
    int ret = 0;
    felica_getidm(felica->_felica, idm);
#endif

    if(ret == 0){
      Local<Array> result = Array::New(isolate, 8);
      for(int i = 0; i < 8; i++){
        result->Set(Number::New(isolate, i), Number::New(isolate, idm[i]));
      }
      args.GetReturnValue().Set(result);
    }else{
      return;
    }
  }

  void Felica::_get_pmm(const v8::FunctionCallbackInfo<v8::Value> & args){


    uint8 pmm[8];

    Isolate * isolate = args.GetIsolate();
    if (args.Length() != 0){
      isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Wrong number of arguments")));
      return;
    }

    Felica* felica = ObjectWrap::Unwrap<Felica>(args.This());

    if(felica->_felica == NULL){
      isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "felica has not initialized.")));
      return;
    }

#if defined HAVE_LIBPAFE
    int ret = felica_get_pmm(felica->_felica, pmm);
#elif defined HAVE_FELICALIB
    int ret = 0;
    felica_getpmm(felica->_felica, pmm);
#endif

    if(ret == 0){
      Local<Array> result = Array::New(isolate, 8);
      for(int i = 0; i < 8; i++){
        result->Set(Number::New(isolate, i), Number::New(isolate, pmm[i]));
      }
      return args.GetReturnValue().Set(result);
    }else{
      return;
    }
  }



//---




