/* 
 * File:   utils.h
 * Author: vincent
 *
 * Created on October 27, 2015, 10:55 AM
 */

#ifndef UTILS_H
#define	UTILS_H

#include <stdint.h>


#define M_TWOPI         (M_PI * 2.0)

#ifdef	__cplusplus
extern "C" {
#endif


float regFen(float val_, float b1_i, float b1_f, float b2_i, float b2_f);

float regFenLim(float val_, float b1_i, float b1_f, float b2_i, float b2_f);


/**
 * distance_between5: 24ms
 * distance_between2: 24ms
 * distance_between3: 21ms
 * distance_between4: 40ms
 *
 */
float distance_between5(float, float, float, float);
inline float distance_between(float lat1, float long1, float lat2, float long2) {
	return distance_between5(lat1, long1, lat2, long2);
}

float distance_between(float lat1, float long1, float lat2, float long2);

void calculePos (const char *nom, float *lat, float *lon);

long unsigned int toBase10 (char *entree);

extern void loggerMsg(const char *msg_);

double radians(double value);

double degrees(double value);

double sq(double value);

uint32_t get_sec_jour(uint8_t hour_, uint8_t min_, uint8_t sec_);

float compute2Complement(uint8_t msb, uint8_t lsb);

float percentageBatt(float tensionValue, float current);

void encode_uint16 (uint8_t* dest, uint16_t input);

void encode_uint32 (uint8_t* dest, uint32_t input);

uint16_t decode_uint16 (uint8_t* dest);

uint32_t decode_uint32 (uint8_t* dest);

void const_char_to_buffer(const char *str_, uint8_t *buff_, uint16_t max_size);

float simpLinReg(float* x, float* y, float* lrCoef, int n);

#ifdef	__cplusplus
}
#endif

#endif	/* UTILS_H */

