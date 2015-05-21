/*
 * MidiInterface.cpp
 *
 *  Created on: 21.05.2015
 *      Author: nAkoustix
 */

#include "MidiInterface.h"
#include "USB_MIDI.h"

MidiInterface::MidiInterface() {
	config = new midi_configuration_t;
}

MidiInterface::~MidiInterface() {

}

void MidiInterface::setConfiguration(midi_configuration_t *config)
{
	this->config = config;
	configurationChanged();
}

void MidiInterface::configurationChanged()
{
	reNote();
}

void MidiInterface::reNote()
{
	usbMIDI.sendNoteOff(config->baseNote, 0, config->channel);
	usbMIDI.sendNoteOn(config->baseNote, config->velocity, config->channel);
}

void MidiInterface::setOffset(control_type_e control, float offset)
{
	if( config->antenna[control].use14Bit )
	{
		float value = MIDI_PITCHBEND_NULL + offset * MIDI_PITCHBEND_NULL;
		if( value > MIDI_PITCHBEND_MAX ) value = MIDI_PITCHBEND_MAX;
		switch( control )
		{
		case CT_PITCH:	usbMIDI.sendPitchBend( (uint32_t) value, config->channel ); break;
		// TODO: determine a second 14Bit parameter! Don't use pitchbend for both!
		case CT_VOLUME:	usbMIDI.sendPitchBend( (uint32_t) value, config->channel ); break;
		}
	}
	else
	{
		float value = MIDI_CC_NULL + offset * MIDI_CC_NULL;
		if( value > MIDI_CC_MAX )	value = MIDI_CC_MAX;
		usbMIDI.sendControlChange( config->antenna[control].cc, (uint32_t) value, config->channel);
	}
}



void MidiInterface::setChannel(uint8_t chn)
{
	config->channel = chn;
	configurationChanged();
}

void MidiInterface::setBaseNote(uint8_t note)
{
	config->baseNote = note;
	configurationChanged();
}

void MidiInterface::setVelocity( uint8_t velo )
{
	config->velocity = velo;
	configurationChanged();
}

void MidiInterface::setCC(control_type_e control, uint8_t cc)
{
	config->antenna[control].cc = cc;
	configurationChanged();
}

void MidiInterface::setUse14Bit(control_type_e control, bool use)
{
	config->antenna[control].use14Bit = use;
	configurationChanged();
}

