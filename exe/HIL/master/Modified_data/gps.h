#ifndef EXE_HIL_MASTER_MODIFIED_DATA_GPS_H_
#define EXE_HIL_MASTER_MODIFIED_DATA_GPS_H_

#include "trick/DRAscii.hh"
#include "trick/DataRecordGroup.hh"
#include "trick/data_record_proto.h"

extern "C" void record_gps() {
    Trick::DRAscii *drg = new Trick::DRAscii("gps");
    drg->set_freq(Trick::DR_Always);
    drg->set_cycle(0.5);
    drg->set_single_prec_only(false);
    drg->add_variable("rkt.time->gpstime.SOW");
    // drg->add_variable("rkt.gps_con.gdop");
    // drg->add_variable("rkt.gps_con.nsat");
    drg->add_variable("rkt.dynamics._SBEE[0]");
    drg->add_variable("rkt.dynamics._SBEE[1]");
    drg->add_variable("rkt.dynamics._SBEE[2]");
    drg->add_variable("rkt.dynamics._VBEE[0]");
    drg->add_variable("rkt.dynamics._VBEE[1]");
    drg->add_variable("rkt.dynamics._VBEE[2]");
    drg->add_variable("rkt.dynamics._SBII[0]");
    drg->add_variable("rkt.dynamics._SBII[1]");
    drg->add_variable("rkt.dynamics._SBII[2]");
    drg->add_variable("rkt.dynamics._VBII[0]");
    drg->add_variable("rkt.dynamics._VBII[1]");
    drg->add_variable("rkt.dynamics._VBII[2]");
    // drg->add_variable("rkt.dynamics._SBIIP[0]");
    // drg->add_variable("rkt.dynamics._SBIIP[1]");
    // drg->add_variable("rkt.dynamics._SBIIP[2]");
    // drg->add_variable("rkt.dynamics._VBIIP[0]");
    // drg->add_variable("rkt.dynamics._VBIIP[1]");
    // drg->add_variable("rkt.dynamics._VBIIP[2]");
    // drg->add_variable("rkt.newton._ABII[0]");
    // drg->add_variable("rkt.newton._ABII[1]");
    // drg->add_variable("rkt.newton._ABII[2]");
    // drg->add_variable("rkt.newton._ABEE[0]");
    // drg->add_variable("rkt.newton._ABEE[1]");
    // drg->add_variable("rkt.newton._ABEE[2]");
    // drg->add_variable("rkt.euler._WBEB[0]");
    // drg->add_variable("rkt.euler._WBEB[1]");
    // drg->add_variable("rkt.euler._WBEB[2]");
    // drg->add_variable("rkt.gyro->_EWBIB[0]");
    // drg->add_variable("rkt.gyro->_EWBIB[1]");
    // drg->add_variable("rkt.gyro->_EWBIB[2]");
    // drg->add_variable("rkt.accelerometer->_EFSPB[0]");
    // drg->add_variable("rkt.accelerometer->_EFSPB[1]");
    // drg->add_variable("rkt.accelerometer->_EFSPB[2]");
    // drg->add_variable("rkt.sdt._DELTA_BETA[0]");
    // drg->add_variable("rkt.sdt._DELTA_BETA[1]");
    // drg->add_variable("rkt.sdt._DELTA_BETA[2]");
    // drg->add_variable("rkt.dynamics._TBI[0][0]");
    // drg->add_variable("rkt.dynamics._TBI[0][1]");
    // drg->add_variable("rkt.dynamics._TBI[0][2]");
    // drg->add_variable("rkt.dynamics._TBI[1][0]");
    // drg->add_variable("rkt.dynamics._TBI[1][1]");
    // drg->add_variable("rkt.dynamics._TBI[1][2]");
    // drg->add_variable("rkt.dynamics._TBI[2][0]");
    // drg->add_variable("rkt.dynamics._TBI[2][1]");
    // drg->add_variable("rkt.dynamics._TBI[2][2]");
    // drg->add_variable("rkt.env._TEI[0][0]");
    // drg->add_variable("rkt.env._TEI[0][1]");
    // drg->add_variable("rkt.env._TEI[0][2]");
    // drg->add_variable("rkt.env._TEI[1][0]");
    // drg->add_variable("rkt.env._TEI[1][1]");
    // drg->add_variable("rkt.env._TEI[1][2]");
    // drg->add_variable("rkt.env._TEI[2][0]");
    // drg->add_variable("rkt.env._TEI[2][1]");
    // drg->add_variable("rkt.env._TEI[2][2]");
    drg->add_variable("rkt.dynamics.lonx");
    drg->add_variable("rkt.dynamics.latx");
    // drg->add_variable("rkt.accelerometer->_FSPCB[0]");
    // drg->add_variable("rkt.accelerometer->_FSPCB[1]");
    // drg->add_variable("rkt.accelerometer->_FSPCB[2]");
    // drg->add_variable("rkt.newton._ABII[0]");
    // drg->add_variable("rkt.newton._ABII[1]");
    // drg->add_variable("rkt.newton._ABII[2]");
    // drg->add_variable("rkt.newton._FSPB[0]");
    // drg->add_variable("rkt.newton._FSPB[1]");
    // drg->add_variable("rkt.newton._FSPB[2]");
    // drg->add_variable("rkt.newton._NEXT_ACC[0]");
    // drg->add_variable("rkt.newton._NEXT_ACC[1]");
    // drg->add_variable("rkt.newton._NEXT_ACC[2]");
    // drg->add_variable("rkt.env._GRAVG[0]");
    // drg->add_variable("rkt.env._GRAVG[1]");
    // drg->add_variable("rkt.env._GRAVG[2]");
    // drg->add_variable("rkt.dynamics._WBIBD[0]");
    // drg->add_variable("rkt.dynamics._WBIBD[1]");
    // drg->add_variable("rkt.dynamics._WBIBD[2]");
    // drg->add_variable("rkt.env.wind->_VAED[0]");
    // drg->add_variable("rkt.env.wind->_VAED[1]");
    // drg->add_variable("rkt.env.wind->_VAED[2]");
    drg->add_variable("rkt.sdt->_DELTA_VEL[0]");
    drg->add_variable("rkt.sdt->_DELTA_VEL[1]");
    drg->add_variable("rkt.sdt->_DELTA_VEL[2]");
    // drg->add_variable("rkt.sdt->_PHI[0]");
    // drg->add_variable("rkt.sdt->_PHI[1]");
    // drg->add_variable("rkt.sdt->_PHI[2]");
    // drg->add_variable("rkt.newton._CONING[0]");
    // drg->add_variable("rkt.newton._CONING[1]");
    // drg->add_variable("rkt.newton._CONING[2]");
    // drg->add_variable("rkt.newton.liftoff");
    // drg->add_variable("rkt.dynamics._NEXT_ACC[0]");
    // drg->add_variable("rkt.dynamics._NEXT_ACC[1]");
    // drg->add_variable("rkt.dynamics._NEXT_ACC[2]");
    // drg->add_variable("rkt.propulsion.thrust");
    // drg->add_variable("rkt.propulsion.vmass");
    // drg->add_variable("rkt.dynamics.alt");
    drg->add_variable("rkt.dynamics.psibdx");
    drg->add_variable("rkt.dynamics.thtbdx");
    drg->add_variable("rkt.dynamics.phibdx");
    drg->add_variable("rkt.dynamics.alphax");
    // drg->add_variable("rkt.env.vmach");
    // drg->add_variable("rkt.kinematics.alppx");
    // drg->add_variable("rkt.dynamics._thtvdx");
    // drg->add_variable("rkt.dynamics.liftoff");
    // drg->add_variable("rkt.newton._grndtrck");
    // drg->add_variable("rkt.kinematics.phipx");
    // drg->add_variable("rkt.env.wind->_VAED[0]");
    // drg->add_variable("rkt.env.wind->_VAED[1]");
    // drg->add_variable("rkt.env.wind->_VAED[2]");
    // drg->add_variable("rkt.dynamics._VBAB[0]");
    // drg->add_variable("rkt.dynamics._VBAB[1]");
    // drg->add_variable("rkt.dynamics._VBAB[2]");
    // drg->add_variable("rkt.newton._VBED[0]");
    // drg->add_variable("rkt.newton._VBED[1]");
    // drg->add_variable("rkt.newton._VBED[2]");
    // drg->add_variable("rkt.dynamics.lonx");
    // drg->add_variable("rkt.dynamics.latx");
    // drg->add_variable("rkt.newton._VBED[0]");
    // drg->add_variable("rkt.newton._VBED[1]");
    // drg->add_variable("rkt.newton._VBED[2]");
    // drg->add_variable("rkt.aerodynamics.cn");
    // drg->add_variable("rkt.aerodynamics.ca");
    // drg->add_variable("rkt.aerodynamics.ca_on");
    // drg->add_variable("rkt.aerodynamics.cl");
    // drg->add_variable("rkt.aerodynamics.cnq");
    // drg->add_variable("rkt.aerodynamics.cmq");
    // drg->add_variable("rkt.aerodynamics.xcp");
    // drg->add_variable("rkt.newton._ABIB[0]");
    // drg->add_variable("rkt.newton._ABIB[1]");
    // drg->add_variable("rkt.newton._ABIB[2]");
    // drg->add_variable("rkt.aerodynamics.clm");
    // drg->add_variable("rkt.aerodynamics.cln");
    // drg->add_variable("rkt.kinematics.alphaix");
    // drg->add_variable("rkt.newton._thtvdx");
    // drg->add_variable("rkt.forces._FAPB[0]");
    // drg->add_variable("rkt.forces._FAPB[1]");
    // drg->add_variable("rkt.forces._FAPB[2]");
    // drg->add_variable("rkt.forces._FMAB[0]");
    // drg->add_variable("rkt.forces._FMAB[1]");
    // drg->add_variable("rkt.forces._FMAB[2]");
    // drg->add_variable("rkt.kinematics.alppx");
    // drg->add_variable("rkt.env.vmach");
    // drg->add_variable("rkt.aerodynamics.xcp");
    // drg->add_variable("rkt.aerodynamics.clm");
    // drg->add_variable("rkt.aerodynamics.cln");
    // drg->add_variable("rkt.propulsion.xcg");
    // drg->add_variable("rkt.propulsion.vmass");
    // drg->add_variable("rkt.propulsion.fmasse");
    // drg->add_variable("rkt.propulsion.fmass0");
    // drg->add_variable("rkt.propulsion.mass_ratio");
    // drg->add_variable("rkt.propulsion._IBBB[0][0]");
    // drg->add_variable("rkt.propulsion._IBBB[1][1]");
    // drg->add_variable("rkt.propulsion._IBBB[2][2]");
    // drg->add_variable("rkt.env.atmosphere->pressure");
    // drg->add_variable("rkt.env.atmosphere->tempk");
    // drg->add_variable("rkt.env.atmosphere->density");
    // drg->add_variable("rkt.env.atmosphere->vsound");
    // drg->add_variable("rkt.env.atmosphere->zu");
    // drg->add_variable("rkt.env.atmosphere->zl");
    // drg->add_variable("rkt.env.atmosphere->ht");
    // drg->add_variable("rkt.env.atmosphere->z");
    // drg->add_variable("rkt.env.atmosphere->g");
    // drg->add_variable("rkt.env.atmosphere->tmb");
    // drg->add_variable("rkt.env.atmosphere->expv");
    // drg->add_variable("rkt.propulsion.thrust");
    // drg->add_variable("rkt.tvc.s2_act1_y1");
    // drg->add_variable("rkt.tvc.lx");
    // drg->add_variable("rkt.tvc._FPB[0]");
    // drg->add_variable("rkt.tvc._FPB[1]");
    // drg->add_variable("rkt.tvc._FPB[2]");
    // drg->add_variable("rkt.tvc._FMPB[0]");
    // drg->add_variable("rkt.tvc._FMPB[1]");
    // drg->add_variable("rkt.tvc._FMPB[2]");
    // drg->add_variable("rkt.tvc.s2_act2_y1");
    // drg->add_variable("rkt.tvc.s2_act3_y1");
    // drg->add_variable("rkt.tvc.s2_act4_y1");
    // drg->add_variable("rkt.tvc.s2_act1_y1_saturation");
    // drg->add_variable("rkt.tvc.s2_act2_y1_saturation");
    // drg->add_variable("rkt.tvc.s2_act3_y1_saturation");
    // drg->add_variable("rkt.tvc.s2_act4_y1_saturation");
    // drg->add_variable("rkt.tvc.s2_act1_y2");
    drg->add_variable("rkt.tvc.s2_act2_y2");
    // drg->add_variable("rkt.tvc.s2_act3_y2");
    // drg->add_variable("rkt.tvc.s2_act2_y2");
    // drg->add_variable("rkt.tvc.s2_act3_y2");
    // drg->add_variable("rkt.tvc.s2_act4_y2");
    // drg->add_variable("rkt.tvc.s2_act1_y2_saturation");
    // drg->add_variable("rkt.tvc.s2_act2_y2_saturation");
    // drg->add_variable("rkt.tvc.s2_act3_y2_saturation");
    // drg->add_variable("rkt.tvc.s2_act4_y2_saturation");
    // drg->add_variable("rkt.tvc.s2_act1_rate");
    drg->add_variable("rkt.tvc.s2_act2_rate");
    // drg->add_variable("rkt.tvc.s2_act3_rate");
    // drg->add_variable("rkt.tvc.s2_act2_rate");
    // drg->add_variable("rkt.tvc.s2_act3_rate");
    // drg->add_variable("rkt.tvc.s2_act4_rate");
    // drg->add_variable("rkt.tvc.s2_act1_rate_saturation");
    // drg->add_variable("rkt.tvc.s2_act2_rate_saturation");
    // drg->add_variable("rkt.tvc.s2_act3_rate_saturation");
    // drg->add_variable("rkt.tvc.s2_act4_rate_saturation");
    // drg->add_variable("rkt.tvc.s3_act1_y2_saturation");
    // drg->add_variable("rkt.tvc.s3_act2_y2_saturation");
    // drg->add_variable("rkt.tvc.s3_act3_y2_saturation");
    // drg->add_variable("rkt.tvc.s3_act4_y2_saturation");
    // drg->add_variable("rkt.tvc.s3_act1_rate_saturation");
    // drg->add_variable("rkt.tvc.s3_act3_rate_saturation");
    // drg->add_variable("rkt.tvc.s3_act2_rate_saturation");
    // drg->add_variable("rkt.tvc.s3_act4_rate_saturation");
    // drg->add_variable("rkt.tvc.theta_a_cmd");
    // drg->add_variable("rkt.tvc.theta_b_cmd");
    // drg->add_variable("rkt.tvc.theta_c_cmd");
    // drg->add_variable("rkt.tvc.theta_d_cmd");
    // drg->add_variable("rkt.forces.pendulum_L");
    // drg->add_variable("rkt.forces.slosh_mass");
    // drg->add_variable("rkt.gyro->_WBICB[0]");
    // drg->add_variable("rkt.gyro->_WBICB[1]");
    // drg->add_variable("rkt.gyro->_WBICB[2]");
    // drg->add_variable("rkt.dynamics._WBIB[0]");
    // drg->add_variable("rkt.dynamics._WBIB[1]");
    // drg->add_variable("rkt.dynamics._WBIB[2]");
    // drg->add_variable("rkt.forces._FMB[0]");
    // drg->add_variable("rkt.forces._FMB[1]");
    // drg->add_variable("rkt.forces._FMB[2]");
    // drg->add_variable("rkt.forces._FAPB[0]");
    // drg->add_variable("rkt.forces._FAPB[1]");
    // drg->add_variable("rkt.forces._FAPB[2]");
    // drg->add_variable("rkt.forces._FMAB[0]");
    // drg->add_variable("rkt.forces._FMAB[1]");
    // drg->add_variable("rkt.forces._FMAB[2]");
    // drg->add_variable("rkt.env.pdynmc");
    add_data_record_group(drg, Trick::DR_Buffer);
    drg->enable();
}

#endif  // EXE_HIL_MASTER_MODIFIED_DATA_GPS_H_
