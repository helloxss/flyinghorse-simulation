MKFILE_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
SIM_HOME = $(patsubst %/exe/SIL/master/S_overrides.mk, %, $(MKFILE_PATH))

$(info MKFILE_PATH = $(MKFILE_PATH))
$(info SIM_HOME = $(SIM_HOME))

INCLUDES = -I${TRICK_HOME}/trick_models \
		   -I$(SIM_HOME)/models/gnc/include \
		   -I$(SIM_HOME)/models/dm/include \
		   -I$(SIM_HOME)/models/cad/include \
		   -I$(SIM_HOME)/models/math/include \
		   -I$(SIM_HOME)/models/aux/include \
		   -I$(SIM_HOME)/models/sensor/include \
		   -I$(SIM_HOME)/models/driver/include \
		   -I$(SIM_HOME)/models/icf/include \
		   -I$(SIM_HOME)/models/equipment_protocol/include \
		   -I$(SIM_HOME)/models/flight_events/include

TRICK_CFLAGS += --std=c++11 ${INCLUDES} -g -D_GNU_SOURCE -DCONFIG_SIL_ENABLE
TRICK_CFLAGS += -Wall -Wmissing-prototypes -Wextra -Wshadow
TRICK_CXXFLAGS += --std=c++11 ${INCLUDES} -g -DCONFIG_SIL_ENABLE
TRICK_CXXFLAGS += -Wall -Wextra -Wshadow
TRICK_USER_LINK_LIBS += -larmadillo -lboost_serialization -lgsl -lgslcblas
MAKEFLAGS += -j16
