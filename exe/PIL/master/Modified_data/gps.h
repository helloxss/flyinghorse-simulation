#ifndef EXE_PIL_MASTER_MODIFIED_DATA_GPS_H_
#define EXE_PIL_MASTER_MODIFIED_DATA_GPS_H_

#include "trick/DRAscii.hh"
#include "trick/DataRecordGroup.hh"
#include "trick/data_record_proto.h"

extern "C" void record_gps() {
    Trick::DRAscii *drg = new Trick::DRAscii("gps");
    drg->set_freq(Trick::DR_Always);
    drg->set_cycle(0.1);
    drg->set_single_prec_only(false);
    drg->add_variable("rkt.time->gpstime.SOW");
    // drg->add_variable("rkt.gps_con.gdop");
    // drg->add_variable("rkt.gps_con.nsat");
    // drg->add_variable("rkt.newton._SBEE[0]");
    // drg->add_variable("rkt.newton._SBEE[1]");
    // drg->add_variable("rkt.newton._SBEE[2]");
    // drg->add_variable("rkt.newton._VBEE[0]");
    // drg->add_variable("rkt.newton._VBEE[1]");
    // drg->add_variable("rkt.newton._VBEE[2]");
    // drg->add_variable("rkt.newton._ABEE[0]");
    // drg->add_variable("rkt.newton._ABEE[1]");
    // drg->add_variable("rkt.newton._ABEE[2]");
    // drg->add_variable("rkt.gps_con.chan[0].prn");
    // drg->add_variable("rkt.gps_con.chan[0].rho0.range");
    // drg->add_variable("rkt.gps_con.chan[0].rho0.rate");
    // drg->add_variable("rkt.gps_con.chan[1].prn");
    // drg->add_variable("rkt.gps_con.chan[1].rho0.range");
    // drg->add_variable("rkt.gps_con.chan[1].rho0.rate");
    // drg->add_variable("rkt.gps_con.chan[2].prn");
    // drg->add_variable("rkt.gps_con.chan[2].rho0.range");
    // drg->add_variable("rkt.gps_con.chan[2].rho0.rate");
    // drg->add_variable("rkt.gps_con.chan[3].prn");
    // drg->add_variable("rkt.gps_con.chan[3].rho0.range");
    // drg->add_variable("rkt.gps_con.chan[3].rho0.rate");
    // drg->add_variable("rkt.gps_con.chan[4].prn");
    // drg->add_variable("rkt.gps_con.chan[4].rho0.range");
    // drg->add_variable("rkt.gps_con.chan[4].rho0.rate");
    // drg->add_variable("rkt.gps_con.chan[5].prn");
    // drg->add_variable("rkt.gps_con.chan[5].rho0.range");
    // drg->add_variable("rkt.gps_con.chan[5].rho0.rate");
    // drg->add_variable("rkt.gps_con.chan[6].prn");
    // drg->add_variable("rkt.gps_con.chan[6].rho0.range");
    // drg->add_variable("rkt.gps_con.chan[6].rho0.rate");
    // drg->add_variable("rkt.gps_con.chan[7].prn");
    // drg->add_variable("rkt.gps_con.chan[7].rho0.range");
    // drg->add_variable("rkt.gps_con.chan[7].rho0.rate");
    // drg->add_variable("rkt.gps_con.chan[8].prn");
    // drg->add_variable("rkt.gps_con.chan[8].rho0.range");
    // drg->add_variable("rkt.gps_con.chan[8].rho0.rate");
    // drg->add_variable("rkt.gps_con.chan[9].prn");
    // drg->add_variable("rkt.gps_con.chan[9].rho0.range");
    // drg->add_variable("rkt.gps_con.chan[9].rho0.rate");
    // drg->add_variable("rkt.gps_con.chan[10].prn");
    // drg->add_variable("rkt.gps_con.chan[10].rho0.range");
    // drg->add_variable("rkt.gps_con.chan[10].rho0.rate");
    // drg->add_variable("rkt.gps_con.chan[11].prn");
    // drg->add_variable("rkt.gps_con.chan[11].rho0.range");
    // drg->add_variable("rkt.gps_con.chan[11].rho0.rate");
    // drg->add_variable("rkt.gyro->_WBICB[0]");
    // drg->add_variable("rkt.gyro->_WBICB[1]");
    // drg->add_variable("rkt.gyro->_WBICB[2]");
    // drg->add_variable("rkt.gyro->_EWBIB[0]");
    // drg->add_variable("rkt.gyro->_EWBIB[1]");
    // drg->add_variable("rkt.gyro->_EWBIB[2]");
    // drg->add_variable("rkt.sdt._DELTA_BETA[0]");
    // drg->add_variable("rkt.sdt._DELTA_BETA[1]");
    // drg->add_variable("rkt.sdt._DELTA_BETA[2]");
    // drg->add_variable("rkt.kinematics._TBI[0][0]");
    // drg->add_variable("rkt.kinematics._TBI[0][1]");
    // drg->add_variable("rkt.kinematics._TBI[0][2]");
    // drg->add_variable("rkt.kinematics._TBI[1][0]");
    // drg->add_variable("rkt.kinematics._TBI[1][1]");
    // drg->add_variable("rkt.kinematics._TBI[1][2]");
    // drg->add_variable("rkt.kinematics._TBI[2][0]");
    // drg->add_variable("rkt.kinematics._TBI[2][1]");
    // drg->add_variable("rkt.kinematics._TBI[2][2]");
    // drg->add_variable("rkt.newton.lonx");
    // drg->add_variable("rkt.newton.latx");
    drg->add_variable("rkt.accelerometer->_FSPCB[0]");
    drg->add_variable("rkt.accelerometer->_FSPCB[1]");
    drg->add_variable("rkt.accelerometer->_FSPCB[2]");
    // drg->add_variable("rkt.newton._ABII[0]");
    // drg->add_variable("rkt.newton._ABII[1]");
    // drg->add_variable("rkt.newton._ABII[2]");
    drg->add_variable("rkt.newton._FSPB[0]");
    drg->add_variable("rkt.newton._FSPB[1]");
    drg->add_variable("rkt.newton._FSPB[2]");
    drg->add_variable("rkt.newton._NEXT_ACC[0]");
    drg->add_variable("rkt.newton._NEXT_ACC[1]");
    drg->add_variable("rkt.newton._NEXT_ACC[2]");
    // drg->add_variable("rkt.env._GRAVG[0]");
    // drg->add_variable("rkt.env._GRAVG[1]");
    // drg->add_variable("rkt.env._GRAVG[2]");
    // drg->add_variable("rkt.sdt._DELTA_VEL[0]");
    // drg->add_variable("rkt.sdt._DELTA_VEL[1]");
    // drg->add_variable("rkt.sdt._DELTA_VEL[2]");
    // drg->add_variable("rkt.newton._CONING[0]");
    // drg->add_variable("rkt.newton._CONING[1]");
    // drg->add_variable("rkt.newton._CONING[2]");
    drg->add_variable("rkt.newton.liftoff");
    drg->add_variable("rkt.propulsion.thrust");
    add_data_record_group(drg, Trick::DR_Buffer);
    drg->enable();
}

#endif  // EXE_PIL_MASTER_MODIFIED_DATA_GPS_H_
