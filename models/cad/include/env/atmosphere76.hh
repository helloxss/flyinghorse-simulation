#ifndef __Atmosphere76_HH__
#define __Atmosphere76_HH__
/********************************* TRICK HEADER *******************************
PURPOSE:
      (US 1976 Standard Atmosphere (public domain))
LIBRARY DEPENDENCY:
      ((../../src/env/atmosphere76.cpp))
*******************************************************************************/
#include <cstdio>

#include "env/atmosphere.hh"

namespace cad {
class Atmosphere76 : public Atmosphere {
 public:
    Atmosphere76();

    virtual ~Atmosphere76();

    virtual void set_altitude(double altitude_in_meter);

 private:
    void update_values();
};
}  // namespace cad

#endif  // __Atmosphere76__
