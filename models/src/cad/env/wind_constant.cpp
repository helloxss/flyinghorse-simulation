#include "cad/env/wind_constant.hh"

#include <cstring>

cad::Wind_Constant::Wind_Constant(double dvba, double dir, double twind, double vertical_wind)
    :   Wind(twind, vertical_wind)
{
    strcpy(name, "Constant Wind");

    altitude = 0;

    vwind  = dvba;
    psiwdx = dir;
}

cad::Wind_Constant::~Wind_Constant(){
}

void cad::Wind_Constant::set_altitude(double altitude_in_meter){
    altitude = altitude_in_meter;
}
