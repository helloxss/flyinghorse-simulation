#ifndef __aerodynamics_HH__
#define __aerodynamics_HH__
/********************************* TRICK HEADER *******************************
PURPOSE:
      (Describe the AERODYNAMICS Module On Board)
LIBRARY DEPENDENCY:
      ((../src/rocket/Aerodynamics.cpp))
PROGRAMMERS:
      (((Jun-Xu Lai) () () () ))
*******************************************************************************/

#include "cad/datadeck.hh"
#include "Euler.hh"
#include "Tvc.hh"
#include "Environment.hh"
#include "Propulsion.hh"

class TVC;
class _Euler_;
class Propulsion;
class Kinematics;
class Newton;

class AeroDynamics{
    TRICK_INTERFACE(AeroDynamics);

    public:
        template<class Archive>
        void serialize(Archive & ar, const unsigned int version){
            ar & kinematics;
            ar & environment;
            ar & propulsion;
            ar & euler;
            ar & newton;
            ar & tvc;

            ar & aerotable;

            ar & xcg_ref;
            ar & alplimx;
            ar & alimitx;
            ar & refa;
            ar & refd;
            ar & dyb;
            ar & dma;
            ar & dnb;
            ar & dnd;
            ar & dmq;
            ar & dnr;
            ar & dmde;
            ar & dndr;
            ar & gymax;
            ar & dla;
            ar & cy;
            ar & cll;
            ar & clm;
            ar & cln;
            ar & cx;
            ar & cz;

            ar & ca0;
            ar & caa;
            ar & cn0;
            ar & clm0;
            ar & clmq;
            ar & cla;
            ar & clde;
            ar & cyb;
            ar & cydr;
            ar & cllda;
            ar & cllp;
            ar & cma;
            ar & cmde;
            ar & cmq;
            ar & cnb;
            ar & cndr;
            ar & cnr;
            ar & stmarg_yaw;
            ar & stmarg_pitch;
            ar & dlde;
            ar & dydr;
            ar & dllp;
            ar & dllda;
            ar & realp1;
            ar & realp2;
            ar & wnp;
            ar & zetp;
            ar & rpreal;
            ar & realy1;
            ar & realy2;
            ar & wny;
            ar & zety;
            ar & ryreal;
            ar & gnavail;
            ar & gyavail;
            ar & gnmax;
        }

        AeroDynamics(Kinematics &kine,  Environment &env,
                     Propulsion &prop,  _Euler_     &eul,
                     Newton     &newt,  TVC         &t);
        AeroDynamics(const AeroDynamics& other);

        AeroDynamics& operator=(const AeroDynamics& other);

        void load_aerotable(char* filename);

        void initialize();
        void calculate_aero(double int_step);

        double get_dyb();
        double get_dma();
        double get_dnb();
        double get_dnd();
        double get_dmq();
        double get_dnr();
        double get_dmde();
        double get_dndr();
        double get_gymax();
        double get_dla();
        double get_refa();
        double get_refd();
        double get_cy();
        double get_cll();
        double get_clm();
        double get_cln();
        double get_cx();
        double get_cz();
        double get_dlde();

        /* Input File */
        void set_xcg_ref(double);     /* *io (m)      Reference cg location from nose - m*/
        void set_alplimx(double);     /* *io (d)      Alpha limiter for vehicle - deg*/
        void set_alimitx(double);     /* *io (--)     Structural  limiter for vehicle*/
        void set_refa(double);        /* *io (m2)     Reference area for aero coefficients - m^2*/
        void set_refd(double);        /* *io (m)      Reference length for aero coefficients - m*/

    private:
        void aerodynamics_der();

        Kinematics  * kinematics;
        Environment * environment;
        Propulsion  * propulsion;
        _Euler_     * euler;
        Newton      * newton;
        TVC         * tvc;

        Datadeck aerotable; /* ** (--) Aero Deck */

