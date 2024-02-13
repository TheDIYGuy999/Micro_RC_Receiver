#ifndef helper_h
#define helper_h

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
