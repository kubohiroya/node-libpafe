#include "pafe.h"

using v8::FunctionTemplate;

NAN_MODULE_INIT(InitAll) {
  Pasori::Init(target);
  Felica::Init(target);
}

NODE_MODULE(pafe_addon, InitAll)
