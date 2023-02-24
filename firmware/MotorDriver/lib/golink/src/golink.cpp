#include "stdint.h"
#include "arduino-esp32-can/src/CAN.h"
#include "isotp-c/isotp.h"
#include "binn/src/binn.h"

binn *obj;



void testFunction(void)
{
    obj = binn_object();
}