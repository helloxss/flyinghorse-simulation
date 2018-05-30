#ifndef __Time_management_HH__
#define __Time_management_HH__
/********************************* TRICK HEADER *******************************
PURPOSE:
      (Describe the Time Management Module Variables and Algorithm)
LIBRARY DEPENDENCY:
      ((../src/Time_management.cpp))
PROGRAMMERS:
      (((Lai Jun Xu) () () () ))
*******************************************************************************/
#include "global_constants.hh"
#include "aux.hh"
#include <ctime>
#include "sim_services/include/simtime.h"
#include <iomanip>
#include "time_utility.hh"
#include <armadillo>

class time_management {
    TRICK_INTERFACE(time_management);

 public:
    uint32_t tm_gps_start_time_year;
    uint32_t tm_gps_start_time_month;
    uint32_t tm_gps_start_time_day;
    uint32_t tm_gps_start_time_hour;
    uint32_t tm_gps_start_time_minute;
    double tm_gps_start_time_second;
    static time_management* get_instance() {
        static time_management time;

        return &time;
    }

    time_management(const time_management &other) = delete;
    time_management& operator=(const time_management &other) = delete;

    ~time_management() {}

    void load_start_time(unsigned int Year, unsigned int DOY, unsigned int Hour, unsigned int Min, unsigned int Sec);
    void dm_time();/* convert simulation time to gps time */
    uint16_t get_gpstime_week_num();
    uint32_t get_gpstime_msec_of_week();

    time_util::GPS_TIME get_gpstime();
    time_util::UTC_TIME get_utctime();

    time_util::Modified_julian_date get_modified_julian_date();

 private:
    time_management();

    double last_time;

    time_util::GPS_TIME gpstime;
};

#endif
