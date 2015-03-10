/*
 * BandPassFilter.h
 *
 *  Created on: 04.03.2015
 *      Author: mem
 */

#ifndef BANDPASSFILTER_H_
#define BANDPASSFILTER_H_

#include "filter_variable.h"
#include <stdbool.h>

class BandPassFilter : public AudioFilterStateVariable {
public:
	BandPassFilter();
	virtual ~BandPassFilter();

	bool isEnabled() {return enabled;}
	void setEnabled(bool en);

	void update();
private:
	bool enabled;

};

#endif /* BANDPASSFILTER_H_ */
