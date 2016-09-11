#ifndef __DEBUG_H__
#define __DEBUG_H__

#define DEBUG

#include <stdlib.h>
#include "Arduino.h"

#ifdef DEBUG
#define DMSG(args...)       printf (args)
#define DMSG_STR(str)       printf (str)
#define DMSG_HEX(num)       printf (" %02X", num)
#define DMSG_INT(num)       printf (" %d ", num)
#else
#define DMSG(args...)
#define DMSG_STR(str)
#define DMSG_HEX(num)
#define DMSG_INT(num)
#endif

#endif
