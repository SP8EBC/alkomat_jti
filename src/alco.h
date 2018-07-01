/*
 * alco.h
 *
 *  Created on: 01.12.2017
 *      Author: mateusz
 */

#ifndef ALCO_H_
#define ALCO_H_


#include <stdint.h>

#define LN 48

class alco {
private:
	uint16_t adcValues[48];
	uint16_t meanValue;
	uint16_t scaledCorrection;

	uint16_t* begin;
	uint16_t* end;
	uint16_t* head;
	uint16_t* tail;
public:
	alco();

	void push_back(uint16_t value);
	void calculate_mean(void);
	void correction_temperature(float t);

	uint16_t getMeanValue() const {
		return meanValue;
	}

	uint16_t getScaledCorrection() const {
		return scaledCorrection;
	}
};

#endif /* ALCO_H_ */
