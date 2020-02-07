#include "pafe.h"
#include <iostream>
#include <string>
#include <sstream>
#include <stdio.h>

using namespace std;

Nan::Persistent<v8::Function> Pasori::constructor;
NAN_MODULE_INIT(Pasori::Init) {
  Nan::HandleScope scope;
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(PasoriNew);
  tpl->SetClassName(Nan::New("Pasori").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  Nan::SetPrototypeMethod(tpl, "close", PasoriClose);
  Nan::SetPrototypeMethod(tpl, "version", PasoriVersion);
  Nan::SetPrototypeMethod(tpl, "type", PasoriType);
  Nan::SetPrototypeMethod(tpl, "setTimeout", PasoriSetTimeout);
  Nan::SetPrototypeMethod(tpl, "polling", PasoriPolling);

  constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
  Nan::Set(target, Nan::New("Pasori").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}
Pasori::Pasori() {
}

Pasori::~Pasori() {
}

NAN_METHOD(Pasori::PasoriNew) {
  if (info.IsConstructCall()) {
    pasori *_pasori = pasori_open();
    if(_pasori == NULL){
      Nan::ThrowError("Unable to open pasori device");
      return info.GetReturnValue().SetUndefined();
    }
    pasori_init(_pasori);
    Pasori * pasoriObject = new Pasori();
    pasoriObject->Wrap(info.This());
    SetPasori(info.This(), _pasori);
    info.GetReturnValue().Set(info.This());
  } else {
    const int argc = 0;
    v8::Local<v8::Value> argv[argc] = {};
    v8::Local<v8::Function> cons = Nan::New(constructor);
    info.GetReturnValue().Set(Nan::NewInstance(cons, argc, argv).ToLocalChecked());
  }
}

pasori* Pasori::GetPasori(v8::Local<v8::Object> object){
  return static_cast<pasori *>(v8::Local<v8::External>::Cast(object->GetInternalField(0))->Value());
}

void Pasori::SetPasori(v8::Local<v8::Object> thisPasori, pasori* _pasori){
  v8::Isolate *isolate = v8::Isolate::GetCurrent();
  v8::HandleScope scope(isolate);
  thisPasori->SetInternalField(0, v8::External::New(isolate, _pasori));
}

NAN_METHOD(Pasori::PasoriClose) {
  pasori *_pasori = GetPasori(info.This());
  if(_pasori == NULL){
    Nan::ThrowError("Pasori device has not been initialized");
  }else{
    pasori_close(_pasori);
  }
  return info.GetReturnValue().SetUndefined();
}

NAN_METHOD(Pasori::PasoriVersion) {
  pasori *_pasori = GetPasori(info.This());
  if(_pasori == NULL){
    Nan::ThrowError("Pasori device has not been initialized");
    return info.GetReturnValue().SetUndefined();
  }else{
    int v1 = 0, v2 = 0;
    int ret = pasori_version(_pasori, &v1, &v2);
    if(ret != 0){
      Nan::ThrowError("fail to call pasori_version");
      return info.GetReturnValue().SetUndefined();
    }
    ostringstream ostr;
    ostr << "" << v1 << "." << v2;
    info.GetReturnValue().Set(Nan::New<v8::String>(ostr.str()).ToLocalChecked());
  }
}

NAN_METHOD(Pasori::PasoriType) {
  pasori *_pasori = GetPasori(info.This());
  if(_pasori == NULL){
    Nan::ThrowError("Pasori device has not been initialized");
    return info.GetReturnValue().SetUndefined();
  }else{
    int type = pasori_type(_pasori);
    if(type == -1){
      Nan::ThrowError("fail to call pasori_version");
      return info.GetReturnValue().SetUndefined();
    }
    /*
    switch(type){
    case PASORI_TYPE_S310:
      info.GetReturnValue().Set(Nan::New("S310"));
      break;
    case PASORI_TYPE_S320:
      info.GetReturnValue().Set(Nan::New("S320"));
      break;
    case PASORI_TYPE_S330:
      info.GetReturnValue().Set(Nan::New("S330"));
      break;
    }
    */
    info.GetReturnValue().Set(Nan::New(type));
  }
}

NAN_METHOD(Pasori::PasoriSetTimeout) {
  if(info.Length() != 1 || ! info[0]->IsNumber()){
    Nan::ThrowTypeError("the only first parameter must be a number");
    return info.GetReturnValue().SetUndefined();
  }
  pasori *_pasori = GetPasori(info.This());
  int timeout = info[0]->Int32Value(Nan::GetCurrentContext()).ToChecked();
  pasori_set_timeout(_pasori, timeout);
  return info.GetReturnValue().SetUndefined();
}

class AddAsyncFelicaPollingWorker : public Nan::AsyncWorker{
public:
  AddAsyncFelicaPollingWorker(pasori* _pasori,
			      uint16 systemcode,
			      uint8 timeslot,
			      Nan::Callback* callback)
    : Nan::AsyncWorker(callback), systemcode(systemcode), timeslot(timeslot){
    this->_felica = NULL;
    this->RFU = 0;
    this->_pasori = _pasori;
    this->systemcode = systemcode;
    this->timeslot = timeslot;
    this->errorObject = Nan::Null();
  }

  ~AddAsyncFelicaPollingWorker(){}

  void Execute(){
    _felica = felica_polling(_pasori, systemcode, RFU, timeslot);
  }
  
  void HandleOKCallback(){
    if(_felica == NULL){
      v8::Local<v8::Value> callbackArgs[] = {      
	Nan::Null()
      };
      callback->Call(1, callbackArgs);
      return;
    }else{
      v8::Local<v8::Object> thisFelica = Felica::FelicaNewInstance(_felica);

      v8::Local<v8::Value> callbackArgs[] = {   
	thisFelica
      };
      callback->Call(1, callbackArgs);
      return;
    }
  }
  
  void HandleErrorCallback(){
  }
  
private:
  pasori* _pasori;
  felica* _felica;
  uint16 systemcode;
  uint8 RFU;
  uint8 timeslot;
  v8::Local<v8::Value> errorObject;
};


NAN_METHOD(Pasori::PasoriPolling) {
  pasori *_pasori = GetPasori(info.This());
  if(_pasori == NULL){
    Nan::ThrowError("Pasori device has not been initialized");
    return info.GetReturnValue().SetUndefined();
  }
  if(info.Length() != 3){
    Nan::ThrowTypeError("3 Arguments must be required");
    return info.GetReturnValue().SetUndefined();
  }
  if(!info[2]->IsFunction()){
    Nan::ThrowTypeError("The 3rd parameter must be a function");
    return info.GetReturnValue().SetUndefined();
  }

  auto callback = new Nan::Callback(info[2].As<v8::Function>());
  Nan::AsyncQueueWorker(new AddAsyncFelicaPollingWorker(_pasori,
							info[0]->Int32Value(Nan::GetCurrentContext()).ToChecked(),
							info[1]->Int32Value(Nan::GetCurrentContext()).ToChecked(),
							callback));
  return info.GetReturnValue().SetUndefined();
}


/*******************************************/

Nan::Persistent<v8::Function> Felica::constructor;

NAN_MODULE_INIT(Felica::Init) {
  Nan::HandleScope scope;
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(FelicaNew);
  tpl->SetClassName(Nan::New("Felica").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  Nan::SetPrototypeMethod(tpl, "close", FelicaClose);
  Nan::SetPrototypeMethod(tpl, "getIDm", FelicaGetIDm);
  Nan::SetPrototypeMethod(tpl, "getPMm", FelicaGetPMm);
  Nan::SetPrototypeMethod(tpl, "readSingle", FelicaReadSingle);
  Nan::SetPrototypeMethod(tpl, "readMulti", FelicaReadMulti);
  Nan::SetPrototypeMethod(tpl, "writeSingle", FelicaWriteSingle);
  Nan::SetPrototypeMethod(tpl, "writeMulti", FelicaWriteMulti);

 constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
  Nan::Set(target, Nan::New("Felica").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}

Felica::Felica(felica* _felica) {
  this->_felica = _felica;
}

Felica::~Felica() {
}

v8::Local<v8::Object> Felica::FelicaNewInstance(felica * _felica) {
  Nan::EscapableHandleScope scope;

  const int argc = 1;
  v8::Local<v8::Value> argv[argc] = {
    Nan::New<v8::External>(_felica)
  };
  v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
  //v8::Local<v8::Object> instance = cons->NewInstance(argc, argv);
  v8::Local<v8::Object> instance = Nan::NewInstance(cons, argc, argv).ToLocalChecked();

  return scope.Escape(instance);
}

NAN_METHOD(Felica::FelicaNew) {
  if(info.Length() != 1){
    Nan::ThrowTypeError("Argument Error");
    return info.GetReturnValue().SetUndefined();
  }
    
  if (info.IsConstructCall()) {
    felica* _felica = static_cast<felica*>(info[0].As<v8::External>()->Value());
    Felica *felicaObject = new Felica(_felica);
    felicaObject->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
  } else {
    const int argc = 1;
    v8::Local<v8::Value> argv[argc] = {info[0]};
    v8::Local<v8::Function> cons = Nan::New(constructor);
    info.GetReturnValue().Set(Nan::NewInstance(cons, argc, argv).ToLocalChecked());
  }
}

NAN_METHOD(Felica::FelicaClose) {
  Felica* felicaObject = ObjectWrap::Unwrap<Felica>(info.This());
  felica* _felica = felicaObject->_felica;
  if(_felica == NULL){
    Nan::ThrowError("felica has not been initialized.");
    return info.GetReturnValue().SetUndefined();
  }
  free(_felica);
  return info.GetReturnValue().SetUndefined();
}

NAN_METHOD(Felica::FelicaGetIDm) {
  Felica* felicaObject = ObjectWrap::Unwrap<Felica>(info.This());
  felica* _felica = felicaObject->_felica;
  if(_felica == NULL){
    Nan::ThrowError("felica has not been initialized");
    return info.GetReturnValue().SetUndefined();
  }

  uint8 idm[FELICA_IDM_LENGTH];
  char idmstr[FELICA_IDM_LENGTH*2+1];
  
#if defined HAVE_LIBPAFE
  int ret = felica_get_idm(_felica, idm);
#elif defined HAVE_FELICALIB
  int ret = 0;  
  felica_getidm(_felica, idm);
#endif
  if(ret == 0){
    v8::Local<v8::Array> arr = Nan::New<v8::Array>(8);
    for(int i = 0; i < 8; i++){
      Nan::Set(arrInBlock, i, Nan::New(idm[i]));
    }
    info.GetReturnValue().Set(arr);
  }else{
    Nan::ThrowTypeError("internal error on getIDm");
    return info.GetReturnValue().SetUndefined();
  }
}

NAN_METHOD(Felica::FelicaGetPMm) {
  Felica* felicaObject = ObjectWrap::Unwrap<Felica>(info.This());
  felica* _felica = felicaObject->_felica;
  if(_felica == NULL){
    Nan::ThrowError("felica has not been initialized");
    return info.GetReturnValue().SetUndefined();
  }

  uint8 pmm[FELICA_PMM_LENGTH];
  char pmmstr[FELICA_PMM_LENGTH*2+1];
  
#if defined HAVE_LIBPAFE
  int ret = felica_get_pmm(_felica, pmm);
#elif defined HAVE_FELICALIB
  int ret = 0;
  felica_getpmm(_felica, pmm);
#endif
  if(ret == 0){//success
    v8::Local<v8::Array> arr = Nan::New<v8::Array>(8);
    for(int i = 0; i < 8; i++){
      Nan::Set(arrInBlock, i, Nan::New(pmm[i]));
    }
    info.GetReturnValue().Set(arr);
  }else{
    Nan::ThrowTypeError("internal error on getPMm");
    return info.GetReturnValue().SetUndefined();
  }
}

NAN_METHOD(Felica::FelicaReadSingle) {
  int servicecode;
  int mode;
  uint8 addr;
  uint8 data[FELICA_DATA_LEN];

  if (info.Length() < 1 || 3 < info.Length()){
    Nan::ThrowTypeError("Wrong number of arguments");
    return info.GetReturnValue().SetUndefined();
  }
  if (! info[2]->IsNumber()){
    Nan::ThrowTypeError("3rd argument must be blockaddr:integer");
    return info.GetReturnValue().SetUndefined();
  }else{
    addr = info[2]->Int32Value(Nan::GetCurrentContext()).ToChecked();
  }
  if (! info[1]->IsNumber()){
    Nan::ThrowTypeError("2nd argument must be mode:integer");
    return info.GetReturnValue().SetUndefined();
  }else{
    mode = info[1]->Int32Value(Nan::GetCurrentContext()).ToChecked();
  }
  if (! info[0]->IsNumber()){
    Nan::ThrowTypeError("1st argument must be servicecode:number");
    return info.GetReturnValue().SetUndefined();
  }else{
    servicecode = info[0]->Int32Value(Nan::GetCurrentContext()).ToChecked();
  }

  Felica* felicaObject = ObjectWrap::Unwrap<Felica>(info.This());
  felica* _felica = felicaObject->_felica;
  if(_felica == NULL){
    Nan::ThrowError("felica has not been initialized");
    return info.GetReturnValue().SetUndefined();
  }

#if defined HAVE_LIBPAFE
  int ret = felica_read_single(_felica, servicecode, mode, addr, data);
#elif defined HAVE_FELICALIB
  int ret = felica_read_without_encryption02(_felica, servicecode, mode, addr, data);
#endif

  if(ret == 0){
    v8::Local<v8::Array> arr = Nan::New<v8::Array>(FELICA_DATA_LEN);
    for(int i = 0; i < FELICA_DATA_LEN; i++){
      Nan::Set(arr, i, Nan::New(data[i]));
    }
    info.GetReturnValue().Set(arr);
  }else{
    Nan::ThrowError("felica read error");
    return info.GetReturnValue().SetUndefined();
  }
}

NAN_METHOD(Felica::FelicaWriteSingle) {
  int servicecode;
  int mode;
  uint8 addr;
  uint8 data[FELICA_DATA_LEN];

  if (info.Length() < 1 || 4 < info.Length()){
    Nan::ThrowTypeError("Wrong number of arguments");
    return info.GetReturnValue().SetUndefined();
  }
  
  if (! info[0]->IsNumber()){
    Nan::ThrowTypeError("1st argument must be servicecode:number");
    return info.GetReturnValue().SetUndefined();
  }else{
    servicecode = info[0]->Int32Value(Nan::GetCurrentContext()).ToChecked();
  }

  if (! info[1]->IsNumber()){
    Nan::ThrowTypeError("2nd argument must be mode:integer");
    return info.GetReturnValue().SetUndefined();
  }else{
    mode = info[1]->Int32Value(Nan::GetCurrentContext()).ToChecked();
  }
  
  if (! info[2]->IsNumber()){
    Nan::ThrowTypeError("3rd argument must be blockaddr:integer");
    return info.GetReturnValue().SetUndefined();
  }else{
    addr = info[2]->Int32Value(Nan::GetCurrentContext()).ToChecked();
  }

  if (! info[3]->IsArray()){
    Nan::ThrowError("4th argument must be Array type");
    return info.GetReturnValue().SetUndefined();
  }else{
    v8::Local<v8::Array> arr = info[3].As<v8::Array>();
    for(int i = 0; i < FELICA_DATA_LEN; i++){
      data[i] = Nan::To<v8::Uint32>(arr->Get(i)).ToLocalChecked()->Value();
    }
  }

  Felica* felicaObject = ObjectWrap::Unwrap<Felica>(info.This());
  felica* _felica = felicaObject->_felica;
  if(_felica == NULL){
    Nan::ThrowError("felica has not been initialized");
    return;
  }

#if defined HAVE_LIBPAFE
  int ret = felica_write_single(_felica, servicecode, mode, addr, data);
#elif defined HAVE_FELICALIB
  int ret = felica_write_without_encryption02(_felica, servicecode, mode, addr, data);
#endif

  if(ret == 0){
    return info.GetReturnValue().SetUndefined();
  }else{
    Nan::ThrowError("felica write error");
    return info.GetReturnValue().SetUndefined();
  }
}

NAN_METHOD(Felica::FelicaReadMulti) {
  int n;
  uint8 servicecode[4];
  uint8 mode[4];
  uint8 addr[4];
  uint8 data[FELICA_DATA_LEN*4];

  if (info.Length() < 1 || 4 < info.Length()){
    Nan::ThrowTypeError("FelicaReadMulti: Wrong number of arguments");
    return info.GetReturnValue().SetUndefined();
  }

  if (! info[0]->IsNumber()){
    Nan::ThrowTypeError("FelicaReadMulti: 1st argument must be n:number");
    return info.GetReturnValue().SetUndefined();
  }else{
    n = info[0]->Int32Value(Nan::GetCurrentContext()).ToChecked();
  }
  
  if (! info[1]->IsArray()){ 
    Nan::ThrowTypeError("FelicaReadMulti: 2nd argument must be servicecode:number[]");
    return info.GetReturnValue().SetUndefined();
  }else{
    v8::Local<v8::Array> arr = info[1].As<v8::Array>();
    for(int i = 0; i < n; i++){
      servicecode[i] = Nan::To<v8::Uint32>(arr->Get(i)).ToLocalChecked()->Value();
    }
  }
     
  if (! info[2]->IsArray()){ 
    Nan::ThrowTypeError("FelicaReadMulti: 3rd argument must be mode:number[]");
    return info.GetReturnValue().SetUndefined();
  }else{
    v8::Local<v8::Array> arr = info[2].As<v8::Array>();
    for(int i = 0; i < n; i++){
      mode[i] = Nan::To<v8::Uint32>(arr->Get(i)).ToLocalChecked()->Value();
    }
  }
  
  if (! info[3]->IsArray()){ 
    Nan::ThrowTypeError("FelicaReadMulti: 4th argument must be addr:number[]");
    return info.GetReturnValue().SetUndefined();
  }else{
    v8::Local<v8::Array> arr = info[3].As<v8::Array>();
    for(int i = 0; i < n; i++){
      addr[i] = Nan::To<v8::Uint32>(arr->Get(i)).ToLocalChecked()->Value();
    }
  }

  Felica* felicaObject = ObjectWrap::Unwrap<Felica>(info.This());
  felica* _felica = felicaObject->_felica;
  if(_felica == NULL){
    Nan::ThrowError("felica has not been initialized");
    return info.GetReturnValue().SetUndefined();
  }

#if defined HAVE_LIBPAFE
  int ret = felica_read_multi(_felica, n, servicecode, mode, addr, data);
#elif defined HAVE_FELICALIB
  int ret = felica_read_without_encryption02(_felica, servicecode, mode, addr, data);
#endif

  if(ret == 0){
    v8::Local<v8::Array> arr = Nan::New<v8::Array>(n);
    for(int j = 0; j < n; j++){
      v8::Local<v8::Array> arrInBlock = Nan::New<v8::Array>(FELICA_DATA_LEN);
      Nan::Set(arr, j, arrInBlock);
      for(int i = 0; i < FELICA_DATA_LEN; i++){
	Nan::Set(arrInBlock, i, Nan::New(data[FELICA_DATA_LEN * j + i]));
      }
    }
    info.GetReturnValue().Set(arr);
  }else{
    Nan::ThrowError("felica read error");
    return info.GetReturnValue().SetUndefined();
  }
}

NAN_METHOD(Felica::FelicaWriteMulti) {
  int n;
  uint8 servicecode[2];
  uint8 mode[2];
  uint8 addr[2];
  uint8 data[FELICA_DATA_LEN*2];

  if (info.Length() < 1 || 5 < info.Length()){
    Nan::ThrowTypeError("FelicaWriteMulti: Wrong number of arguments");
    return info.GetReturnValue().SetUndefined();
  }
 
  if (! info[0]->IsNumber()){
    Nan::ThrowTypeError("FelicaWriteMulti: 1st argument must be n:number");
    return info.GetReturnValue().SetUndefined();
  }else{
    n = info[0]->Int32Value(Nan::GetCurrentContext()).ToChecked();
  }
  

  if (! info[1]->IsArray()){
    Nan::ThrowTypeError("FelicaWriteMulti: 2nd argument must be servicecode:number[]");
    return info.GetReturnValue().SetUndefined();
  }else{
    v8::Local<v8::Array> arr = info[1].As<v8::Array>();
    for(int i = 0; i < n; i++){
      servicecode[i] = Nan::To<v8::Uint32>(arr->Get(i)).ToLocalChecked()->Value();
    }
  }
  
  if (! info[2]->IsArray()){
    Nan::ThrowTypeError("FelicaWriteMulti: 3rd argument must be mode:number[]");
    return info.GetReturnValue().SetUndefined();
  }else{
    v8::Local<v8::Array> arr = info[2].As<v8::Array>();
    for(int i = 0; i < n; i++){
      mode[i] = Nan::To<v8::Uint32>(arr->Get(i)).ToLocalChecked()->Value();
    }
  }

  if (! info[3]->IsArray()){
    Nan::ThrowTypeError("FelicaWriteMulti: 4th argument must be addr:number");
    return info.GetReturnValue().SetUndefined();
  }else{
    v8::Local<v8::Array> arr = info[3].As<v8::Array>();
    for(int i = 0; i < n; i++){
      addr[i] = Nan::To<v8::Uint32>(arr->Get(i)).ToLocalChecked()->Value();
    }
  }

  if (! info[4]->IsArray()){
    Nan::ThrowError("FelicaWriteMulti: 5th argument must be Array type");
    return info.GetReturnValue().SetUndefined();
  }else{
    v8::Local<v8::Array> arr = info[4].As<v8::Array>();
    for(int i = 0; i < FELICA_DATA_LEN * n; i++){
      data[i] = Nan::To<v8::Uint32>(arr->Get(i)).ToLocalChecked()->Value();
    }
  }
  

  Felica* felicaObject = ObjectWrap::Unwrap<Felica>(info.This());
  felica* _felica = felicaObject->_felica;
  if(_felica == NULL){
    Nan::ThrowError("felica has not been initialized");
    return;
  }

#if defined HAVE_LIBPAFE
  int ret = felica_write_multi(_felica, n, servicecode, mode, addr, data);
#elif defined HAVE_FELICALIB
  int ret = felica_write_without_encryption02(_felica, servicecode, mode, addr, data);
#endif

  if(ret == 0){
    return info.GetReturnValue().SetUndefined();
  }else{
    Nan::ThrowError("felica write error");
    return info.GetReturnValue().SetUndefined();
  }
}
