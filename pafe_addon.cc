#include "pafe.h"

using v8::FunctionTemplate;

NAN_MODULE_INIT(InitAll) {
  PasoriObject::Init(target);
  FelicaObject::Init(target);
}

NODE_MODULE(pafe_addon, InitAll)
