#ifndef __ACCEL_IDEAL_HH__
#define __ACCEL_IDEAL_HH__
/********************************* TRICK HEADER *******************************
PURPOSE:
      (Ideal Accelerometer Implementation)
LIBRARY DEPENDENCY:
      ((../../../src/sensor/accel/accelerometer_ideal.cpp))
*******************************************************************************/

#include <armadillo>
#include <aux/aux.hh>

#include "sensor/accel/accelerometer.hh"
#include "rocket/Newton.hh"

namespace sensor {
    class AccelerometerIdeal : public Accelerometer
    {
        TRICK_INTERFACE(sensor__AccelerometerIdeal);

        public:
            char name[256];

            AccelerometerIdeal(Newton &newt);

            virtual ~AccelerometerIdeal() {};

            virtual void propagate_error(double int_step);

        private:
            Newton * newton;
    };
}

#endif//__ACCEL_IDEAL__