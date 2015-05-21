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

