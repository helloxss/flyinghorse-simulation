#include <fstream>
#include <cstring>
#include <cmath>
#include <stdlib.h>
#include <assert.h>
#include <iomanip>

#include <armadillo>

#include "math/utility.hh"
#include "math/matrix/utility.hh"

#include "cad/utility.hh"
#include "aux/global_constants.hh"

///////////////////////////////////////////////////////////////////////////////
// Returns great circle distance between two point on a spherical Earth
// Reference: Bate et al. "Fundamentals of Astrodynamics", Dover 1971, p. 310
// Return output
//    distancex = great circle distance - km
// Parameter input
//    lon1 = longitude of first point - rad
//    lat1 = latitude -of first point  rad
//    lon2 = longitude of second point - rad
//    lat2 = latitude -of second point  rad
//
// 030414 Created from FORTRAN by Peter H Zipfel
///////////////////////////////////////////////////////////////////////////////
double cad::distance(const double &lon1,
                const double &lat1,
                const double &lon2,
                const double &lat2)
{
    double dum =
        sin(lat2) * sin(lat1) + cos(lat2) * cos(lat1) * cos(lon2 - lon1);
    if (fabs(dum) > 1.)
        dum = 1. * sign(dum);
    double distancex = REARTH * acos(dum) * 1.e-3;

    return distancex;
}

//////////////////////////////////////////////////////////////////////////////
// Calculates geodetic longitude, latitude, and altitude from inertial
// displacement vector
// using the WGS 84 reference ellipsoid
// Reference: Britting,K.R."Inertial Navigation Systems Analysis", Wiley. 1971
//
// Parameter output
//    lon = geodetic longitude - rad
//    lat = geodetic latitude - rad
//    alt = altitude above ellipsoid - m
// Parameter input
//    lat = geodetic latitude - rad
//    SBII(3x1) = Inertial position - m
//
// 030414 Created from FORTRAN by Peter H Zipfel
// 170121 Create Armadillo Version by soncyang
///////////////////////////////////////////////////////////////////////////////
void cad::geo84_in(double &lon,
              double &lat,
              double &alt,
              arma::vec3 SBII,
              const double &time)
{
    int count(0);
    double lat0(0);
    double alamda(0);

    // initializing geodetic latitude using geocentric latitude
    double dbi = norm(SBII);
    double latg = asin(SBII(2, 0) / dbi);
    lat = latg;

    // iterating to calculate geodetic latitude and altitude
    do {
        lat0 = lat;
        double r0 =
            SMAJOR_AXIS *
            (1. - FLATTENING * (1. - cos(2. * lat0)) / 2. +
             5. * pow(FLATTENING, 2) * (1. - cos(4. * lat0)) / 16.);  // eq 4-21
        alt = dbi - r0;
        double dd = FLATTENING * sin(2. * lat0) *
                    (1. - FLATTENING / 2. - alt / r0);  // eq 4-15
        lat = latg + dd;
        count++;
        assert(count <= 100 &&
               " *** Stop: Geodetic latitude does not "
               "converge,'cad_geo84_in()' *** ");

    } while (fabs(lat - lat0) > SMALL);

    // longitude
    double sbii1 = SBII(0, 0);
    double sbii2 = SBII(1, 0);
    double dum4 = asin(sbii2 / sqrt(sbii1 * sbii1 + sbii2 * sbii2));
    // Resolving the multi-valued arcsin function
    if ((sbii1 >= 0.0) && (sbii2 >= 0.0))
        alamda = dum4;  // quadrant I
    if ((sbii1 < 0.0) && (sbii2 >= 0.0))
        alamda = (180. * RAD) - dum4;  // quadrant II
    if ((sbii1 < 0.0) && (sbii2 < 0.0))
        alamda = (180. * RAD) - dum4;  // quadrant III
    if ((sbii1 > 0.0) && (sbii2 < 0.0))
        alamda = (360. * RAD) + dum4;  // quadrant IV
    lon = alamda - WEII3 * time - GW_CLONG;
    if ((lon) > (180. * RAD))
        lon = -((360. * RAD) - lon);  // east positive, west negative
}

