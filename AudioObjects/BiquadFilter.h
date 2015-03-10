/*
 * BiquadFilter.h
 *
 *  Created on: 04.03.2015
 *      Author: mem
 */

#ifndef BIQUADFILTER_H_
#define BIQUADFILTER_H_

#include <stdbool.h>
#include "filter_biquad.h"

class BiquadFilter : public AudioFilterBiquad {
public:
	BiquadFilter();
	virtual ~BiquadFilter();

	bool isEnabled() {return enabled;}
	void setEnabled(bool en);

	void update();

private:
	bool enabled;
};

#endif /* BIQUADFILTER_H_ */
