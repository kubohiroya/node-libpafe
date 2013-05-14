#define BUILDING_NODE_EXTENTION


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


class PaFe : ObjectWrap {
public:

  static void Init(Handle<Object> target){
    Local<FunctionTemplate> t = FunctionTemplate::New(PaFe::New);
    NODE_SET_PROTOTYPE_METHOD(t, "close", PaFe::close);
    NODE_SET_PROTOTYPE_METHOD(t, "closeFelica", PaFe::closeFelica);
    NODE_SET_PROTOTYPE_METHOD(t, "polling", PaFe::polling);
    NODE_SET_PROTOTYPE_METHOD(t, "readSingle", PaFe::readSingle);
    t->InstanceTemplate()->SetInternalFieldCount(1);
    target->Set(String::New("PaFe"), t->GetFunction());
  }

  PaFe(){
  }

  ~PaFe(){
  }

  static Handle<Value> close(const Arguments & args){
    HandleScope scope;
    if (0 != args.Length()){
      ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
      return scope.Close(Undefined());
    }
    PaFe* pafe = ObjectWrap::Unwrap<PaFe>(args.This());
    pasori_close(pafe->_pasori);
    free(pafe->_pasori);
    return scope.Close(Undefined());
  }

  static Handle<Value> closeFelica(const Arguments & args){
    HandleScope scope;
    if (0 != args.Length()){
      ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
      return scope.Close(Undefined());
    }
    PaFe* pafe = ObjectWrap::Unwrap<PaFe>(args.This());
    free(pafe->_felica);
    return scope.Close(Undefined());
  }

  static Handle<Value> New(const Arguments & args){
    HandleScope scope;
    if (!args.IsConstructCall()){
      return args.Callee()->NewInstance();
    }

    PaFe* pafe = new PaFe();
    pasori * _pasori = pasori_open();

    pasori_set_timeout(_pasori, 50);

    pafe->_pasori = _pasori;

    if (_pasori == NULL) {
      ThrowException(Exception::TypeError(String::New("pasori open error")));
      return scope.Close(Undefined());
    }

    (pafe)->Wrap(args.Holder());
    return scope.Close(args.Holder());
  }

  static Handle<Value> polling(const Arguments & args){
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
    felica * _felica = felica_polling(pafe->_pasori, systemcode, 0, timeslot);

    if (_felica == NULL) {
      return scope.Close(Integer::New(-1));
    }

    pafe->_felica = _felica;

    return scope.Close(Integer::New(0));
  }

  static Handle<Value> readSingle(const Arguments& args) {
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

    PaFe* obj = ObjectWrap::Unwrap<PaFe>(args.This());

    int ret = felica_read_single(obj->_felica, servicecode, mode, addr, data);
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


