#ifndef __INS_HH__
#define __INS_HH__
/********************************* TRICK HEADER *******************************
PURPOSE:
      (Describe the INS Module On Board, Error equations based on Zipfel, Figure 10.27, space stabilized INS with GPS updates)
LIBRARY DEPENDENCY:
      ((../src/rocket/Ins.cpp))
*******************************************************************************/
#include <functional>

#include "Newton.hh"
#include "Euler.hh"
#include "Environment.hh"
#include "Kinematics.hh"
#include "GPS_receiver.hh"
#include "Time_management.hh"

#include "sensor/gyro/gyro.hh"
#include "sensor/gyro/gyro_ideal.hh"
#include "sensor/gyro/gyro_rocket6g.hh"

#include "sensor/accel/accelerometer.hh"
#include "sensor/accel/accelerometer_ideal.hh"
#include "sensor/accel/accelerometer_rocket6g.hh"

class GPS_Receiver;
class Newton;
class Kinematics;
class _Euler_;
class Environment;
class time_management;

class INS {
    TRICK_INTERFACE(INS);
    public:
        template<class Archive>
        void serialize(Archive & ar, const unsigned int version){
            ar & _EVBI;
            ar & _EVBID;
            ar & _ESBI;
            ar & _ESBID;
            ar & _RICI;
            ar & _RICID;
            ar & _TBIC;
            ar & _TEIC;
            ar & _SBIIC;
            ar & _VBIIC;
            ar & _SBEEC;
            ar & _VBEEC;
            ar & _WBICI;
            ar & _EGRAVI;
            ar & loncx;
            ar & latcx;
            ar & altc;
            ar & _VBECD;
            ar & _TDCI;
            ar & dbic;
            ar & dvbec;
            ar & alphacx;
            ar & betacx;
            ar & thtvdcx;
            ar & psivdcx;
            ar & alppcx;
            ar & phipcx;
            ar & phibdcx;
            ar & thtbdcx;
            ar & psibdcx;
        }

        INS(time_management &tim);
        INS(const INS& other);

        INS& operator=(const INS& other);

        void initialize();

        void update(double int_step);

        void set_ideal();
        void set_non_ideal(double frax_algnmnt);

        /* Input File */

        std::function<arma::vec3()> grab_computed_WBIB;
        std::function<arma::vec3()> grab_error_of_computed_WBIB;

        std::function<arma::vec3()> grab_computed_FSPB;
        std::function<arma::vec3()> grab_error_of_computed_FSPB;

        std::function<arma::vec3()>   grab_SBII;
        std::function<arma::vec3()>   grab_VBII;
        std::function<double()>       grab_dbi;
        std::function<arma::mat33()>  grab_TBI;
        std::function<int()>          grab_gps_update;
        std::function<arma::vec3()>   grab_SXH;
        std::function<arma::vec3()>   grab_VXH;
        std::function<void()>         clear_gps_flag;

        double get_dvbec();
        double get_alphacx();
        double get_betacx();
        double get_phibdcx();
        double get_thtbdcx();
        double get_psibdcx();
        double get_thtvdcx();

        arma::vec3 get_SBIIC();
        arma::vec3 get_VBIIC();
        arma::vec3 get_WBICI();
        arma::vec3 get_EGRAVI();
        arma::mat33 get_TBIC();
        arma::vec3 get_SBEEC();
        arma::vec3 get_VBEEC();
        arma::mat33 get_TEIC();

        /* Internal Initializers */
        void default_data();

    private:

        time_management *time;
        /* Internal Getter */
        arma::mat build_WEII();



        /* Internal Propagator / Calculators */
        bool GPS_update();

        arma::vec3 calculate_INS_derived_postion(arma::vec3 SBII);
        arma::vec3 calculate_INS_derived_velocity(arma::vec3 VBII);
        arma::vec3 calculate_INS_derived_bodyrate(arma::mat33 TBIC, arma::vec3 WBICB);

        arma::mat33 calculate_INS_derived_TBI(arma::mat33 TBI);
        arma::mat33 calculate_INS_derived_TEI();
        arma::vec3 calculate_gravity_error(double dbi);
        double calculate_INS_derived_dvbe();

        double calculate_INS_derived_alpha(arma::vec3 VBECB);
        double calculate_INS_derived_beta(arma::vec3 VBECB);

        double calculate_INS_derived_alpp(arma::vec3 VBECB);
        double calculate_INS_derived_phip(arma::vec3 VBECB);

        double calculate_INS_derived_psivd(arma::vec3 VBECD);
        double calculate_INS_derived_thtvd(arma::vec3 VBECD);