        double xcg_ref;     /* *io (m)      Reference cg location from nose - m*/
        double alplimx;     /* *io (d)      Alpha limiter for vehicle - deg*/
        double alimitx;     /* *io (--)     Structural  limiter for vehicle*/
        double refa;        /* *io (m2)     Reference area for aero coefficients - m^2*/
        double refd;        /* *io (m)      Reference length for aero coefficients - m*/
        double dyb;         /* *io (m/s2)   Side force derivative - m/s^2*/
        double dma;         /* *io (1/s2)   Pitch moment derivative - 1/s^2*/
        double dnb;         /* *io (1/s2)   Yawing moment derivative - 1/s^2*/
        double dnd;         /* *io (m/s2)   pitch contrl force derivative -m/s^2*/
        double dmq;         /* *io (1/s)    Pitch damping derivative - 1/s*/
        double dnr;         /* *io (1/s)    Yaw dampnig derivative - 1/s*/
        double dmde;        /* *io (1/s2)   Pitch control derivative - 1/s^2*/
        double dndr;        /* *io (1/s2)   Yaw control derivative - 1/s^2*/
        double gymax;       /* *io (--)     Max g permissable in yaw for vehicle - g's*/
        double dla;         /* *io (m/s2)   Lift slope derivative - m/s^2*/
        double cy;          /* *io (--)     Side force coefficient - ND*/
        double cll;         /* *io (--)     Rolling moment coefficient - ND*/
        double clm;         /* *io (--)     Pitching moment coefficient - ND*/
        double cln;         /* *io (--)     Yawing moment coefficient - ND*/
        double cx;          /* *io (--)     X-force coefficient - ND*/
        double cz;          /* *io (--)     Z-force coefficient - ND*/

        double ca0;         /* *io (--)     Axial force coeff(Mach) - ND*/
        double caa;         /* *io (--)     Delta axial force due to alpha(Mach) - ND*/
        double cn0;         /* *io (--)     Normal force coeff(Mach,alpha) - ND*/
        double clm0;        /* *io (--)     Pitch moment coeff(Mach,alpha) - ND*/
        double clmq;        /* *io (1/d)    Pitch dampning deriv(Mach) - 1/deg*/
        double cla;         /* *io (1/d)    Lift slope derivative(alpha,mach) - 1/deg*/
        double clde;        /* *io (1/d)    Lift force due to elevator (alpha.mach), - 1/deg*/
        double cyb;         /* *io (1/d)    Weather vane der wrt beta(alpha,mach) - 1/deg*/
        double cydr;        /* *io (1/d)    Side force due to rudder deriv(alpha,mach) - 1/deg*/
        double cllda;       /* *io (1/d)    Roll control effectiveness(alpha,mach), - 1/deg*/
        double cllp;        /* *io (1/r)    Roll damping deriv(alpha,mach) - 1/rad*/
        double cma;         /* *io (1/d)    Pitch moment due to alpha deriv(alpha,mach) -1/deg*/
        double cmde;        /* *io (1/d)    Pitch control effectiveness(alpha,mach), - 1/deg*/
        double cmq;         /* *io (1/r)    Pitch dampning deriv(alpha,mach) - 1/rad*/
        double cnb;         /* *io (1/d)    Yaw moment deriv(alpha,mach) - 1/deg*/
        double cndr;        /* *io (1/d)    Yaw moment due to rudder deriv(alpha,mach) - 1/deg*/
        double cnr;         /* *io (1/r)    Yaw damping deriv(alpha,mach) - 1/rad*/
        double stmarg_yaw;  /* *io (--)     Static margin yaw (+stable, -unstable) - caliber*/
        double stmarg_pitch;/* *io (--)     Static margin pitch (+stable, -unstable) - caliber*/
        double dlde;        /* *io (m/s2)   Lift elevator control derivative - m/s^2*/
        double dydr;        /* *io (m/s2)   Side force control derivative - m/s^2*/
        double dllp;        /* *io (1/s)    Roll damping derivative - 1/s*/
        double dllda;       /* *io (1/s2)   Roll control derivative - 1/s^2*/
        double realp1;      /* *io (r/s)    First real root of airframe pitch dyn  - rad/s*/
        double realp2;      /* *io (r/s)    Second real root of airframe pitch dyn - rad/s*/
        double wnp;         /* *io (r/s)    Natural frequency of airframe pitch dynamics - rad/s*/
        double zetp;        /* *io (--)     Damping of airframe pitch dynamics - NA*/
        double rpreal;      /* *io (r/s)    Real part or mean value (real roots) of pitch  - rad/s*/
        double realy1;      /* *io (r/s)    First real root of airframe yaw dynamics - rad/s*/
        double realy2;      /* *io (r/s)    Second real root of airframe yaw dynamics - rad/s*/
        double wny;         /* *io (r/s)    Natural frequency of airframe yaw dynamics - rad/s*/
        double zety;        /* *io (--)     Damping of airframe yaw dynamics - NA*/
        double ryreal;      /* *io (r/s)    Real part or mean value (real roots) of yaw - rad/s*/
        double gnavail;     /* *io (--)     G available in pitch for vehicle - g's*/
        double gyavail;     /* *io (--)     G available in yaw for vehicle - g's*/
        double gnmax;       /* *io (--)     Max g permissable in pitch for vehicle - g's*/
};


#endif  // __aerodynamics_HH__