//////////////////////////////////////////////////////////////////////////////
// Returns geodetic velocity vector information from inertial postion and
// velocity
// using the WGS 84 reference ellipsoid
//
// Calls utilities
//    geo84_in(...), tdi84(...)
// Parameter output
//    dvbe = geodetic velocity - m/s
//    psivdx = geodetic heading angle - deg
//    thtvdx = geodetic flight path angle - deg
// Parameter input
//    SBII(3x1) = Inertial position - m
//    VBII(3x1) = Inertial velocity - m
//
// 040710 created by Peter H Zipfel
///////////////////////////////////////////////////////////////////////////////
void cad::geo84vel_in(double &dvbe,
                 double &psivdx,
                 double &thtvdx,
                 arma::vec3 SBII,
                 arma::vec3 VBII,
                 const double &time)
{
    double lon(0);
    double lat(0);
    double alt(0);
    // geodetic longitude, latitude and altitude
    geo84_in(lon, lat, alt, SBII, time);
    arma::mat33 TDI = tdi84(lon, lat, alt, time);

    // Earth's angular velocity skew-symmetric matrix (3x3)
    arma::mat33 WEII(arma::fill::zeros);
    WEII(0, 1) = -WEII3;
    WEII(1, 0) = WEII3;

    // geographic velocity in geodetic axes VBED(3x1) and flight path angles
    arma::vec3 VBED = TDI * (VBII - WEII * SBII);
    arma::vec3 POLAR = pol_from_cart(VBED);

    dvbe   = POLAR[0];
    psivdx = DEG * POLAR[1];
    thtvdx = DEG * POLAR[2];
}

////////////////////////////////////////////////////////////////////////////////
// Returns geocentric lon, lat, alt from inertial displacement vector
// for spherical Earth
//
// Parameter output
//    lonc = geocentric longitude - rad
//    latc = geocentric latitude - rad
//    altc = geocentric latitude - m
// Parameter input
//    SBII = Inertial position - m
//    time = simulation time - sec
//
// 010628 Created by Peter H Zipfel
// 030416 Modified for SBII (not SBIE) input, PZi
////////////////////////////////////////////////////////////////////////////////
void cad::geoc_in(double &lonc,
             double &latc,
             double &altc,
             arma::vec3 SBII,
             const double &time)
{
    double lon_cel(0);
    double sbii1 = SBII(0);
    double sbii2 = SBII(1);
    double sbii3 = SBII(2);
    arma::vec3 RESULT;

    // latitude
    double dbi = sqrt(sbii1 * sbii1 + sbii2 * sbii2 + sbii3 * sbii3);
    latc = asin((sbii3) / dbi);

    // altitude
    altc = dbi - REARTH;

    // longitude
    double dum4 = asin(sbii2 / sqrt(sbii1 * sbii1 + sbii2 * sbii2));
    // Resolving the multi-valued arcsin function
    if ((sbii1 >= 0.0) && (sbii2 >= 0.0))
        lon_cel = dum4;  // quadrant I
    if ((sbii1 < 0.0) && (sbii2 >= 0.0))
        lon_cel = (180. * RAD) - dum4;  // quadrant II
    if ((sbii1 < 0.0) && (sbii2 < 0.0))
        lon_cel = (180. * RAD) - dum4;  // quadrant III
    if ((sbii1 > 0.0) && (sbii2 < 0.0))
        lon_cel = (360. * RAD) + dum4;  // quadrant IV
    lonc = lon_cel - WEII3 * time - GW_CLONG;
    if ((lonc) > (180. * RAD))
        lonc = -((360. * RAD) - lonc);  // east positive, west negative
}

////////////////////////////////////////////////////////////////////////////////
// Returns lon, lat, alt from displacement vector in Earth coord for spherical
// earth
//
// Return output
//    RETURN[0]=lon
//    RETURN[1]=lat
//    RETURN[2]=alt
// Paramter input
//    SBIE = displacement of vehicle wrt Earth center in Earth coordinates
//
// 010628 Created by Peter H Zipfel
////////////////////////////////////////////////////////////////////////////////
arma::vec3 cad::geoc_ine(arma::vec3 SBIE)
{
    double dum4(0);
    double alamda(0);
    double x(0), y(0), z(0);
    double dbi(0);
    double alt(0);
    double lat(0);
    double lon(0);

    // downloading inertial components
    x = SBIE(0);
    y = SBIE(1);
    z = SBIE(2);
    // Latitude
    //dbi = sqrt(x * x + y * y + z * z);
    dbi = norm(SBIE);
    lat = asin((z) / dbi);

    // Altitude
    alt = dbi - REARTH;

    // Longitude
    dum4 = asin(y / sqrt(x * x + y * y));

    // Resolving the multi-valued arcsin function
    if ((x >= 0.0) && (y >= 0.0)) {
        alamda = dum4;  // quadrant I
    }
    if ((x < 0.0) && (y >= 0.0)) {
        alamda = (180.0 * RAD) - dum4;  // quadrant II
    }
    if ((x < 0.0) && (y < 0.0)) {
        alamda = (180.0 * RAD) - dum4;  // quadrant III
    }
    if ((x >= 0.0) && (y < 0.0)) {
        alamda = (360.0 * RAD) + dum4;  // quadrant IV
    }

    lon = alamda;
    if ((lon) > (180.0 * RAD)) {
        lon = -((360.0 * RAD) - lon);  // east positive, west negative
    }

    return arma::vec3({lon, lat, alt});
}

