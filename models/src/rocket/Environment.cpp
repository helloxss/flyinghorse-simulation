#include "aux/utility_header.hh"

#include "rocket/Environment.hh"
#include "sim_services/include/simtime.h"

#include "cad/env/atmosphere.hh"
#include "cad/env/atmosphere76.hh"
#include "cad/env/atmosphere_nasa2002.hh"
#include "cad/env/atmosphere_weatherdeck.hh"

Environment::Environment(Newton &newt, AeroDynamics &aero, Kinematics &kine)
    :   newton(&newt), aerodynamics(&aero), kinematics(&kine),
        VECTOR_INIT(GRAVG, 3),
        VECTOR_INIT(VAED, 3),
        VECTOR_INIT(VAEDS, 3),
        VECTOR_INIT(VAEDSD, 3)
{
    this->default_data();

    atmosphere = NULL;
}

Environment::Environment(const Environment& other)
    :   newton(other.newton), aerodynamics(other.aerodynamics), kinematics(other.kinematics),
        VECTOR_INIT(GRAVG, 3),
        VECTOR_INIT(VAED, 3),
        VECTOR_INIT(VAEDS, 3),
        VECTOR_INIT(VAEDSD, 3)
{
    this->default_data();
}

Environment& Environment::operator=(const Environment& other) {
    if(&other == this)
        return *this;

    this->kinematics   = other.kinematics;
    this->newton       = other.newton;
    this->aerodynamics = other.aerodynamics;

    return *this;
}

Environment::~Environment() {
    if(atmosphere) delete atmosphere;
}

void Environment::initialize() {
    dvba = newton->get_dvbe();
}

void Environment::default_data() {
}

void Environment::atmosphere_use_public() {
    atmosphere = new cad::Atmosphere76();
}

void Environment::atmosphere_use_nasa() {
    atmosphere = new cad::Atmosphere_nasa2002();
}

void Environment::atmosphere_use_weather_deck(char* filename) {
    atmosphere = new cad::Atmosphere_weatherdeck(filename);
}

void Environment::set_no_wind(){
    mwind = NO_WIND;
}

void Environment::set_constant_wind(double dvae){
    mwind = CONSTANT_WIND;

    this->dvae = dvae;
}

void Environment::set_tabular_wind(){
    mwind = TABULAR_WIND;
}

void Environment::set_no_wind_turbulunce(){
    mturb = NO_TURBULENCE;
}

void Environment::set_wind_turbulunce(){
    mturb = DRYDEN_TURBULENCE;
}

void Environment::calculate_env(double int_step) {
    double dvw(0);

    arma::vec3 VBED = newton->get_VBED_();
    // XXX: arma matrix subsitute
    Matrix SBII = newton->get_IPos();
    double alt = newton->get_alt();

    this->GRAVG = arma::vec3(cad_grav84(SBII, get_rettime()).get_pbody());
    this->grav = norm(GRAVG);

    atmosphere->set_altitude(alt);

    rho    = atmosphere->get_density();
    press  = atmosphere->get_pressure();
    tempk  = atmosphere->get_temperature_in_kelvin();
    vsound = atmosphere->get_speed_of_sound();

    tempc = tempk - 273.16;

    //mach number
    vmach = fabs(dvba / vsound);

    //dynamic pressure
    pdynmc = 0.5 * rho * dvba * dvba;

    if(mwind == CONSTANT_WIND){

        dvw = dvae;

    }else if(mwind == TABULAR_WIND){

        dvw = atmosphere->get_speed_of_wind();
        psiwdx = atmosphere->get_direction_of_wind();

    }

    if(mwind != NO_WIND){
        //wind components in geodetic coordinates
        arma::vec3 VAED_RAW;
        VAED_RAW(0) = -dvw * cos(psiwdx * RAD);
        VAED_RAW(1) = -dvw * sin(psiwdx * RAD);
        VAED_RAW(2) = vaed3;

        //smoothing wind by filtering with time constant 'twind' sec
        arma::vec3 VAEDSD_NEW = (VAED_RAW - VAEDS) * (1 / twind);
        VAEDS = integrate(VAEDSD_NEW, VAEDSD, VAEDS, int_step);
        VAEDSD = VAEDSD_NEW;
        VAED = VAEDS;
    }

    //wind turbulence in normal - load plane
    if(mturb == 1){
        arma::vec3 VTAD = environment_dryden(dvba,int_step);
        VAED = VTAD+VAEDS;
    }

    //flight conditions
    arma::vec3 VBAD = VBED - VAED;
    dvba = norm(VBAD);

    //mach number
    vmach = fabs(dvba/vsound);
    //dynamic pressure
    pdynmc = 0.5 * rho * dvba * dvba;
}

arma::vec3 Environment::environment_dryden(double dvba,double int_step) {
    arma::vec3 VTAD;

    arma::mat TBD = kinematics->get_TBD_();
    double alppx = kinematics->get_alppx();
    double phipx = kinematics->get_phipx();

    double value1;
    do
        value1 = (double)rand() / RAND_MAX;
    while(value1 == 0);

    double value2 = (double)rand() / RAND_MAX;
    gauss_value = (1/sqrt(int_step)) * sqrt(2 * log(1/value1)) * cos(2 * PI * value2);

    //filter, converting white gaussian noise into a time sequence of Dryden
    // turbulence velocity variable 'tau'  (One - dimensional cross - velocity Dryden spectrum)
    //integrating first state variable
    double taux1d_new = taux2;
    taux1 = integrate(taux1d_new,taux1d,taux1,int_step);
    taux1d = taux1d_new;

    //integrating second state variable
    double vl = dvba/turb_length;
    double taux2d_new =  - vl * vl * taux1 - 2 * vl * taux2+vl * vl * gauss_value;
    taux2 = integrate(taux2d_new,taux2d,taux2,int_step);
    taux2d = taux2d_new;

    //computing Dryden 'tau' from the two filter states ('2 * PI' changed to 'PI' according to Pritchard)
    tau = turb_sigma * sqrt(1/(vl * PI)) * (taux1+sqrt(3.) * taux2/vl);

    //inserting the turbulence into the load factor plane (aeroballistic 1A - 3A plane)
    // and transforming into body coordinates VTAB = TBA * VTAA; VTAA = [0 0 tau]
    arma::vec3 VTAB;
    VTAB(0) = -tau * sin(alppx * RAD);
    VTAB(1) = tau * sin(phipx * RAD) * cos(alppx * RAD);
    VTAB(2) = tau * cos(phipx * RAD) * cos(alppx * RAD);

    //turbulence in geodetic coordinates
    VTAD = trans(TBD) * VTAB;

    return VTAD;
}

double Environment::get_rho() { return rho; }
double Environment::get_vmach() { return vmach; }
double Environment::get_pdynmc() { return pdynmc; }
double Environment::get_tempk() { return tempk; }
double Environment::get_dvba() { return dvba; }
double Environment::get_grav() { return grav; }
double Environment::get_press() { return press; }
arma::vec3 Environment::get_GRAVG_() { return GRAVG; }
arma::vec3 Environment::get_VAED_() { return VAED; }

Matrix Environment::get_GRAVG() {
    Matrix GRAVG(_GRAVG);
    return GRAVG;
}
Matrix Environment::get_VAED() {
    Matrix VAED(_VAED);
    return VAED;
}

