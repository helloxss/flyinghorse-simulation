#include "Rcs.hh"

void RCS::default_data(){
}

void RCS::initialize(INS *i, Guidance *guia, Propulsion *plp){
    ins = i;
    guidance = guia;
    propulsion = plp;
}

///////////////////////////////////////////////////////////////////////////////
// RCS thruster module
// Member function of class 'Hyper'
// Calls thrusters dynamic subroutine
//
// mrcs_moment = |rcs_type||rcs_mode|
//
// rcs_type = 0 no RCS thrusting
//          = 1 proportional moment thrusters
//          = 2 on-off moment thrusters (Schmitt trigger)
//
// rcs_mode = 0 no control
//          = 1 all geodetic Euler angle control
//          = 2 thrust vector direction and roll angle control
//          = 3 incidence and roll angle control
//          = 4 geodetic pitch angle control
//
// mrcs_force = 0 no side force control
//            = 1 proportional side thrusters
//            = 2 on-off side thrusters (Schmitt trigger)
//
// 040302 Created by Peter H Zipfel
///////////////////////////////////////////////////////////////////////////////

void RCS::actuate(){
    // local variables

    // local module-variables
    double e_roll(0);
    double e_pitch(0);
    double e_yaw(0);

    Matrix UTBC(guidance->utbc);

    //-------------------------------------------------------------------------
    // decoding rcs_mom flag
    int rcs_type = mrcs_moment / 10;
    int rcs_mode = (mrcs_moment % 10);
    /**********************************
    //proportional moment thrusters
    if(rcs_type==1){

        //gain calculations
        double rgain_roll=2*rcs_zeta*rcs_freq*IBBB.get_loc(0,0);
        double rgain_pitch=2*rcs_zeta*rcs_freq*IBBB.get_loc(1,1);
        double rgain_yaw=2*rcs_zeta*rcs_freq*IBBB.get_loc(2,2);
        double pgain=rcs_freq/(2*rcs_zeta);

        //roll angle control (always)
        e_roll=rgain_roll*(pgain*(phibdcomx-phibdcx)-ppcx);
        FMRCS[0]=rcs_prop(e_roll,roll_mom_max);

        //geodetic Euler angle control
        if(rcs_mode==1){
            e_pitch=rgain_pitch*(pgain*(thtbdcomx-thtbdcx)-qqcx);
            e_yaw=rgain_yaw*(pgain*(psibdcomx-psibdcx)-rrcx);
        }
        //vector directional control
        else if(rcs_mode==2){
            e_pitch=rgain_pitch*(pgain*(-UTBC[2])*DEG-qqcx);//DEG is used,
    because all angle var. are in deg
            e_yaw=rgain_yaw*(pgain*(UTBC[1])*DEG-rrcx);
        }
        //moments generated about the two transverse principle axes wrt the c.m
        FMRCS[1]=rcs_prop(e_pitch,pitch_mom_max);
        FMRCS[2]=rcs_prop(e_yaw,yaw_mom_max);
    }
    //Proportional side force thrusters
    if(mrcs_force==1){

        //acceleration control
        double ay=FSPCB[1];
        e_right=acc_gain*(aycomx*AGRAV-ay);
        FARCS[1]=rcs_prop(e_right,side_force_max);

        double az=FSPCB[2];
        e_down=acc_gain*(azcomx*AGRAV-az);
        FARCS[2]=rcs_prop(e_down,side_force_max);
    }
    *************************************************************************/
    // on-off moment thrusters (Schmitt trigger)
    if (rcs_type == 2) {
        // roll angle control (always)
        e_roll = phibdcomx - (rcs_tau * ins->ppcx + ins->phibdcx);
        // on/off output of Schmitt trigger


        // geodetic Euler angle control
        if (rcs_mode == 1) {
            e_pitch = thtbdcomx - (rcs_tau * ins->qqcx + ins->thtbdcx);
            e_yaw = psibdcomx - (rcs_tau * ins->rrcx + ins->psibdcx);
        }

        // incidence angle control
        if (rcs_mode == 3) {
            e_pitch = guidance->alphacomx - (rcs_tau * ins->qqcx + ins->alphacx);
            e_yaw = -guidance->betacomx - (rcs_tau * ins->rrcx - ins->betacx);
        }
        // geodetic yaw angle control
        if (rcs_mode == 4) {
            // e_yaw=psibdcomx-(rcs_tau*rrcx+psibdcx);
            e_pitch = thtbdcomx - (rcs_tau * ins->qqcx + ins->thtbdcx);
        }
        // vector directional control
        else if (rcs_mode == 2) {
            e_pitch = -rcs_tau * ins->qqcx - UTBC[2] * DEG;
            e_yaw = -rcs_tau * ins->rrcx + UTBC[1] * DEG;
        }



        // moments generated about the three principle axes wrt the c.m
        // FMRCS[0]=o_roll*roll_mom_max;
        // FMRCS[1]=o_pitch*pitch_mom_max;
        // FMRCS[2]=o_yaw*yaw_mom_max;
        // FMRCS[0]=o_roll*rcs_thrust*rocket_r;
        // FMRCS[1]=o_pitch*rcs_thrust*(xcg-1);
        // FMRCS[2]=o_yaw*rcs_thrust*(xcg-1);
    }
    /*************************************************
    //on-off side force thrusters (Schmitt trigger)
    if(mrcs_force==2){

        //acceleration control
        double ay=FSPCB[1];
        e_right=acc_gain*(aycomx*AGRAV-ay);

        int o_right_save=o_right;
        o_right=rcs_schmitt(e_right,right_save,dead_zone,hysteresis);
        right_save=e_right;
        if(o_right!=o_right_save)right_count++;

        double az=FSPCB[2];
        e_down=acc_gain*(azcomx*AGRAV-az);
        int o_down_save=o_down;
        o_down=rcs_schmitt(e_down,down_save,dead_zone,hysteresis);
        down_save=e_down;
        if(o_down!=o_down_save)down_count++;

        FARCS[0]=0;
        FARCS[1]=o_right*side_force_max;
        FARCS[2]=o_down*side_force_max;
    }
    ***************************************************/
}

