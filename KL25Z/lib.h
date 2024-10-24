#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"

#define MASK(x) (1 << (x))

#define FULL_SPEED 7500
#define SLOW_SPEED 5000  // 66%
