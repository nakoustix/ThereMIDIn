/*
 * OSCWaveform.h
 *
 *  Created on: 04.03.2015
 *      Author: mem
 */

#ifndef OSCWAVEFORM_H_
#define OSCWAVEFORM_H_

#include "synth_waveform.h"
#include <stdbool.h>

class OSCWaveform : public AudioSynthWaveform {
public:
	OSCWaveform();
	virtual ~OSCWaveform();

	void setEnabled(bool e);
	bool isEnabled() { return enabled; }

	void update();

private:
	bool enabled;
};

#endif /* OSCWAVEFORM_H_ */