///////////////////////////////////////////////////////////////////////////////
// Proportional thrusters with limiters
// Member function of class 'Hyper'
//
// parameter input
//    input = commanded input - Nm
//    limiter = max and min value of applied moment - Nm
// return output
//    output = moment about principle axis through c.m. - Nm
//
// 040309 Created by Peter H Zipfel
///////////////////////////////////////////////////////////////////////////////
/**************************************************
double Hyper::rcs_prop(double input,double limiter)
{
    //local variable
    double output(0);
    output=input;
    if(fabs(input)>limiter)
        output=limiter*sign(input);

    return output;
}
****************************************************/

///////////////////////////////////////////////////////////////////////////////
// On-off thrusters modeled by the Schmitt trigger
// Ref: Bryson,A.E.,"Control of Spacecraft and Aircraft",
//      Princeton University Press, 1994
//
// Switching mechanism with dead-zone and hysteresis, includes also pure
// hysteresis (dead_zone=0) pure dead-zone (hysteresis=0), or simple on/off
// relay
// (dead_zone=0,hysteresis=0)
// Member function of class 'Hyper'
//
// parameter input
//    input_new = new input value, state variable
//    input = previous input value, saved state variable
//    dead_zone = dead zone of trigger,
//                if zero -> pure hysteresis - units of input
//    hysteresis = hysteresis,
//                 if zero -> pure dead zone - units of input return output
//    =+/-1: trigger on; =0 trigger off
//
// 040308 Created by Peter H Zipfel
//////////////////////////////////////////////////////////////////////////////

void RCS::rcs_schmitt_thrust() {
    Matrix FMRCS(fmrcs);

    int o_roll_save = o_roll;
    o_roll = rcs_schmitt(e_roll, roll_save, dead_zone, hysteresis);
    roll_save = e_roll;
    if (o_roll != o_roll_save)
        roll_count++;
    int o_pitch_save = o_pitch;
    o_pitch = rcs_schmitt(e_pitch, pitch_save, dead_zone, hysteresis);
    pitch_save = e_pitch;
    if (o_pitch != o_pitch_save)
        pitch_count++;
    int o_yaw_save = o_yaw;
    o_yaw = rcs_schmitt(e_yaw, yaw_save, dead_zone, hysteresis);
    yaw_save = e_yaw;
    if (o_yaw != o_yaw_save)
        yaw_count++;

    FMRCS[0] = o_roll * rcs_thrust * rocket_r;
    FMRCS[1] = o_pitch * rcs_thrust * (propulsion->xcg - 1);
    FMRCS[2] = o_yaw * rcs_thrust * (propulsion->xcg - 1);

    FMRCS.fill(fmrcs);
}

int RCS::rcs_schmitt(double input_new, double input, double dead_zone, double hysteresis) {
    // local variable
    int output(0);

    // input signal 'trend' (=1 increasing, =-1 decreasing)
    // input signal 'side' (=-1 left, =1 right)
    int trend = sign(input_new - input);
    int side = sign(input);
    double trigger = (dead_zone * side + hysteresis * trend) / 2;

    if (input >= trigger && side == 1) {
        output = 1;
    } else if (input <= trigger && side == -1) {
        output = -1;
    } else
        output = 0;

    return output;
}

