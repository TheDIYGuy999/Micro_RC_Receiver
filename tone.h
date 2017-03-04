#ifndef tone_h
#define tone_h

#include "Arduino.h"

//
// =======================================================================================================
// TONE FUNCTION FOR STAR WARS R2-D2
// =======================================================================================================
//

int r2d2Tones[] = {
  3520, 3136, 2637, 2093, 2349, 3951, 2794, 4186
};

void R2D2_tell() {
  if (toneOut) {
  for (int notePlay = 0; notePlay < 8; notePlay++) {
      int noteRandom = random(7);
      tone(A2, r2d2Tones[noteRandom], 80); // Pin, frequency, duration
      delay(50);
      noTone(A2);
    }
  }
}

#endif
