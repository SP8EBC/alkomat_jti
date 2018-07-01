#include "alco.h"
#include <cstring>
#include <cmath>

void alco::push_back(uint16_t value) {
	if (head != end) {
		*head = value;
		head++;
	}
	else {
		*head = value;
		head = begin;
	}
}

void alco::calculate_mean(void) {
	uint64_t mean = 0;
	for (uint16_t i = 0; i < LN; i++) {
		mean += *(begin + i);
	}
	mean /= LN;
	this->meanValue = mean;
}

alco::alco() {
	memset(adcValues, 0x00, LN);

	begin = adcValues;
	end = adcValues + LN - 1;
	head = adcValues;
	tail = adcValues;

	meanValue = 0;
	scaledCorrection = 0;
}

void alco::correction_temperature(float t) {
	uint16_t in = this->meanValue;
	float correction = pow(0.979807692f, t) + 0.3666f;

	this->scaledCorrection = (uint16_t)(correction * 100);
	this->meanValue = in * correction;
}
