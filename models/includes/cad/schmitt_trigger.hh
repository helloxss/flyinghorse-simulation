#ifndef __SCHTRI_HH__
#define __SCHTRI_HH__
/********************************* TRICK HEADER *******************************
PURPOSE:
      (A schimitt trigger implementation from rocket-sim)
LIBRARY DEPENDENCY:
      ((../../src/cad/schmitt_trigger.cpp))
*******************************************************************************/

#include "aux/aux.hh"

class Schmitt_Trigger {
    TRICK_INTERFACE(Schmitt_Trigger);
    public:
        Schmitt_Trigger(double dead_zone, double hysteresis);
        Schmitt_Trigger(const Schmitt_Trigger& other);

        Schmitt_Trigger& operator=(const Schmitt_Trigger& other);

        int trigger(double in);

    private:
        double  dead_zone;       /* *o  (--)    Dead zone of Schmitt trigger */
        double  hysteresis;      /* *o  (--)    Hysteresis of Schmitt trigger */

        double  saved_value;     /* *o  (--)    Saved Value */
};

#endif  // __SCHTRI_HH__