///////////////////////////////////////////////////////////////////////////////
// Earth gravitational acceleration, using the WGS 84 ellipsoid
// Ref: Chatfield, A.B.,"Fundamentals of High Accuracy Inertial
// Navigation",p.10, Prog.Astro and Aeronautics, Vol 174, AIAA, 1997.
//
// Return output
//    GRAVG(3x1) = gravitational acceleration in geocentric coord - m/s^2
// Parameter input
//    SBII = inertial displacement vector - m
//    time = simulation time - sec
//
// 030417 Created from FORTRAN by Peter H Zipfel
///////////////////////////////////////////////////////////////////////////////
arma::vec3 cad::grav84(arma::vec3 SBII, const double &time)
{
    double lonc(0), latc(0), altc(0);

    geoc_in(lonc, latc, altc, SBII, time);
    double dbi = norm(SBII);
    double dum1 = GM / (dbi * dbi);
    double dum2 = 3 * sqrt(5.);
    double dum3 = pow((SMAJOR_AXIS / dbi), 2);
    double gravg1 = -dum1 * dum2 * C20 * dum3 * sin(latc) * cos(latc);
    double gravg2 = 0;
    double gravg3 =
        dum1 * (1. + dum2 / 2. * C20 * dum3 * (3. * pow(sin(latc), 2) - 1.));

    return arma::vec3({gravg1, gravg2, gravg3});
}

///////////////////////////////////////////////////////////////////////////////
// Returns the inertial displacement vector from longitude, latitude and
// altitude
// using the WGS 84 reference ellipsoid
// Reference: Britting,K.R."Inertial Navigation Systems Analysis"
// pp.45-49, Wiley, 1971
//
// Return output
//     SBII(3x1) = Inertial vehicle position - m
// Parameter input
//     lon = geodetic longitude - rad
//     lat = geodetic latitude - rad
//     alt = altitude above ellipsoid - m
//     time = simulation time - sec
//
// 030411 Created from FORTRAN by Peter H Zipfel
// 170121 Create Armadillo Version by soncyang
///////////////////////////////////////////////////////////////////////////////
arma::vec3 cad::in_geo84(const double lon,
                         const double lat,
                         const double alt,
                         const double &time)
{
    arma::vec3 SBID;

    // deflection of the normal, dd, and length of earth's radius to ellipse
    // surface, R0
    double r0 = SMAJOR_AXIS * (1. - FLATTENING * (1. - cos(2. * lat)) / 2. +
                               5. * pow(FLATTENING, 2) * (1. - cos(4. * lat)) /
                                   16.);  // eq 4-21
    double dd = FLATTENING * sin(2. * lat) *
                (1. - FLATTENING / 2. - alt / r0);  // eq 4-15

    // vehicle's displacement vector from earth's center SBID(3x1) in geodetic
    // coord.
    double dbi = r0 + alt;
    SBID(0, 0) = -dbi * sin(dd);
    SBID(1, 0) = 0.;
    SBID(2, 0) = -dbi * cos(dd);

    // celestial longitude of vehicle at simulation 'time'
    double lon_cel = GW_CLONG + WEII3 * time + lon;

    // vehicle's displacement vector in inertial coord.
    // SBII(3x1)=TID(3x3)xSBID(3x3)
    double slat = sin(lat);
    double clat = cos(lat);
    double slon = sin(lon_cel);
    double clon = cos(lon_cel);
    double sbid1 = SBID(0, 0);
    double sbid3 = SBID(2, 0);
    double sbii1 = -slat * clon * sbid1 - clat * clon * sbid3;
    double sbii2 = -slat * slon * sbid1 - clat * slon * sbid3;
    double sbii3 = clat * sbid1 - slat * sbid3;

    return arma::vec3({sbii1, sbii2, sbii3});
}

