#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"
#include <MKL25Z4.h>   // Include the KL25Z header file

#define MASK(x) (1 << (x))

#define FULL_SPEED 7500
#define SLOW_SPEED 5000  // 66%
