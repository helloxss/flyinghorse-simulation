#include "kinematics.hh"
#include "trick_utils/math/include/quat_macros.h"
#include "trick_utils/math/include/matrix_macros.h"
#include "trick_utils/math/include/vector_macros.h"
#include "trick_utils/math/include/trick_math_proto.h"
#include "sim_services/include/simtime.h"


void Kinematics::initialize(Newton *newt, Environment *env, _Euler_ *eul){
    newton = newt;
    environment = env;
    euler = eul;

    double lonx=newton->lonx;
    double latx=newton->latx;
    double alt=newton->alt;
    double dvbe=newton->dvbe;
    Matrix TDI(3,3);
    Matrix TBD(3,3);
    Matrix TBI(3,3);

    TBD=mat3tr(psibdx*RAD,thtbdx*RAD,phibdx*RAD);
    TDI=cad_tdi84(lonx*RAD,latx*RAD,alt,get_rettime());
    TBI=TBD*TDI;

    TBD.fill(tbd);
    TBI.fill(tbi);
}

void Kinematics::calculate(double int_step){

    Matrix UNIT(3,3); UNIT.identity();
    double cthtbd(0);
    double phip(0);
    Matrix TBD(3,3);

    double dvba=environment->dvba;
    double lonx=newton->lonx;
    double latx=newton->latx;
    double alt=newton->alt;

    Matrix WBIB(3,1);
    Matrix VBED(3,1);
    Matrix VAED(3,1);
    Matrix VBII(3,1);

    WBIB.build_vec3(euler->wbib);
    VBED.build_vec3(newton->vbed);
    VAED.build_vec3(environment->vaed);
    VBII.build_vec3(newton->IVel);

    Matrix TBI(3,3);
    Matrix TBID(3,3);

    TBI.build_mat33(tbi);
    TBID.build_mat33(tbid);

    //*integrating direction cosine matrix
    Matrix TBID_NEW=~WBIB.skew_sym()*TBI;
    TBI=integrate(TBID_NEW,TBID,TBI,int_step);
    TBID=TBID_NEW;

    //orthonormalizing TBI
    Matrix EE=UNIT-TBI*~TBI;
    TBI=TBI+EE*TBI*0.5;

    //TBI orthogonality check
    double e1=EE.get_loc(0,0);
    double e2=EE.get_loc(1,1);
    double e3=EE.get_loc(2,2);
    ortho_error=sqrt(e1*e1+e2*e2+e3*e3);

    //_Euler_ angles
    // XXX: Timing might make the position skew
    Matrix TDI = cad_tdi84(lonx*RAD,latx*RAD,alt,get_rettime());
    TBD=TBI*~TDI;
    double tbd13=TBD.get_loc(0,2);
    double tbd11=TBD.get_loc(0,0);
    double tbd33=TBD.get_loc(2,2);
    double tbd12=TBD.get_loc(0,1);
    double tbd23=TBD.get_loc(1,2);

    //*geodetic _Euler_ angles
    //pitch angle: 'thtbd'
    //note: when |tbd13| >= 1, thtbd = +- pi/2, but cos(thtbd) is
    //      forced to be a small positive number to prevent division by zero
    if(fabs(tbd13)<1){
        thtbd=asin(-tbd13);
        cthtbd=cos(thtbd);
    }
    else{
        thtbd=PI/2*sign(-tbd13);
        cthtbd=EPS;
    }
    //yaw angle: 'psibd'
    double cpsi=tbd11/cthtbd;
    if(fabs(cpsi)>1)
        cpsi=1*sign(cpsi);
    psibd=acos(cpsi)*sign(tbd12);

    //roll angle: 'phibdc'
    double cphi=tbd33/cthtbd;
    if(fabs(cphi)>1)
        cphi=1*sign(cphi);
    phibd=acos(cphi)*sign(tbd23);

    psibdx=DEG*psibd;
    thtbdx=DEG*thtbd;
    phibdx=DEG*phibd;

    //*incidence angles using wind vector VAED in geodetic coord
    Matrix VBAB=TBD*(VBED-VAED);
    double vbab1=VBAB.get_loc(0,0);
    double vbab2=VBAB.get_loc(1,0);
    double vbab3=VBAB.get_loc(2,0);
    double alpha=atan2(vbab3,vbab1);
    double beta=asin(vbab2/dvba);
    alphax=alpha*DEG;
    betax=beta*DEG;

    //incidence angles in load factor plane (aeroballistic)
    double dum=vbab1/dvba;

    if(fabs(dum)>1)
        dum=1*sign(dum);
    double alpp=acos(dum);
    if(vbab2==0&&vbab3==0)
        phip=0.;
    //note: if vbeb2 is <EPS the value phip is forced to be 0 or PI
    //      to prevent oscillations
    else if(fabs(vbab2)<EPS)
        if(vbab3>0) phip=0;
        if(vbab3<0) phip=PI;
    else
        phip=atan2(vbab2,vbab3);
    alppx=alpp*DEG;
    phipx=phip*DEG;

    //*diagnostic: calculating the inertial incidence angles
    Matrix VBIB=TBI*VBII;
    double vbib1=VBIB.get_loc(0,0);
    double vbib2=VBIB.get_loc(1,0);
    double vbib3=VBIB.get_loc(2,0);
    double alphai=atan2(vbib3,vbib1);
    double dvbi=VBIB.absolute();
    double betai=asin(vbib2/dvbi);
    alphaix=alphai*DEG;
    betaix=betai*DEG;

    //update Matrix variables
    TBI.fill(tbi);
    TBID.fill(tbid);
    TBD.fill(tbd);
}