///////////////////////////////////////////////////////////////////////////////
// Returns the inertial displacement vector from geocentric longitude, latitude
// and altitude
// for spherical Earth
//
// Return output
//    SBII = position of vehicle wrt center of Earth, in inertial coord
// Argument input
//    lon = geographic longitude - rad
//    lat = geocentric latitude - rad
//    alt = altitude above spherical Earth = m
//
// 010405 Created by Peter H Zipfel
///////////////////////////////////////////////////////////////////////////////
arma::vec3 cad::in_geoc(const double &lon,
                   const double &lat,
                   const double &alt,
                   const double &time)
{
    arma::vec3 VEC;

    double dbi = alt + REARTH;
    double cel_lon = lon + WEII3 * time + GW_CLONG;
    double clat = cos(lat);
    double slat = sin(lat);
    double clon = cos(cel_lon);
    double slon = sin(cel_lon);

    VEC(0) = dbi * clat * clon;
    VEC(1) = dbi * clat * slon;
    VEC(2) = dbi * slat;

    return VEC;
}

///////////////////////////////////////////////////////////////////////////////
// Calculates inertial displacement and velocity vectors from orbital elements
// Reference: Bate et al. "Fundamentals of Astrodynamics", Dover 1971, p.71
//
// Return output
//    parabola_flag = 0 ok
//                  = 1 not suitable (divide by zero), because parabolic
//                    trajectory
// Parameter output
//    SBII = Inertial position - m
//    SBII = Inertial velocity - m/s
// Parameter input
//    semi = semi-major axis of orbital ellipsoid - m
//    ecc = eccentricity of elliptical orbit - ND
//    inclx = inclination of orbital wrt equatorial plane - deg
//    lon_anodex = celestial longitude of the ascending node - deg
//    arg_perix = argument of periapsis (ascending node to periapsis) - deg
//    true_anomx = true anomaly (periapsis to satellite) - deg
//
// 040510 Created by Peter H Zipfel
///////////////////////////////////////////////////////////////////////////////
int cad::in_orb(arma::vec3 &SBII,
           arma::vec3 &VBII,
           const double &semi,
           const double &ecc,
           const double &inclx,
           const double &lon_anodex,
           const double &arg_perix,
           const double &true_anomx)
{
    // local variable
    int parabola_flag(0);
    arma::vec3 SBIP;
    arma::vec3 VBIP;
    arma::mat33 TIP;

    // semi-latus rectum from semi-major axis and eccentricity
    double pp = semi * (1 - ecc * ecc);

    // angles
    double c_true_anom = cos(true_anomx * RAD);
    double s_true_anom = sin(true_anomx * RAD);

    // inertial distance in perifocal coordinates
    double dbi = pp / (1 + ecc * c_true_anom);

    // inertial position vector
    SBIP[0] = dbi * c_true_anom;
    SBIP[1] = dbi * s_true_anom;
    SBIP[2] = 0;

    // pypass calculation if parabola
    if (pp == 0)
        parabola_flag = 1;
    else {
        // inertial velocity
        double dum = sqrt(GM / pp);
        VBIP[0] = -dum * s_true_anom;
        VBIP[1] = dum * (ecc + c_true_anom);
        VBIP[2] = 0;
    }

    // transforming to inertial coordinates
    TIP = tip(inclx * RAD, lon_anodex * RAD, arg_perix * RAD);
    SBII = TIP * SBIP;
    VBII = TIP * VBIP;

    return parabola_flag;
}

