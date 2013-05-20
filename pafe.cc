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

class PaFe : ObjectWrap {
public:

  static void Init(Handle<Object> target){
    Local<FunctionTemplate> t = FunctionTemplate::New(PaFe::New);
    NODE_SET_PROTOTYPE_METHOD(t, "pasori_open", PaFe::_pasori_open);
    NODE_SET_PROTOTYPE_METHOD(t, "pasori_set_timeout", PaFe::_pasori_set_timeout);
    NODE_SET_PROTOTYPE_METHOD(t, "pasori_close", PaFe::_pasori_close);
    NODE_SET_PROTOTYPE_METHOD(t, "felica_close", PaFe::_felica_close);
    NODE_SET_PROTOTYPE_METHOD(t, "felica_polling", PaFe::_felica_polling);
    NODE_SET_PROTOTYPE_METHOD(t, "felica_read_single", PaFe::_felica_read_single);
    t->InstanceTemplate()->SetInternalFieldCount(1);
    target->Set(String::New("PaFe"), t->GetFunction());
  }

  PaFe(){
    this->_pasori = NULL;
    this->_felica = NULL;
  }

  ~PaFe(){
  }

  static Handle<Value> New(const Arguments & args){
    HandleScope scope;
    if (!args.IsConstructCall()){
      return args.Callee()->NewInstance();
    }

    PaFe* pafe = new PaFe();
    (pafe)->Wrap(args.Holder());
    return scope.Close(args.Holder());
  }

  static Handle<Value> _pasori_open(const Arguments & args){
    HandleScope scope;

    if (0 != args.Length()){
      ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
      return scope.Close(Undefined());
    }

    PaFe* pafe = ObjectWrap::Unwrap<PaFe>(args.This());

    if(pafe->_pasori != NULL){
      ThrowException(Exception::TypeError(String::New("Pasori device is already initialized.")));
      return scope.Close(Undefined());
    }

    pasori * _pasori = pasori_open();

    if (_pasori == NULL) {
      ThrowException(Exception::TypeError(String::New("pasori open error")));
      return scope.Close(Undefined());
    }

    pafe->_pasori = _pasori;

    return scope.Close(Undefined());
  }

  static Handle<Value> _pasori_set_timeout(const Arguments & args){
    HandleScope scope;
    int timeout = 0;
    if (1 != args.Length()){
      ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
      return scope.Close(Undefined());
    }
    if (0 < args.Length()){
      timeout = args[0]->NumberValue();
    }

    PaFe* pafe = ObjectWrap::Unwrap<PaFe>(args.This());
    if(pafe->_pasori == NULL){
      ThrowException(Exception::TypeError(String::New("Pasori device has not initialized.")));
      return scope.Close(Undefined());
    }
    pasori_set_timeout(pafe->_pasori, timeout);

    return scope.Close(Undefined());
  }

  static Handle<Value> _pasori_close(const Arguments & args){
    HandleScope scope;
    if (0 != args.Length()){
      ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
      return scope.Close(Undefined());
    }
    PaFe* pafe = ObjectWrap::Unwrap<PaFe>(args.This());
    if(pafe->_pasori == NULL){
      ThrowException(Exception::TypeError(String::New("Pasori device has not initialized.")));
      return scope.Close(Undefined());
    }
    pasori_close(pafe->_pasori);

    free(pafe->_pasori);

    return scope.Close(Undefined());
  }

  static Handle<Value> _felica_close(const Arguments & args){
    HandleScope scope;
    if (0 != args.Length()){
      ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
      return scope.Close(Undefined());
    }
    PaFe* pafe = ObjectWrap::Unwrap<PaFe>(args.This());

    if(pafe->_felica == NULL){
      ThrowException(Exception::TypeError(String::New("felica has not initialized.")));
      return scope.Close(Undefined());
    }

    free(pafe->_felica);

    return scope.Close(Undefined());
  }

  static Handle<Value> _felica_polling(const Arguments & args){
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

    PaFe* pafe = ObjectWrap::Unwrap<PaFe>(args.This());

    if(pafe->_pasori == NULL){
      ThrowException(Exception::TypeError(String::New("Pasori device has not initialized.")));
      return scope.Close(Undefined());
    }

    felica * _felica = felica_polling(pafe->_pasori, systemcode, 0, timeslot);

    if (_felica == NULL) {
      return scope.Close(Boolean::New(FALSE));
    }

    pafe->_felica = _felica;

    return scope.Close(Boolean::New(TRUE));
  }

  static Handle<Value> _felica_read_single(const Arguments& args) {
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

    PaFe* pafe = ObjectWrap::Unwrap<PaFe>(args.This());

    if(pafe->_pasori == NULL){
      ThrowException(Exception::TypeError(String::New("Pasori device has not initialized.")));
      return scope.Close(Undefined());
    }
    if(pafe->_felica == NULL){
      ThrowException(Exception::TypeError(String::New("felica has not initialized.")));
      return scope.Close(Undefined());
    }

    int ret = felica_read_single(pafe->_felica, servicecode, mode, addr, data);
    if(ret == 0){
      std::string str(data, data+datalen);
      return scope.Close(String::New(str.c_str (), str.length()));
    }else{
      return scope.Close(Undefined());
    }
  }

private:
  pasori * _pasori;
  felica * _felica;

};


void init(Handle<Object> exports) {
  PaFe::Init(exports);
}

NODE_MODULE(pafe, init)


