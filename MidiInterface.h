/*
 * MidiInterface.h
 *
 *  Created on: 21.05.2015
 *      Author: nAkoustix
 */

#ifndef MIDIINTERFACE_H_
#define MIDIINTERFACE_H_

#include "globals.h"

#define MIDI_PITCHBEND_MAX	16383
#define MIDI_PITCHBEND_NULL	8192
#define MIDI_CC_NULL		64
#define MIDI_CC_MAX			127

class MidiInterface {
public:
	MidiInterface();
	virtual ~MidiInterface();

	void setEnabled(bool en);
	bool isEnabled()  {return config->enabled;}
	void setConfiguration(midi_configuration_t *config);
	void reNote();
	void noteOn();
	void noteOff();
	void nextProgram();

	void setChannel(uint8_t chn);
	void setBaseNote(uint8_t note);
	void setVelocity(uint8_t velo);
	void setCC(control_type_e control, uint8_t cc);
	void setUse14Bit(control_type_e control, bool use);

	void muteControl(control_type_e control, bool mute);
	bool isMuted(control_type_e control);

	void setOffset(control_type_e control, float offset);

	midi_configuration_t *configuration() const { return config; }

private:
	void configurationChanged();

	midi_configuration_t *config;
};

#endif /* MIDIINTERFACE_H_ */