////////////////////////////////////////////////////////////////////////////////
// Projects initial state through 'tgo' to final state along a Keplerian
// trajectory
// Based on Ray Morth, unpublished utility
//
// Return output
//    kepler_flag = 0: good Kepler projection;
//                = 1: bad (# of iterations>20, or neg. sqrt), no new proj cal,
//                  use prev value;  - ND
// Parameter output
//    SPII = projected inertial position after tgo - m
//    VPII = projected inertial velocity after tgo - m/s
// Parameter input
//    SBII = current inertial position - m
//    VBII = current inertial velocity - m/s
//    tgo = time-to-go to projected point - sec
//
// 040319 Created from FORTRAN by Peter H Zipfel
///////////////////////////////////////////////////////////////////////////////
int cad::kepler(arma::vec3 &SPII,
           arma::vec3 &VPII,
           arma::vec3 SBII,
           arma::vec3 VBII,
           const double &tgo)
{
    // local variables
    double sde(0);
    double cde(0);
    int kepler_flag(0);

    double sqrt_GM = sqrt(GM);
    double ro = norm(SBII);
    double vo = norm(VBII);
    double rvo = dot(SBII, VBII);
    double a1 = vo * vo / GM;
    double sa = ro / (2 - ro * a1);
    if (sa < 0) {
        // return without re-calculating SPII, VPII
        kepler_flag = 1;
        return kepler_flag;
    }
    double smua = sqrt_GM * sqrt(sa);
    double mdot = smua / (sa * sa);

    // calculating 'de'iteratively
    double dm = mdot * tgo;
    double de = dm;  // initialize eccentricity
    double a11 = rvo / smua;
    double a21 = (sa - ro) / sa;
    int count20 = 0;
    double adm(0);
    do {
        cde = 1 - cos(de);
        sde = sin(de);
        double dmn = de + a11 * cde - a21 * sde;
        double dmerr = dm - dmn;

        adm = fabs(dmerr) / mdot;
        double dmde = 1 + a11 * sde - a21 * (1 - cde);
        de = de + dmerr / dmde;
        count20++;
        if (count20 > 20) {
            // return without re-calculating SPII, VPII
            kepler_flag = 1;
            return kepler_flag;
        }
    } while (adm > SMALL);

    // projected position
    double fk = (ro - sa * cde) / ro;
    double gk = (dm + sde - de) / mdot;
    SPII = SBII * fk + VBII * gk;

    // projected velocity
    double rp = norm(SPII);
    double fdk = -smua * sde / ro;
    double gdk = rp - sa * cde;
    VPII = SBII * (fdk / rp) + VBII * (gdk / rp);

    return kepler_flag;
}

////////////////////////////////////////////////////////////////////////////////
// Projects initial state through 'tgo' to final state along a Keplerian
// trajectory
// Based on: Bate, Mueller, White, "Fundamentals of Astrodynamics", Dover 1971
//
// Return output
//    iter_flag=0: # of iterations < 20; =1: # of iterations > 20;  - ND
// Parameter output
//    SPII = projected inertial position after tgo - m
//    VPII = projected inertial velocity after tgo - m/s
// Parameter input
//    SBII = current inertial position - m
//    VBII = current inertial velocity - m/s
//    tgo = time-to-go to projected point - sec
//
// 040318 Created from ASTRO_KEP by Peter H Zipfel
///////////////////////////////////////////////////////////////////////////////
int cad::kepler1(arma::vec3 &SPII,
            arma::vec3 &VPII,
            arma::vec3 SBII,
            arma::vec3 VBII,
            const double &tgo)
{
    double c(0);
    double s(0);
    double z(0);
    double dt(0);
    int iter_flag(0);

    double ro = norm(SBII);
    double vo = norm(VBII);
    double al = (2 * GM / ro - vo * vo) / GM;
    double en = -GM * al / 2;  // specific mechanical energy - J/kg
    arma::vec3 AM = skew_sym(SBII) * VBII;
    double h = norm(AM);  // angular momentum - m^2/s
    double de2 = 1 + 2 * en * h * h / (GM * GM);
    if (de2 < 0)
        de2 = 0;
    double dum = dot(SBII, VBII);
    double sqrt_GM = sqrt(GM);

    // initial guaess of x
    double x = 0;
    int count20 = 0;

    // calculating x using newton iteration
    do {
        count20++;
        z = x * x * al;
        kepler1_ucs(c, s, z);
        dt =
            (x * x * x * s + dum * x * x * c / sqrt_GM + ro * x * (1 - z * s)) /
            sqrt_GM;
        double dtx =
            (x * x * c + dum * x * (1 - z * s) / sqrt_GM + ro * (1 - z * c)) /
            sqrt_GM;
        x = x + (tgo - dt) / dtx;
    } while (fabs((tgo - dt) / tgo) > SMALL);

    // projected inertial position
    double f = 1 - x * x * c / ro;
    double g = tgo - x * x * s / sqrt_GM;
    SPII = SBII * f + VBII * g;

    // projecting inertial velocity
    double rx = norm(SPII);
    double fd = sqrt_GM * x * (z * s - 1) / (ro * rx);
    double gd = 1 - x * x * c / rx;
    VPII = SBII * fd + VBII * gd;

    // diagnostic: bad iteration if count20 > 20
    if (count20 > 20)
        iter_flag = 1;
    return iter_flag;
}

