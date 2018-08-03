/********************************* TRICK HEADER *******************************
PURPOSE:
      Flight Computer Agent
LIBRARY DEPENDENCY:
      (
        (../src/flight_computer_eqpt.c)
      )
PROGRAMMERS:
      (((Dung-Ru Tsai) () () () ))
*******************************************************************************/
#ifndef MODELS_EQUIPMENT_PROTOCOL_INCLUDE_FLIGHT_COMPUTER_EQPT_H_
#define MODELS_EQUIPMENT_PROTOCOL_INCLUDE_FLIGHT_COMPUTER_EQPT_H_
#include "icf_trx_ctrl.h"
#include "dsp_can_interfaces.h"

struct flight_seq_datablk_t {
    uint32_t target_id;
    uint8_t task_id;
    int queue_idx;
};

#ifdef __cplusplus
extern "C" {
#endif
int fc_can_cmd_dispatch(void *rxframe);
#ifdef __cplusplus
}
#endif
#endif   //  MODELS_EQUIPMENT_PROTOCOL_INCLUDE_FLIGHT_COMPUTER_EQPT_H_
