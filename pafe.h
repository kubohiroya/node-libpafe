#ifndef NODE_PAFE_H
#define NODE_PAFE_H

#include <nan.h>

#if defined(HAVE_LIBPAFE)
extern "C"{
#include <libpafe.h>
}
#elif defined(HAVE_FELICALIB)
extern "C"{
#include <felicalib.h>
}
#else
#error "unknown platform"
#endif

#define FELICA_DATA_LEN 16

// Based on https://nodejs.org/api/addons.html#addons_wrapping_c_objects but using NAN
class Pasori : public Nan::ObjectWrap {
 public:
  static NAN_MODULE_INIT(Init);

 private:
  explicit Pasori();
  ~Pasori();
  static NAN_METHOD(PasoriNew);
  static NAN_METHOD(PasoriClose);
  static NAN_METHOD(PasoriVersion);
  static NAN_METHOD(PasoriType);
  static NAN_METHOD(PasoriSetTimeout);
  static NAN_METHOD(PasoriPolling);
  static Nan::Persistent<v8::Function> constructor;
  static pasori* GetPasori(v8::Handle<v8::Object> object);
  static void SetPasori(v8::Local<v8::Object> thisPasori, pasori* _pasori);
};

class Felica : public Nan::ObjectWrap {
 public:
  static NAN_MODULE_INIT(Init);
  static v8::Local<v8::Object> FelicaNewInstance(felica *_felica);
  
 private:
  explicit Felica(felica* _felica);
  ~Felica();
  static NAN_METHOD(FelicaNew);
  static NAN_METHOD(FelicaClose);
  static NAN_METHOD(FelicaGetIDm);
  static NAN_METHOD(FelicaGetPMm);
  static NAN_METHOD(FelicaRead);
  static NAN_METHOD(FelicaReadSingle);
  static NAN_METHOD(FelicaWrite);
  static Nan::Persistent<v8::Function> constructor;
  felica* _felica; 
};

#endif
