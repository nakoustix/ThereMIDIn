/*
 * Synthesizer.h
 *
 *  Created on: 04.03.2015
 *      Author: mem
 */

#ifndef SYNTHESIZER_H_
#define SYNTHESIZER_H_

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include "AudioObjects/OSCWaveform.h"
#include "AudioObjects/BandPassFilter.h"
#include "AudioObjects/BiquadFilter.h"
#include "globals.h"

class Synthesizer {
public:
	Synthesizer();
	virtual ~Synthesizer();

	void setPreset(synth_preset_t *p);

	void setOSCAmplitude(int index, float amp);
	void setOSCWaveform(int index, int wave);
	void setOSCWavetablePosition(int index, uint8_t wtpos);
	void setOSCDutycycle(int index, float f);
	void setOSCSemitones(int index, int8_t f);
	void setOSCCents(int index, int8_t f);

	void setFilterSerParCrossfade(float f);
	void setFilterDryWet(float f);
	void setFilter1Freq(float f);
	void setFilter2Freq(float f);

	void setFrequency(float f);
	void noteOn();
	void noteOff();

	void enable(float gain);

	void disconnect();
	void reconnect();

	OSCWaveform * getOSC(int index);
	BandPassFilter *getFilter1() { return &flt1; }
	BiquadFilter *getFilter2() { return &flt2; }


	synth_preset_t 		preset;
private:
	float 				currentFreq;
	float 				masterGain;

	// Audio objects
	OSCWaveform 			osc1;
	OSCWaveform				osc2;
	OSCWaveform				osc3;
	OSCWaveform 			*osc[3];

	BandPassFilter 			flt1;
	BiquadFilter			flt2;

	AudioMixer4           	oscSum;
	AudioMixer4				serParMixer;
	AudioMixer4				fltSum;
	AudioMixer4				fltDryWet;

	AudioOutputI2S          out;

	AudioConnection			*osc12sum;
	AudioConnection			*osc22sum;
	AudioConnection			*osc32sum;

	AudioConnection			*oscSum2flt1;
	AudioConnection			*oscSum2serParMixer;
	AudioConnection			*flt12serParMixer;
	AudioConnection			*flt12fltSum;
	AudioConnection			*serParMixer2flt2;
	AudioConnection			*flt22fltSum;
	AudioConnection			*oscSum2fltDryWet;
	AudioConnection			*fltSum2fltDryWet;

	AudioConnection			*fltDryWet2outL;
	AudioConnection			*fltDryWet2outR;

	AudioControlSGTL5000     audioShield;     //xy=586,175
};

#endif /* SYNTHESIZER_H_ */
