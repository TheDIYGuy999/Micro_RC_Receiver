#ifndef pid_h
#define pid_h

#include "Arduino.h"

//
// =======================================================================================================
// LOOP TIME MEASUREMENT
// =======================================================================================================
//

unsigned int loopTime;

void loopDuration() {
  static unsigned long timerOld;
  unsigned long timer = millis();
  loopTime = timer - timerOld;
  timerOld = timer;
}

#endif
