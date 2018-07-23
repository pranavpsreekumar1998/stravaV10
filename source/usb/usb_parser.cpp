/*
 * usb_parser.cpp
 *
 *  Created on: 4 juil. 2018
 *      Author: Vincent
 */

#include "segger_wrapper.h"
#include "usb_parser.h"
#include "Model.h"


#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"


/**
 * Decodes chars from the VCOM line
 * @param c
 */
void usb_cdc_decoder(char c) {

	NRF_LOG_RAW_INFO("%c", c);
	NRF_LOG_FLUSH();

	// TODO
	switch (vparser.encode(c)) {
	case _SENTENCE_LOC:

		locator.sim_loc.data.lat = (float)vparser.getLat() / 10000000.;
		locator.sim_loc.data.lon = (float)vparser.getLon() / 10000000.;
		locator.sim_loc.data.alt = (float)vparser.getEle();
		locator.sim_loc.data.utc_time = vparser.getSecJ();

		locator.sim_loc.setIsUpdated();

		break;
	default:
		break;

	}

}