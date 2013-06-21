#include "pasori_open_multi.h"

pasori_devices* 
pasori_open_multi(void)
{
  pasori_devices* pd;

  pd = (pasori_devices *)malloc(sizeof(pasori_devices));

  pd->error_code = 0;
  pd->num_devices = 1;

  pd->pasoris = (pasori **) malloc(sizeof(pasori *) * pd->num_devices);
  
  pd->pasoris[0] = pasori_open(NULL);
  
  return pd;
};
