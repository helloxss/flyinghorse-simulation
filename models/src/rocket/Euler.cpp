#include "rocket/Euler.hh"
#include "trick_utils/math/include/quat_macros.h"
#include "trick_utils/math/include/matrix_macros.h"
#include "trick_utils/math/include/vector_macros.h"
#include "trick_utils/math/include/trick_math_proto.h"
#include "sim_services/include/simtime.h"

_Euler_::_Euler_(Kinematics& kine, Propulsion& prop, Forces& forc)
    :   kinematics(&kine), propulsion(&prop), forces(&forc),
        VECTOR_INIT(WEII, 3),
        VECTOR_INIT(WBII, 3),
        VECTOR_INIT(WBIB, 3),
        VECTOR_INIT(WBIBD, 3),
        VECTOR_INIT(WBEB, 3)
{
    this->default_data();
}

_Euler_::_Euler_(const _Euler_& other)
    :   kinematics(other.kinematics), propulsion(other.propulsion), forces(other.forces),
        VECTOR_INIT(WEII, 3),
        VECTOR_INIT(WBII, 3),
        VECTOR_INIT(WBIB, 3),
        VECTOR_INIT(WBIBD, 3),
        VECTOR_INIT(WBEB, 3)
{
    this->default_data();

    /* Propagative Stats */
    this->WBIB = other.WBIB;
    this->WBIBD = other.WBIBD;

    /* Generating Outputs */
    this->WBII = other.WBII;
    this->WBEB = other.WBEB;
}

_Euler_& _Euler_::operator=(const _Euler_& other){
    if(&other == this)
        return *this;

    this->kinematics = other.kinematics;
    this->propulsion = other.propulsion;
    this->forces = other.forces;

    /* Propagative Stats */
    this->WBIB = other.WBIB;
    this->WBIBD = other.WBIBD;

    /* Generating Outputs */
    this->WBII = other.WBII;
    this->WBEB = other.WBEB;

    return *this;
}

void _Euler_::load_angular_velocity(double ppx, double qqx, double rrx){
    arma::mat33 TBI = kinematics->get_TBI_();

    //body rate wrt Earth frame in body coordinates
    this->WBEB = {ppx * RAD, qqx * RAD, rrx * RAD};
    this->WBIB = this->WBEB + TBI * this->WEII;
}

void _Euler_::initialize()
{
}

void _Euler_::default_data(){
    this->WEII.zeros();
    this->WEII(2) = WEII3;
}

void _Euler_::propagate(double int_step)
{
    arma::vec3 FMB(forces->get_FMB().get_pbody());

    // XXX: Trans
    arma::mat33 IBBB = (propulsion->get_IBBB().get_pbody());
    IBBB = trans(IBBB);

    arma::mat33 TBI = kinematics->get_TBI_();

    propagate_WBIB(int_step, FMB, IBBB);

    //angular velocity wrt inertial frame in inertial coordinates
    this->WBII = calculate_WBII(TBI);

    //angular velocity wrt Earth in body coordinates
    this->WBEB = calculate_WBEB(TBI);
}

void _Euler_::propagate_WBIB(double int_step, arma::vec3 FMB, arma::mat33 IBBB){
    //integrating the angular velocity acc wrt the inertial frame in body coord
    // Using Armadillo solve for higher accuracy, otherwise will faile the 1ppm test
    arma::vec3 WACC_NEXT = arma::solve(IBBB, (FMB - skew_sym(this->WBIB) * IBBB * this->WBIB));
    this->WBIB = integrate(WACC_NEXT, WBIBD, WBIB, int_step);
    this->WBIBD = WACC_NEXT;
}

arma::vec3 _Euler_::calculate_WBII(arma::mat33 TBI){
    return trans(TBI) * this->WBIB;
}

arma::vec3 _Euler_::calculate_WBEB(arma::mat33 TBI){
    return this->WBIB - TBI * this->WEII;
}

void _Euler_::update_diagnostic_attributes(double int_step){
    this->ppx = get_ppx();
    this->qqx = get_qqx();
    this->rrx = get_rrx();
}

double _Euler_::get_ppx() { return this->WBEB(0) * DEG; }

double _Euler_::get_qqx() { return this->WBEB(1) * DEG; }

double _Euler_::get_rrx() { return this->WBEB(2) * DEG; }

arma::vec3 _Euler_::get_WBII_() { return this->WBII; }

arma::vec3 _Euler_::get_WBIB_() { return this->WBIB; }

Matrix _Euler_::get_WBII()
{
    Matrix WBII(_WBII);
    return WBII;
}

Matrix _Euler_::get_WBIB()
{
    Matrix WBIB(_WBIB);
    return WBIB;
}
