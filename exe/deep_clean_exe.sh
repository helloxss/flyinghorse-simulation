#!/bin/bash
set -x
LOCAL_FILE_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
SIM_HOME_PATH=$(echo $LOCAL_FILE_DIR | sed 's/\/exe//g')
cd $SIM_HOME_PATH/exe/HIL/master
make clean
cd $SIM_HOME_PATH/exe/HIL/slave
make clean
cd $SIM_HOME_PATH/exe/HIL/simgen_sample_code
make clean
cd $SIM_HOME_PATH/exe/HIL/sdt_sample_code
make clean
cd $SIM_HOME_PATH/exe/HIL/ratetable_sample_code
make clean
cd $SIM_HOME_PATH/exe/HIL/fc_can_test
make clean
cd $SIM_HOME_PATH/exe/PIL/master
make clean
cd $SIM_HOME_PATH/exe/PIL/slave
make clean
cd $SIM_HOME_PATH/exe/SIL/master
make clean
cd $SIM_HOME_PATH/exe/SIL/slave
make clean
$SIM_HOME_PATH/exe/xil_common/script/clean_script.sh $SIM_HOME_PATH/exe/
set +x
