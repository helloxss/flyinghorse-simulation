#ifndef __RCS_HH__
#define __RCS_HH__
/********************************* TRICK HEADER *******************************
PURPOSE:
      (Describe the RCS Module On Board)
LIBRARY DEPENDENCY:
      ((../src/rocket/Rcs.cpp))
PROGRAMMERS:
      (((Chung-Fan Yang) () () () ))
*******************************************************************************/

#include "Newton.hh"
#include "Ins.hh"
#include "Tvc.hh"
#include "Guidance.hh"
class Guidance;

class RCS {
    TRICK_INTERFACE(RCS);
    public:
        RCS(INS &i, Guidance &guia, Propulsion &plp);
        RCS(const RCS& other);

        RCS& operator=(const RCS& other);

        void initialize();

        void actuate();
        void rcs_schmitt_thrust();
        int  rcs_schmitt(double input_new, double input, double dead_zone, double hysteresis);

        enum RCS_TYPE {
            NO_RCS = 0,
            ON_OFF_RCS = 2
        };

        enum RCS_MODE {
            NO_CONTROL = 0,
            ALL_GEODETIC_EULUR_ANGLE_CONTROL = 1,
            THRUST_VECTOR_DIRECTION_AND_ROLL_ANGLE_CONTROL = 2,
            INCIDENCE_AND_ROLL_ANGLE_CONTROL = 3,
            GEODETIC_YAW_ANGLE_CONTROL = 4
        };

        Matrix get_FMRCS();
        Matrix get_FARCS();

        // XXX: backward capability
        int get_mrcs_moment() { return rcs_type * 10 + rcs_mode;  };
        int get_mrcs_force() { return 0; };

        void enable_rcs();
        void disable_rcs();

        void set_mode(enum RCS_MODE);

        enum RCS_MODE get_rcs_mode();

        /* Input File */

        void set_rcs_tau(double);
        void set_roll_mom_max(double);
        void set_pitch_mom_max(double);
        void set_yaw_mom_max(double);
        void set_thtbdcomx(double);
        void set_psibdcomx(double);
        void set_dead_zone(double);
        void set_hysteresis(double);
        void set_rcs_thrust(double);
        void set_rocket_r(double);
        void set_rcs_pos(double);
    private:
        /* Internal Getter */

        /* Internal Initializers */
        void default_data();

        /* Internal Propagator / Calculators */

        /* Internal Calculators */

        /* Routing references */
        INS        * ins;
        Guidance   * guidance;
        Propulsion * propulsion;

        /* Constants */
        enum RCS_TYPE rcs_type;  /* *o  (--)   Attitude control, see RCS_TYPE */
        enum RCS_MODE rcs_mode;  /* *o  (--)   Attitude control, see RCS_MODE */

        /* Propagative Stats */

        /* Generating Outputs */

        /* Non-propagating Diagnostic Variables */
        /* These can be deleted, but keep to remain trackable in trick simulator */

        double  fmrcs[3];        /* *io  (N*m)  Moment generated by the RCS thrusters */
        double  farcs[3];        /* *io  (N)    Force generated by the RCS side thrusters */

        double  rcs_tau;         /* *io  (s)    Slope of the switching function */
        double  roll_mom_max;    /* *io  (N*m)  RCS rolling moment max value */
        double  pitch_mom_max;   /* *io  (N*m)  RCS pitching moment max value */
        double  yaw_mom_max;     /* *io  (N*m)  RCS yawing moment max value */
        double  thtbdcomx;       /* *io  (d)    Pitch angle command */
        double  psibdcomx;       /* *io  (d)    Yaw angle command */
        double  dead_zone;       /* *io  (d)    Dead zone of Schmitt trigger */
        double  hysteresis;      /* *io  (d)    Hysteresis of Schmitt trigger */
        double  rcs_thrust;      /* *io  (N)    rcs thrust */
        double  rocket_r;        /* *io  (m)    rocket's radius */
        double  rcs_pos;         /* *io  (m)    rcs thruster's postion from nose */

        double  rcs_zeta;        /* *io  (--)   Damping of closed-loop rop RCS */
        double  rcs_freq;        /* *io  (r/s)  Natural freq. of closed-loop prop RCS */
        double  roll_save;       /* *io  (d)    Saving roll angle command */
        double  pitch_save;      /* *io  (d)    Saving pitch angle command */
        double  yaw_save;        /* *io  (d)    Saving yaw angle command */
        double  phibdcomx;       /* *io  (d)    Roll angle command */
        double  e_roll;          /* *io  (--)   Roll error signal */
        double  e_pitch;         /* *io  (--)   Pitch error signal */
        double  e_yaw;           /* *io  (--)   Yaw error signal */
        int     o_roll;          /* *io  (--)   Roll output of Schmitt trigger */
        int     o_pitch;         /* *io  (--)   Pitch output of Schmitt trigger */
        int     o_yaw;            /* *io  (--)   Yaw output of Schmitt trigger */
        int     roll_count;      /* *io  (--)   Roll thruster on/off */
        int     pitch_count;     /* *io  (--)   Pitch thruster on/off */
        int     yaw_count;       /* *io  (--)   Yaw thruster on/off */
        double  acc_gain;        /* *io  (N*s2/m) Acceleration gain of side thrusters */
        double  side_force_max;  /* *io  (N)    Maximum side force of thruster */
        double  e_right;         /* *io  (N)    Side force error signal, right */
        double  e_down;          /* *io  (N)    Down force error signal, down */
        int     o_right;         /* *io  (--)   Right output of Schmitt trigger */
        int     o_down;          /* *io  (--)   Down output of Schmitt trigger */
        double  right_save;      /* *io  (N)    Saving right force command */
        double  down_save;       /* *io  (N)    Saving down force command */
        int     right_count;     /* *io  (--)   Right force thruster on/off */
        int     down_count;      /* *io  (--)   Down forece thruster on/off */
        double  factdead_zone;   /* *io  (--)   Factor to mod 'dead_zone' dead_zone*(1+factdead_zone) */
};

#endif  // __RCS_HH__
