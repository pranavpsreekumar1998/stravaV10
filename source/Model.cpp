/*
 * Global.cpp
 *
 *  Created on: 17 oct. 2017
 *      Author: Vincent
 */

#include "Model.h"
#include "nrf_pwr_mgmt.h"
#include "sdk_config.h"
#include "neopixel.h"
#include "segger_wrapper.h"

#include "i2c_scheduler.h"
#include "uart.h"

#if defined (BLE_STACK_SUPPORT_REQD)
#include "ble_api_base.h"
#endif


#ifdef USB_ENABLED
#include "usb_cdc.h"
#endif

SAtt att;

SufferScore   suffer_score;

Attitude      attitude;

ListeSegments mes_segments;

ListeParcours mes_parcours;

ListePoints   mes_points;

Locator       locator;

Boucle        boucle;

SegmentManager     segMngr;

Vue           vue;

STC3100       stc;

VEML6075      veml;

AltiBaro      baro;

GPS_MGMT      gps_mgmt;

VParser       vparser;

sBacklightOrders     backlight;

sNeopixelOrders      neopixel;

sTasksIDs     m_tasks_id;

sAppErrorDescr m_app_error __attribute__ ((section(".noinit")));


// init counters
int Point2D::objectCount2D = 0;
int Point::objectCount = 0;

/**
 *
 */
void model_dispatch_sensors_update(void) {

	uint16_t light_level = veml.getRawUVA();

	LOG_DEBUG("Light level: %u", light_level);
	NRF_LOG_DEBUG("Temperature: %ld", (int)baro.m_temperature);
	NRF_LOG_DEBUG("Pressure: %ld", (int)baro.m_pressure);

	// check if backlighting is used for notifying
	if (backlight.freq == 0) {
		// setup backlight
		if (light_level < BACKLIGHT_AUTO_START_RAW_VIS) {
			// TODO il fait tout noir: TG
			backlight.state = 0;
		} else {
			// sun is shining
			backlight.state = 0;
		}
	}
}

void model_get_navigation(sKomootNavigation *nav) {

#ifdef BLE_STACK_SUPPORT_REQD
	ble_get_navigation(nav);
#endif

}

/**
 *
 */
void perform_system_tasks(void) {

	uart_tasks();

#ifdef USB_ENABLED
	usb_cdc_tasks();
#endif

#if APP_SCHEDULER_ENABLED
	app_sched_execute();
#endif

}

/**
 *
 */
void perform_system_tasks_light(void) {

	uart_tasks();

#if APP_SCHEDULER_ENABLED
	app_sched_execute();
#endif

	if (NRF_LOG_PROCESS() == false)
	{
		nrf_pwr_mgmt_run();
	}
}

/**
 *
 */
void model_go_to_msc_mode(void) {

	boucle.uninit();

}

/**
 *
 * @return true if memory is full, false otherwise
 */
bool check_memory_exception(void) {

	int tot_point_mem = Point::getObjectCount() * sizeof(Point);
	tot_point_mem += Point2D::getObjectCount() * sizeof(Point2D);
	tot_point_mem += segMngr.getNbSegs() * sizeof(sSegmentData);

	if (tot_point_mem + 1000 > TOT_HEAP_MEM_AVAILABLE) {

		LOG_ERROR("Memory exhausted");

		return true;
	}

	return false;
}


/**
 *
 * @param p_context
 */
void idle_task(void * p_context)
{
    for(;;)
    {
		perform_system_tasks();

#if defined (BLE_STACK_SUPPORT_REQD)
		ble_nus_tasks();
#endif

		// BSP tasks
		bsp_tasks();

    	sysview_task_idle();

    	//No more logs to process, go to sleep
    	nrf_pwr_mgmt_run();

    	task_yield();
    }
}

/**
 * Triggered externally when device has new valid data
 *
 * @param p_context
 */
void boucle_task(void * p_context)
{
	for (;;)
	{
		LOG_INFO("\r\nTask %u", millis());

		boucle.run();

		if (!millis()) NRF_LOG_WARNING("No millis");
	}
}

/**
 * Task triggered every APP_TIMEOUT_DELAY_MS.
 *
 * @param p_context
 */
void ls027_task(void * p_context)
{
	for(;;)
	{
		wdt_reload();

		events_wait(TASK_EVENT_LS027_TRIGGER);

		// check screen update & unlock task
		vue.writeWhole();
	}
}

/**
 * Task triggered every APP_TIMEOUT_DELAY_MS.
 *
 * @param p_context
 */
void peripherals_task(void * p_context)
{
	for(;;)
	{
#ifdef _DEBUG_TWI
		static uint32_t _counter = 0;

		if (++_counter >= SENSORS_REFRESH_PER_MS / APP_TIMEOUT_DELAY_MS) {
			_counter = 0;
			stc.refresh(nullptr);
			veml.refresh(nullptr);
			if (boucle.getGlobalMode() != eBoucleGlobalModesFEC) fxos_tasks(nullptr);
			if (boucle.getGlobalMode() != eBoucleGlobalModesFEC) baro.refresh(nullptr);
		}

		model_dispatch_sensors_update();
#endif

#ifndef BLE_STACK_SUPPORT_REQD
		neopixel_radio_callback_handler(false);
#endif

#ifdef ANT_STACK_SUPPORT_REQD
		roller_manager_tasks();
		suffer_score.addHrmData(hrm_info.bpm, millis());
#endif

		// check screen update & unlock task
		if (millis() - vue.getLastRefreshed() > LS027_TIMEOUT_DELAY_MS) {
			vue.refresh();
		}

		gps_mgmt.runWDT();

		gps_mgmt.tasks();

		locator.tasks();

		// update date
		SDate dat;
		locator.getDate(dat);
		attitude.addNewDate(&dat);

		notifications_tasks();

		backlighting_tasks();

		events_wait(TASK_EVENT_PERIPH_TRIGGER);
	}
}


