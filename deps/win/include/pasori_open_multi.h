#ifndef _PASORI_OPEN_MULTI_H
#define _PASORI_OPEN_MULTI_H

#include <windows.h>
#include "felicalib.h"

typedef struct tag_pasori_devices
{
  pasori **pasoris;
  int num_devices;
  int error_code;
} pasori_devices;

pasori_devices* pasori_open_multi(void);

#endif