        double calculate_INS_derived_euler_angles(arma::mat33 TBD);



        /* Internal Calculators */

        /* Sensors */

        /* Constants */
        arma::mat WEII;        /* *o  (r/s)    Earth's angular velocity (skew-sym) */
        double   _WEII[3][3];  /* *o  (r/s)    Earth's angular velocity (skew-sym) */

        /* Propagative Stats */
        arma::vec EVBI;        /* *o  (m/s)   INS vel error */
        double   _EVBI[3];     /* *o  (m/s)   INS vel error */

        arma::vec EVBID;       /* *o  (m/s)   INS vel error derivative */
        double   _EVBID[3];    /* *o  (m/s)   INS vel error derivative */

        arma::vec ESBI;        /* *o  (m)     INS pos error */
        double   _ESBI[3];     /* *o  (m)     INS pos error */

        arma::vec ESBID;       /* *o  (m)     INS pos error derivative */
        double   _ESBID[3];    /* *o  (m)     INS pos error derivative */

        arma::vec RICI;        /* *o  (r)     INS tilt error derivative */
        double   _RICI[3];     /* *o  (r)     INS tilt error */

        arma::vec RICID;       /* *o  (r)     INS tilt error derivative */
        double   _RICID[3];    /* *o  (r)     INS tilt error derivative */

        /* Generating Outputs */
        arma::mat TBIC;        /* *o  (--)    Computed T.M. of body wrt inertia coordinate */
        double _TBIC[3][3];    /* *o  (--)    Computed T.M. of body wrt inertia coordinate */

        arma::vec SBIIC;       /* *o  (m)     Computed pos of body wrt inertia reference point*/
        double   _SBIIC[3];    /* *o  (m)     Computed pos of body wrt inertia reference point*/

        arma::vec VBIIC;       /* *o  (m/s)   Computed body vel in inertia coor */
        double   _VBIIC[3];    /* *o  (m/s)   Computed body vel in inertia coor */

        arma::vec SBEEC;       /* *o   (m)    Computed body position in ECEF */
        double _SBEEC[3];      /* *o   (m)    Computed body position in ECEF */

        arma::vec VBEEC;       /* *o   (m)    Computed body velocity in ECEF */
        double _VBEEC[3];      /* *o   (m)    Computed body velocity in ECEF */

        arma::vec WBICI;       /* *o  (r/s)   Computed inertial body rate in inert coordinate */
        double   _WBICI[3];    /* *o  (r/s)   Computed inertial body rate in inert coordinate */

        arma::vec EGRAVI;      /* *o  (--)    error by gravity */
        double   _EGRAVI[3];   /* *o  (--)    error by gravity */

        double loncx;       /* *io  (d)     INS derived longitude */
        double latcx;       /* *io  (d)     INS derived latitude */
        double altc;        /* *io  (m)     INS derived altitude */

        arma::vec VBECD;       /* *o  (m/s)   Geodetic velocity */
        double   _VBECD[3];    /* *o  (m/s)   Geodetic velocity */

        arma::mat TDCI;        /* *o  (--)    Comp T.M. of geodetic wrt inertial */
        double   _TDCI[3][3];  /* *o  (--)    Comp T.M. of geodetic wrt inertial */

        arma::mat TEIC;         /* *o  (--)   T.M. of inertia to ECEF */
        double _TEIC[3][3];     /* *o  (--)   T.M. of inertia to ECEF */

        double dbic;        /* *io  (m)     INS computed vehicle distance from Earth center */
        double dvbec;       /* *io  (m/s)   Computed body speed wrt earth */

        double alphacx;     /* *io  (d)     INS computed angle of attack */
        double betacx;      /* *io  (d)     INS computed sideslip angle */

        double thtvdcx;     /* *io  (d)     INS computed vertical flight path angle */
        double psivdcx;     /* *io  (d)     INS computed heading angle */

        double alppcx;      /* *io  (d)     INS computed total angle of attack */
        double phipcx;      /* *io  (d)     INS computed aero roll angle */

        double phibdcx;     /* *io  (d)     INS computed geodetic Euler roll angle */
        double thtbdcx;     /* *io  (d)     INS computed geodetic Euler pitch angle */
        double psibdcx;     /* *io  (d)     INS computed geodetic Euler yaw angle */

        /* Non-propagating Diagnostic Variables */
        /* These can be deleted, but keep to remain trackable in trick simulator */

        double ins_pos_err; /* *io  (m)     INS absolute postion error */
        double ins_vel_err; /* *io  (m/s)   INS absolute velocity error */
        double ins_tilt_err; /* *o  (r)     INS absolute tilt error */
};

#endif  // __INS_HH__
