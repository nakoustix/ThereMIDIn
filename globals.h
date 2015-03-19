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
	uint8_t midi_cc;
} midi_antenna_conf_t;

typedef struct
{
	midi_antenna_conf_t pitch, volume;
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


float semif_minus[24] =
{
	0.9438740447523228,
	0.89089821235711,
	0.8408956991601192,
	0.7936996247810941,
	0.7491524751605323,
	0.7071055768659856,
	0.6674186009034223,
	0.6299590943776496,
	0.5946020384387424,
	0.5612294310391519,
	0.5297298931089691,
	0.4999982968349784,
	0.4719354148029035,
	0.4454475888318818,
	0.4204464173959179,
	0.39684846058910844,
	0.37457496164997456,
	0.35355158411550774,
	0.3337081637276954,
	0.3149784742645302,
	0.29730000651397753,
	0.2806137596532399,
	0.26486404433705973,
	0.24999829683787916,
};


float semif_plus[24] =
{
	1.0594633950999308,
	1.122462685556672,
	1.1892081277128579,
	1.2599224804670965,
	1.3348417487183963,
	1.4142159710183209,
	1.4983100540596157,
	1.5874046567863613,
	1.6817971270763188,
	1.7818024941215864,
	1.8877545198195804,
	2.000006812683292,
	2.1189340079884325,
	2.244933018096128,
	2.378424357124058,
	2.519853544387025,
	2.6696925912908718,
	2.828441576642159,
	2.996630315631103,
	3.174820128057906,
	3.3636057117038263,
	3.563617127099255,
	3.7755219003128384,
	4.000027250779582,
};

#endif /* GLOBALS_H_ */
