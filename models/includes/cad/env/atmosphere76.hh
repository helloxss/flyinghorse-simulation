#ifndef __Atmosphere76_HH__
#define __Atmosphere76_HH__
/********************************* TRICK HEADER *******************************
PURPOSE:
      (US 1976 Standard Atmosphere (public domain))
LIBRARY DEPENDENCY:
      ((../../../src/cad/env/atmosphere76.cpp))
*******************************************************************************/

#include "cad/env/atmosphere.hh"

namespace cad {
    class Atmosphere76 : public Atmosphere
    {
        public:
            Atmosphere76();

            virtual ~Atmosphere76();

            virtual void set_altitude(double altitude_in_meter);

        private:
            int update_values();
    };
}

#endif//__Atmosphere76__
