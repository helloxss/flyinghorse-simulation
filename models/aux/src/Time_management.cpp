#include "Time_management.hh"

time_management::time_management() {
    last_time = 0;
}

void time_management::dm_time(double int_step) { /* convert simulation time to gps time */
    double this_time = get_rettime() + int_step;

    /* Get current GPS time */
    /* deduct DM_dt because PV comes from dm_att() output of previous cycle */
    gpstime += this_time - last_time;

    last_time = this_time;
}

// Load simulation start time in Calender date form and UTC time and convert into gps time form
void time_management::load_start_time(unsigned int Year, unsigned int DOY, unsigned int Hour, unsigned int Min, unsigned int Sec) {
    time_util::UTC_TIME caldate;

    caldate.set_day_of_year(Year, DOY);
    caldate.set_hour(Hour);
    caldate.set_min(Min);
    caldate.set_sec(Sec);
    tm_gps_start_time_year = caldate.get_year();
    tm_gps_start_time_month = caldate.get_month();
    tm_gps_start_time_day = caldate.get_day();
    tm_gps_start_time_hour = caldate.get_hour();
    tm_gps_start_time_minute = caldate.get_min();
    tm_gps_start_time_second = caldate.get_sec() + 18;  // UTC time always behind GPS time 18 secs
    this->gpstime = time_util::GPS_TIME(caldate);
}

time_util::GPS_TIME time_management::get_gpstime() {
    // cout<<setprecision(10)<<"gps week: "<<gpstime.get_week()<<'\t'<<"gps sow :"<<gpstime.get_SOW()<<'\t'<<endl;
    return gpstime;
}

time_util::UTC_TIME time_management::get_utctime() {
    time_util::UTC_TIME ret(gpstime);
    // cout<<setprecision(10)<<"Year: "<<ret.get_year()<<'\t'<<"Month: "<<ret.get_month()<<'\t'<<"Day: "<<ret.get_day()<<'\t'<<"Min: "<<ret.get_min()<<'\t'<<"Sec: "<<ret.get_sec()<<endl;
    return ret;
}

time_util::Modified_julian_date time_management::get_modified_julian_date() {
    time_util::Modified_julian_date ret(gpstime);
    // cout<<setprecision(20)<<"Julian_Date: "<<ret.get_jd()<<endl;
    return ret;
}

uint16_t time_management::get_gpstime_week_num() {
    uint16_t week_num;
    week_num = gpstime.get_week();
    return week_num;
}

uint32_t time_management::get_gpstime_msec_of_week() {
    uint32_t msec_of_week;
    msec_of_week = gpstime.get_SOW() * 1000;
    return msec_of_week;
}
