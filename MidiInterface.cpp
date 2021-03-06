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
	pitchOffset = volOffset = 0.f;
}

MidiInterface::~MidiInterface() {

}

void MidiInterface::setEnabled(bool en)
{
	config->enabled = en;
}

void MidiInterface::muteControl(control_type_e control, bool m)
{
	config->antenna[control].muted = m;
}

bool MidiInterface::isMuted(control_type_e control)
{
	return config->antenna[control].muted;
}

void MidiInterface::noteOn()
{
	if( ! config->enabled )
		return;

	usbMIDI.sendNoteOn(config->baseNote, config->velocity, config->channel);
}

void MidiInterface::noteOff()
{
	if( ! config->enabled )
		return;

	usbMIDI.sendNoteOff(config->baseNote, 0, config->channel);
}

void MidiInterface::reNote()
{
	if( ! config->enabled )
		return;

	usbMIDI.sendNoteOff(config->baseNote, 0, config->channel);
	usbMIDI.sendNoteOn(config->baseNote, config->velocity, config->channel);
}

void MidiInterface::setConfiguration(midi_configuration_t *config)
{
	noteOff();
	this->config = config;
	noteOn();
}

void MidiInterface::configurationChanged()
{
	reNote();
}

void MidiInterface::setProgram(uint8_t prg)
{
	config->patch = prg;
	usbMIDI.sendProgramChange(prg, config->channel);
}

float MidiInterface::offset(control_type_e control)
{
	if( control == CT_PITCH )
		return pitchOffset;
	return volOffset;
}

void MidiInterface::setOffset(control_type_e control, float offset)
{
	if( ! config->enabled )
		return;

	if( ! config->antenna[control].muted )
	{
		if( config->antenna[control].use14Bit )
		{
			float value = MIDI_PITCHBEND_NULL + offset * MIDI_PITCHBEND_NULL;
			if( value > MIDI_PITCHBEND_MAX ) value = MIDI_PITCHBEND_MAX;
			switch( control )
			{
			case CT_PITCH:
			{
				pitchOffset = offset;
				usbMIDI.sendPitchBend( (uint32_t) value, config->channel );
				break;
			}
			// TODO: determine a second 14Bit parameter! Don't use pitchbend for both!
			case CT_VOLUME:
			{
				volOffset = offset;
				usbMIDI.sendPitchBend( (uint32_t) value, config->channel );
				break;
			}
			}
		}
		else
		{
			float value = MIDI_CC_NULL + offset * MIDI_CC_NULL;
			if( value > MIDI_CC_MAX )	value = MIDI_CC_MAX;
			usbMIDI.sendControlChange( config->antenna[control].cc, (uint32_t) value, config->channel);
		}
	}
}



void MidiInterface::setChannel(uint8_t chn)
{
	noteOff();
	config->channel = chn;
	noteOn();
}

void MidiInterface::setBaseNote(uint8_t note)
{
	noteOff();
	config->baseNote = note;
	noteOn();
}

void MidiInterface::setVelocity( uint8_t velo )
{
	noteOn();
	config->velocity = velo;
	noteOn();
}

void MidiInterface::setCC(control_type_e control, uint8_t cc)
{
	noteOff();
	config->antenna[control].cc = cc;
	noteOn();
}

void MidiInterface::setUse14Bit(control_type_e control, bool use)
{
	noteOff();
	config->antenna[control].use14Bit = use;
	noteOn();
}

