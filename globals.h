/*
 * globals.h
 *
 *  Created on: 03.03.2015
 *      Author: mem
 */

#ifndef GLOBALS_H_
#define GLOBALS_H_

#include <stdint.h>
#include <stdbool.h>

#define ADC_MAX				4095

// Pin definitions
#define PIN_ST7565_BL_R		2
#define PIN_ST7565_SID		4
#define PIN_ST7565_CS		5
#define PIN_ST7565_RST		6
#define PIN_ST7565_A0		7
#define PIN_ST7565_SCLK		8
#define PIN_ST7565_BL_G		20
#define PIN_ST7565_BL_B		21

#define PIN_FMETER_OSC1		10
#define PIN_FMETER_OSC2		3

#define PIN_MIDI_RX			0
#define PIN_MIDI_TX			1

#define PIN_ROTARY_A		16
#define PIN_ROTARY_B		19

// default midi cc
#define MIDI_CC_PITCHBEND	128
#define MIDI_CC_VOLUME		16

extern void midiConfigChanged();
extern void operatingModeChanged();

typedef enum
{
	CT_PITCH = 0,
	CT_VOLUME,
} control_type_e;

enum ModulationDestination
{
	MOD_DST_GLOBAL_PITCH,
	MOD_DST_GLOBAL_VOL,
};

typedef enum
{
	OSC1 = 0,
	OSC2,
	OSC3
} osc_e;

typedef enum
{
	OSC1Amp = 0,
	OSC1WtPos,
	OSC1Wave,
	OSC2Amp,
	OSC2WtPos,
	OSC2Wave,
	OSC3Amp,
	OSC3WtPos,
	OSC3Wave,
	FLTLPFreq,
	FLTLPGain,
	FLTHPFreq,
	FLTHPGain
} synthlink_e;

typedef enum
{
	PitchAntenna = 0,
	VolumeAntenna
} antenna_e;

typedef struct
{
	uint8_t midi_cc;
	uint32_t base_freq;
	uint32_t top_freq;
} antenna_configuration_t;

typedef struct
{
	bool use14Bit;
	uint8_t cc;
} midi_antenna_conf_t;

typedef struct
{
	midi_antenna_conf_t antenna[2];
	uint8_t baseNote;
	uint8_t channel;
	uint8_t velocity;
} midi_configuration_t;

enum OperatingMode
{
	OPMODE_SYNTH,
	OPMODE_MIDI
};


typedef enum
{
	SINE = 0,
	SAWTOOTH,
	SQUARE,
	TRIANGLE,
	ARBITRARY,
	PULSE
} osc_waveform_e;

typedef struct
{
	bool enabled;
	uint8_t waveform;
	float wavetable_position;
	float duty;
	float amplitude;
	int8_t semitones;
	int8_t cents;
} osc_configuration_t;

typedef struct
{
	bool enabled;
	float frequency;
	float resonance;
} flt1_configuration_t;

typedef struct
{
	float frequency;
} flt2_configuration_t;

typedef struct
{
	flt1_configuration_t flt1;
	flt2_configuration_t flt2;
	float serParCF;
	float dryWet;
} filter_configuration_t;

typedef struct
{
	osc_configuration_t osc[3];
	filter_configuration_t filter;
} synth_preset_t;

typedef struct
{
	float masterGain;
	float baseFreq;
	synth_preset_t preset;
} synth_configuration_t;

typedef struct
{
	uint8_t destination;
	uint16_t min, max;
} synth_link_t;

typedef struct
{
	uint8_t midi_cc;
	synth_link_t synth_link;
} knob_configuration_t;

typedef struct
{
	int operatingMode;
	antenna_configuration_t antenna[2];
	knob_configuration_t knob[4];
	midi_configuration_t midiConf;
	synth_configuration_t synthConf;
} thereMIDIn_configuration_t;

extern float semif_plus[24];
extern float semif_minus[24];
extern float centf_plus[100];
extern float centf_minus[100];


#endif /* GLOBALS_H_ */
