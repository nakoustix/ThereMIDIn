/*
 * MidiInterface.h
 *
 *  Created on: 21.05.2015
 *      Author: nAkoustix
 */

#ifndef MIDIINTERFACE_H_
#define MIDIINTERFACE_H_

#include "globals.h"

class MidiInterface {
public:
	MidiInterface();
	virtual ~MidiInterface();

	void setConfiguration(midi_configuration_t *config);
	void reNote();

	void setChannel(uint8_t chn);
	void setBaseNote(uint8_t note);
	void setVelocity(uint8_t velo);
	void setCC(control_type_e control, uint8_t cc);
	void setUse14Bit(control_type_e control, bool use);

	midi_configuration_t *configuration() const { return config; }

private:
	void configurationChanged();

	midi_configuration_t *config;
};

#endif /* MIDIINTERFACE_H_ */