////////////////////////////////////////////////////////////////////////////////
// Calculates utility functions c(z) and s(z) for kepler(...)
// Reference: Bate, Mueller, White, "Fundamentals of Astrodynamics", Dover 1971,
//            p.196
//
// Paramter output
//    c = c(z) utility function
//    s = s(z) utility function
// Parameter input
//    z = z-variable
//
// 040318 Created from ASTRO_UCS by Peter H Zipfel
///////////////////////////////////////////////////////////////////////////////
void cad::kepler1_ucs(double &c, double &s, const double &z)
{
    double sd(0);
    double cd(0);

    if (z > 0.1) {
        c = (1 - cos(sqrt(z))) / z;
        s = (sqrt(z) - sin(sqrt(z))) / sqrt(z * z * z);
        sd = (c - 3 * s) / (2 * z);
        cd = (1 - z * s - 2 * c) / (2 * z);
    }
    if (z < -0.1) {
        c = (1 - cosh(sqrt(-z))) / z;
        s = (sinh(sqrt(-z)) - sqrt(-z)) / sqrt(-z * z * z);
        sd = (c - 3 * s) / (2 * z);
        cd = (1 - z * s - 2 * c) / (2 * z);
    }
    if (fabs(z) <= 0.1) {
        double dc = 2;
        c = 1 / dc;
        double dcd = -24;
        cd = 1 / dcd;
        double ds = 6;
        s = 1 / ds;
        double dsd = -120;
        sd = 1 / dsd;

        for (int k = 1; k < 7; k++) {
            double z_pow_k = pow(-z, k);
            int n = 2 * k + 1;
            dc = dc * n * (n + 1);
            c = c + z_pow_k / dc;
            dcd = dcd * (n + 2) * (n + 3);
            cd = cd - (k + 1) * z_pow_k / dcd;
            ds = ds * (n + 1) * (n + 2);
            s = s + z_pow_k / ds;
            dsd = dsd * (n + 3) * (n + 4);
            sd = sd - (k + 1) * z_pow_k / dsd;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Calculates the orbital elements from inertial displacement and velocity
// Reference: Bate et al. "Fundamentals of Astrodynamics", Dover 1971, p.58
//
// Return output
//    cadorbin_flag = 0 ok
//                    1 'true_anomx' not calculated, because of circular orbit
//                    2 'semi' not calculated, because parabolic orbit
//                    3 'lon_anodex' not calculated, because equatorial orbit
//                   13 'arg_perix' not calculated, because equatorialand/or
//                      circular orbit
// Parameter output:
//    semi = semi-major axis of orbital ellipsoid - m
//    ecc = eccentricity of elliptical orbit - ND
//    inclx = inclination of orbital wrt equatorial plane - deg
//    lon_anodex = celestial longitude of the ascending node - deg
//    arg_perix = argument of periapsis (ascending node to periapsis) - deg
//    true_anomx = true anomaly (periapsis to satellite) - deg
//
// Parameter input:
//    SBII = Inertial position - m
//    VBII = Inertial velocity - m/s
//
// 040510 Created by Peter H Zipfel
// 170121 Create Armadillo Version by soncyang
///////////////////////////////////////////////////////////////////////////////
int cad::orb_in(double &semi,
                   double &ecc,
                   double &inclx,
                   double &lon_anodex,
                   double &arg_perix,
                   double &true_anomx,
                   arma::vec3 &SBII,
                   arma::vec3 &VBII)
{
    // local variable
    int cadorbin_flag(0);
    arma::vec3 NODE_I;
    double lon_anode(0);
    double arg_peri(0);
    double true_anom(0);

    // angular momentum vector of orbit
    arma::mat ANGL_MOM_I = skew_sym(SBII) * VBII;
    double angl_mom = norm(ANGL_MOM_I);

    // vector of the ascending node (undefined if inclx=0)
    NODE_I(0) = -ANGL_MOM_I(1);
    NODE_I(1) = ANGL_MOM_I(0);
    double node = norm(NODE_I);

    // orbit eccentricity vector and magnitude
    double dbi = norm(SBII);
    double dvbi = norm(VBII);
    arma::vec3 EI;
    EI = (SBII * (dvbi * dvbi - GM / dbi) - VBII * dot(SBII, VBII)) * (1 / GM);
    ecc = norm(EI);

    // semi latus rectum
    double pp = angl_mom * angl_mom / GM;

    // semi-major axis of orbit
    if (pp == 1)
        cadorbin_flag = 2;
    else
        semi = pp / (1 - ecc * ecc);

    // orbit inclination
    double arg = ANGL_MOM_I(2) * (1 / angl_mom);
    if (fabs(arg) > 1)
        arg = 1;
    inclx = acos(arg) * DEG;

    // bypass calculations if equatorial orbit
    if (node < SMALL) {
        cadorbin_flag = 3;
    } else {
        // longitude of the ascending node
        arg = NODE_I(0) / node;
        if (fabs(arg) > 1)
            arg = 1;
        lon_anode = acos(arg);
    }

    // bypass calculations if circular and/or equatorial orbit
    if (ecc < SMALL || node < SMALL)
        cadorbin_flag = 13;
    else {
        // argument of periapsis
        arg = dot(NODE_I, EI) * (1 / (node * ecc));
        if (fabs(arg) > 1)
            arg = 1;
        arg_peri = acos(arg);
    }

    // bypass calculations if circular orbit
    if (ecc < SMALL)
        cadorbin_flag = 1;
    else {
        // true anomaly
        arg = dot(SBII, EI) * (1 / (dbi * ecc));
        if (fabs(arg) > 1)
            arg = 1;
        true_anom = acos(arg);
    }

    // quadrant resolution
    double quadrant = dot(SBII, VBII);
    if (quadrant >= 0)
        true_anomx = true_anom * DEG;
    else
        true_anomx = (2 * PI - true_anom) * DEG;

    if (EI(2) >= 0)
        arg_perix = arg_peri * DEG;
    else
        arg_perix = (2 * PI - arg_peri) * DEG;

    if (NODE_I(1) > 0)
        lon_anodex = lon_anode * DEG;
    else
        lon_anodex = (2 * PI - lon_anode) * DEG;

    return cadorbin_flag;
}

///////////////////////////////////////////////////////////////////////////////
// Returns the T.M. of geodetic wrt inertial coordinates
// using the WGS 84 reference ellipsoid
//
// Return output
//    TDI(3x3) = T.M.of geosetic wrt inertial coord - ND
// Parameter input
//    lon = geodetic longitude - rad
//    lat = geodetic latitude - rad
//    alt = altitude above ellipsoid - m
//
// 030424 Created by Peter H Zipfel
// 170121 Create Armadillo Version by soncyang
///////////////////////////////////////////////////////////////////////////////
arma::mat33 cad::tdi84(const double &lon,
                           const double &lat,
                           const double &alt,
                           const double &time)
{
    arma::mat33 TDI;

    // celestial longitude of vehicle at simulation 'time'
    double lon_cel = GW_CLONG + WEII3 * time + lon;

    // T.M. of geodetic coord wrt inertial coord., TDI(3x3)
    double tdi13 = cos(lat);
    double tdi33 = -sin(lat);
    double tdi22 = cos(lon_cel);
    double tdi21 = -sin(lon_cel);
    TDI(0, 0) = tdi33 * tdi22;
    TDI(0, 1) = -tdi33 * tdi21;
    TDI(0, 2) = tdi13;
    TDI(1, 0) = tdi21;
    TDI(1, 1) = tdi22;
    TDI(1, 2) = 0;
    TDI(2, 0) = -tdi13 * tdi22;
    TDI(2, 1) = tdi13 * tdi21;
    TDI(2, 2) = tdi33;

    return TDI;
}

////////////////////////////////////////////////////////////////////////////////
// Returns the T.M. of earth wrt inertial coordinates
//
// Return output
//    TEI = T.M. of Earthy wrt inertial coordinates
//
// Argument input
//    time = time since start of simulation - s
//
// 010628 Created by Peter H Zipfel
////////////////////////////////////////////////////////////////////////////////
arma::mat33 cad::tei(const double &time)
{
    arma::mat33 TEI(arma::fill::eye);

    double xi = WEII3 * time + GW_CLONG;
    double sxi = sin(xi);
    double cxi = cos(xi);

    TEI(0, 0) = cxi;
    TEI(0, 1) = sxi;
    TEI(1, 0) = -sxi;
    TEI(1, 1) = cxi;

    return TEI;
}

///////////////////////////////////////////////////////////////////////////////
// Returns the T.M. of geographic wrt earth coordinates, TGE
// spherical Earth only
//
// Parameter input
//    lon = geographic longitude - rad
//    lat = geographic latitude - rad
//
// 010628 Created by Peter H Zipfel
///////////////////////////////////////////////////////////////////////////////
arma::mat33 cad::tge(const double &lon, const double &lat)
{
    arma::mat33 TGE(arma::fill::zeros);

    double clon = cos(lon);
    double slon = sin(lon);
    double clat = cos(lat);
    double slat = sin(lat);

    TGE(0, 0) = (-slat * clon);
    TGE(0, 1) = (-slat * slon);
    TGE(0, 2) = clat;
    TGE(1, 0) = -slon;
    TGE(1, 1) = clon;
    TGE(1, 2) = 0.0;
    TGE(2, 0) = (-clat * clon);
    TGE(2, 1) = (-clat * slon);
    TGE(2, 2) = -slat;

    return TGE;
}

////////////////////////////////////////////////////////////////////////////////
// Returns the T.M. of geographic (geocentric) wrt inertial
// using the WGS 84 reference ellipsoid
// Reference: Britting,K.R."Inertial Navigation Systems Analysis",
// pp.45-49, Wiley, 1971
//
// Return output
//    TGI(3x3) = T.M.of geographic wrt inertial coord - ND
// Parameter input
//    lon = geodetic longitude - rad
//    lat = geodetic latitude - rad
//    alt = altitude above ellipsoid - m
//
// 030414 Created from FORTRAN by Peter H Zipfel
// 170121 Create Armadillo Version by soncyang
///////////////////////////////////////////////////////////////////////////////
arma::mat33 cad::tgi84(const double &lon,
                  const double &lat,
                  const double &alt,
                  const double &time)
{
    arma::mat33 TDI = tdi84(lon, lat, alt, time);
    arma::mat33 TGD;

    // deflection of the normal, dd, and length of earth's radius to ellipse
    // surface, R0
    double r0 = SMAJOR_AXIS * (1. - FLATTENING * (1. - cos(2. * lat)) / 2. +
                               5. * pow(FLATTENING, 2) * (1. - cos(4. * lat)) /
                                   16.);  // eq 4-21
    double dd = FLATTENING * sin(2. * lat) *
                (1. - FLATTENING / 2. - alt / r0);  // eq 4-15

    // T.M. of geographic (geocentric) wrt geodetic coord., TGD(3x3)
    TGD(0, 0) = cos(dd);
    TGD(2, 2) = cos(dd);
    TGD(1, 1) = 1;
    TGD(2, 0) = sin(dd);
    TGD(0, 2) = -sin(dd);

    // T.M. of geographic (geocentric) wrt inertial coord., TGI(3x3)
    arma::mat33 TGI = TGD * TDI;

    return TGI;
}

///////////////////////////////////////////////////////////////////////////////
// Returns the transformation matrix of inertial wrt perifocal coordinates
//
// Return output
//    TIP = TM of inertial wrt perifocal
// Parameter input
//    incl = inclination of orbital wrt equatorial plane - rad
//    lon_anode = celestial longitude of the ascending node - rad
//    arg_peri = argument of periapsis (ascending node to periapsis) - rad
//
// 040510 Created by Peter H Zipfel
///////////////////////////////////////////////////////////////////////////////
arma::mat33 cad::tip(const double &incl,
                const double &lon_anode,
                const double &arg_peri)
{
    // local variable
    arma::mat33 TIP(arma::fill::zeros);

    double clon_anode = cos(lon_anode);
    double carg_peri = cos(arg_peri);
    double cincl = cos(incl);
    double slon_anode = sin(lon_anode);
    double sarg_peri = sin(arg_peri);
    double sincl = sin(incl);

    TIP(0, 0) = clon_anode * carg_peri - slon_anode * sarg_peri * cincl;
    TIP(0, 1) = -clon_anode * sarg_peri - slon_anode * carg_peri * cincl;
    TIP(0, 2) = slon_anode * sincl;
    TIP(1, 0) = slon_anode * carg_peri + clon_anode * sarg_peri * cincl;
    TIP(1, 1) = -slon_anode * sarg_peri + clon_anode * carg_peri * cincl;
    TIP(1, 2) = -clon_anode * sincl;
    TIP(2, 0) = sarg_peri * sincl;
    TIP(2, 1) = carg_peri * sincl;
    TIP(2, 2) = cincl;

    return TIP;
}
