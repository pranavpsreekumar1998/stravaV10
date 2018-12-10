/*
 * GUI_connector.H
 *
 *  Created on: 6 d�c. 2018
 *      Author: Vincent
 */

#ifndef TDD_GUI_CONNECTOR_H_
#define TDD_GUI_CONNECTOR_H_

#include <stdint.h>


void GUI_connector_init(void);

void GUI_UpdateLS027(void);

extern "C" void neopixel_update(uint8_t red, uint8_t green, uint8_t blue);


#endif /* TDD_GUI_CONNECTOR_H_ */
