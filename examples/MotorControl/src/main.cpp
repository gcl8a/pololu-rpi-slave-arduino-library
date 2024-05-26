#include <Arduino.h>
#include "RomiTWIPeripheral.h"

RomiTWIPeripheral romi;

void setup() 
{
  romi.init(0x28);
}

void loop() 
{
  
}
