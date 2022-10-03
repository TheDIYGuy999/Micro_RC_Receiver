#ifndef steeringCurves_h
#define steeringCurves_h

#include "Arduino.h"

//
// =======================================================================================================
// NONLINEAR ARRAYS FOR THROTTLE STEERING OVERLAY
// =======================================================================================================
//

// In order to optimize the steering behaviour for your vehicle, just change the steering curves in the arrays below

// This array is intended for the "Semi caterpillar" mode. The inner wheel can max. slow down to 60% of the
// outer wheels RPM
float curveSemi[][2] = {  // see excel sheet!
  {0, 60} // {input value, output value}
  , {25, 70}
  , {50, 80}
  , {75, 90}
  , {100, 100}
};

// This array is intended for the "Caterpillar" mode. The inner wheel can spin backwars  up to 100% of the
// outer wheels RPM. That allows for turning the vehicle "on place"
float curveFull[][2] = {
  {0, -100} // {input value, output value}
  , {25, 9}
  , {50, 61}
  , {75, 87}
  , {100, 100}
};

// This array is intended for the "Differential Thrust" mode. The inner motor can max. slow down to 20% of the
// outer motors RPM
float curveThrust[][2] = {  // see excel sheet!
  {0, 20} // {input value, output value}
  , {25, 40}
  , {50, 60}
  , {75, 80}
  , {100, 100}
};

//
// =======================================================================================================
// ARRAY FOR EXPONENTIAL THROTTLE COMPENSATION
// =======================================================================================================
//

float curveExponentialThrottle[][2] = {
    {0, 0} // {input value, output value}
    , {1000, 1000}
    , {1100, 1150}
    , {1200, 1290}
    , {1300, 1410}
    , {1400, 1470}
    
    , {1500, 1500} // Neutral
    
    , {1600, 1530}
    , {1700, 1590}
    , {1800, 1710}
    , {1900, 1850}
    , {2000, 2000}
    , {3000, 3000} // overload range
};

//
// =======================================================================================================
// ARRAY INTERPOLATION
// =======================================================================================================
//

// Credit: http://interface.khm.de/index.php/lab/interfaces-advanced/nonlinear-mapping/

int reMap(float pts[][2], int input) {
  int rr;
  float bb, mm;

  for (int nn = 0; nn < 13; nn++) { // was 5
    if (input >= pts[nn][0] && input <= pts[nn + 1][0]) {
      mm = ( pts[nn][1] - pts[nn + 1][1] ) / ( pts[nn][0] - pts[nn + 1][0] );
      mm = mm * (input - pts[nn][0]);
      mm = mm +  pts[nn][1];
      rr = mm;
    }
  }
  return (rr);
}

#endif
