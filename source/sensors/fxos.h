/*
 * FXOS.h
 *
 *  Created on: 12 nov. 2017
 *      Author: Vincent
 */

#ifndef SOURCE_SENSORS_FXOS_H_
#define SOURCE_SENSORS_FXOS_H_


#include "fsl_fxos.h"
#include "VueCommon.h"


#define FXOS_7BIT_ADDRESS      0x1E


/*=========================================================================*/

#define FXOS_STANDBY_REGS(_reg1) \
{                                          \
		CTRL_REG1,                             \
		(uint8_t)_reg1 & ~((uint8_t)ACTIVE_MASK),                                     \
}

#define FXOS_STANDBY(p_cmd, p_buffer) \
		I2C_READ_REG(FXOS_7BIT_ADDRESS, p_cmd, p_buffer, 1),  \
		I2C_WRITE(FXOS_7BIT_ADDRESS, p_cmd+1, 1)

#define FXOS_RESET_REGS \
{                                          \
		CMD_RESET,                             \
}

#define FXOS_CMD(p_cmd) \
		I2C_WRITE(FXOS_7BIT_ADDRESS, p_cmd, 1)

#define FXOS_READ_ALL_REGS \
{                                          \
		OUT_X_MSB_REG,                         \
		M_OUT_X_MSB_REG,                       \
}

#define FXOS_READ_ALL(p_cmd, p_mag_buffer, p_acc_buffer) \
		I2C_READ_REG(FXOS_7BIT_ADDRESS, p_cmd, p_acc_buffer, 6), \
		I2C_READ_REG(FXOS_7BIT_ADDRESS, p_cmd+1, p_mag_buffer, 6)


/*=========================================================================*/

bool fxos_init(void);

void fxos_calibration_start(void);

void fxos_readChip(void);

bool is_fxos_updated(void);

void fxos_tasks(void);

bool fxos_get_yaw(float &yaw_rad);

bool fxos_get_pitch(float &yaw_rad);

tHistoValue fxos_histo_read(uint16_t ind_);

uint16_t fxos_histo_size(void);

#endif /* SOURCE_SENSORS_FXOS_H_ */
