#ifndef __INS_HH__
#define __INS_HH__
/********************************* TRICK HEADER *******************************
PURPOSE:
      (Describe the INS Module On Board)
LIBRARY DEPENDENCY:
      ((../src/rocket/Ins.cpp))
PROGRAMMERS:
      (((Chung-Fan Yang) () () () ))
*******************************************************************************/

#include "Newton.hh"
#include "Euler.hh"
#include "Environment.hh"
#include "Kinematics.hh"
#include "GPS_receiver.hh"

#include "sensor/gyro/gyro.hh"
#include "sensor/accel/accelerometer.hh"

class GPS_Receiver;
class Newton;
class Kinematics;
class _Euler_;
class Environment;

class INS {
    TRICK_INTERFACE(INS);
    public:
        INS(Newton &ntn, _Euler_ &elr, Environment &env, Kinematics &kins, GPS_Receiver &gps);
        INS(const INS& other);

        INS& operator=(const INS& other);

        void initialize();

        void update(double int_step);

        void set_gyro(sensor::Gyro &gyro);
        void set_accelerometer(sensor::Accelerometer &accel);

        sensor::Gyro& get_gyro();
        sensor::Accelerometer& get_accelerometer();

        void set_ideal();
        void set_non_ideal(double frax_algnmnt);

        /* Input File */
        enum INS_TYPE {
            INS_NOT_SET = -1,
            IDEAL_INS = 0,
            NON_IDEAL_INS = 1
        };

        double get_dvbec();
        double get_qqcx();
        double get_rrcx();
        double get_ppcx();
        double get_alphacx();
        double get_betacx();
        double get_phibdcx();
        double get_thtbdcx();
        double get_psibdcx();

        Matrix get_SBIIC();
        Matrix get_VBIIC();
        Matrix get_WBICI();
        Matrix get_EGRAVI();

        Matrix get_TBIC();

    private:
        /* Internal Getter */

        /* Internal Initializers */
        void default_data();

        /* Internal Propagator / Calculators */
        arma::vec3 calculate_gravity_error(double dbi);

        /* Internal Calculators */

        /* Routing references */
        Newton       * newton;
        _Euler_      * euler;
        Environment  * environment;
        Kinematics   * kinematics;
        GPS_Receiver * gpsr;

        /* Sensors */
        sensor::Gyro * gyro;
        sensor::Accelerometer * accel;

        /* Constants */

        /* State */
        enum INS_TYPE ins_mode;  /* *o  (--)    INS mode. see INS_TYPE */

        /* Propagative Stats */

        /* Generating Outputs */

        /* Non-propagating Diagnostic Variables */
        /* These can be deleted, but keep to remain trackable in trick simulator */
        double dvbec;       /* *io  (m/s)   Computed body speed wrt earth */
        double qqcx;        /* *io  (d/s)   INS computed pitch rate */
        double rrcx;        /* *io  (d/s)   INS computed yaw rate */

        arma::mat TBIC;         /* *o  (--)    Computed T.M. of body wrt earth coordinate */
        double _TBIC[3][3];     /* *o  (--)    Computed T.M. of body wrt earth coordinate */

        arma::vec SBIIC;       /* *o  (m)     Computed pos of body wrt earth reference point*/
        double   _SBIIC[3];    /* *o  (m)     Computed pos of body wrt earth reference point*/

        arma::vec VBIIC;       /* *o  (m/s)   Computed body vel in earth coor */
        double   _VBIIC[3];    /* *o  (m/s)   Computed body vel in earth coor */

        arma::vec WBICI;       /* *o  (r/s)   Computed inertial body rate in inert coordinate */
        double   _WBICI[3];    /* *o  (r/s)   Computed inertial body rate in inert coordinate */

        double ppcx;        /* *io  (d/s)   INS computed roll rate */
        double alphacx;     /* *io  (d)     INS computed angle of attack */
        double betacx;      /* *io  (d)     INS computed sideslip angle */
        double phibdcx;     /* *io  (d)     INS computed geodetic Euler roll angle */
        double thtbdcx;     /* *io  (d)     INS computed geodetic Euler pitch angle */
        double psibdcx;     /* *io  (d)     INS computed geodetic Euler yaw angle */

        /* grav */
        arma::vec EGRAVI;      /* *o   (--)    error by gravity */
        double   _EGRAVI[3];   /* *o   (--)    error by gravity */

        arma::vec RICI;
        double   _RICI[3];

        arma::vec EVBI;        /* *o  (m/s)   INS vel error */
        double   _EVBI[3];     /* *o  (m/s)   INS vel error */
        arma::vec EVBID;       /* *o  (m/s)   INS vel error derivative */
        double   _EVBID[3];    /* *o  (m/s)   INS vel error derivative */

        arma::vec ESBI;        /* *o  (m)     INS pos error */
        double   _ESBI[3];     /* *o  (m)     INS pos error */
        arma::vec ESBID;       /* *o  (m)     INS pos error derivative */
        double   _ESBID[3];    /* *o  (m)     INS pos error derivative */

        arma::vec VBECD;       /* *o  (m/s)   Geodetic velocity */
        double   _VBECD[3];    /* *o  (m/s)   Geodetic velocity */

        arma::mat TDCI;        /* *o  (--)    Comp T.M. of geodetic wrt inertial */
        double   _TDCI[3][3];  /* *o  (--)    Comp T.M. of geodetic wrt inertial */

        double loncx;       /* *io  (d)     INS derived longitude */
        double latcx;       /* *io  (d)     INS derived latitude */
        double altc;        /* *io  (m)     INS derived altitude */
        double thtvdcx;     /* *io  (d)     INS computed vertical flight path angle */
        double psivdcx;     /* *io  (d)     INS computed heading angle */
        double dbic;        /* *io  (m)     INS computed vehicle distance from Earth center */
        double alppcx;      /* *io  (d)     INS computed total angle of attack */
        double phipcx;      /* *io  (d)     INS computed aero roll angle */
        double ins_pos_err; /* *io  (m)     INS absolute postion error */
        double ins_vel_err; /* *io  (m/s)   INS absolute velocity error */
};

#endif  // __INS_HH__