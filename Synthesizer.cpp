/*
 * Synthesizer.cpp
 *
 *  Created on: 04.03.2015
 *      Author: mem
 */

#include "Synthesizer.h"

Synthesizer::Synthesizer()
{
	osc[0] = &osc1;
	osc[1] = &osc2;
	osc[2] = &osc3;

	currentFreq = 300;
	masterGain = 0.5f;

	// make connections
	osc12sum = new AudioConnection(osc1, 0, oscSum, 0);
	osc22sum = new AudioConnection(osc2, 0, oscSum, 1);
	osc32sum = new AudioConnection(osc3, 0, oscSum, 2);

	oscSum2flt1 = new AudioConnection(oscSum, 0, flt1, 0);
	oscSum2serParMixer = new AudioConnection(oscSum, 0, serParMixer, 0);
	flt12serParMixer = new AudioConnection(flt1, 1, serParMixer, 1);
	flt12fltSum = new AudioConnection(flt1, 1, fltSum, 0);
	serParMixer2flt2 = new AudioConnection(serParMixer, 0, flt2, 0);
	flt22fltSum = new AudioConnection(flt2, 0, fltSum, 1);
	fltSum2fltDryWet = new AudioConnection(fltSum, 0, fltDryWet, 0);
	oscSum2fltDryWet = new AudioConnection(oscSum, 0, fltDryWet, 1);

	fltDryWet2outL = new AudioConnection(fltDryWet, 0, out, 0);
	fltDryWet2outR = new AudioConnection(fltDryWet, 0, out, 1);


	setFilterSerParCrossfade(1.f);

	AudioMemory(20);
}

Synthesizer::~Synthesizer()
{

}

void Synthesizer::enable(float gain)
{
	masterGain = gain;
	audioShield.enable();
	audioShield.volume(gain);
	AudioProcessorUsageMaxReset();
	AudioMemoryUsageMaxReset();
}

void Synthesizer::disconnect()
{
	//TODO: uncomment this!!!
	AudioNoInterrupts();
	//osc12sum->disconnect();
	AudioInterrupts();
}

void Synthesizer::reconnect()
{
	//TODO: uncomment this!!!
	AudioNoInterrupts();
	//osc12sum->connect(osc1, 0, oscSum, 0);
	AudioInterrupts();
}

void Synthesizer::setPreset(synth_preset_t *p)
{
	// FILTER SECTION
	setFilterDryWet(p->filter.dryWet);
	setFilterSerParCrossfade(p->filter.serParCF);

	// OSC SECTION
	for(int i = 0; i < 3; i++)
	{
		setOSCAmplitude(i, p->osc[i].amplitude);
		setOSCWaveform(i, p->osc[i].waveform);
		setOSCWavetablePosition(i, p->osc[i].wavetable_position);
		setOSCDutycycle(i, p->osc[i].duty);
		setOSCSemitones(i, p->osc[i].semitones);
		setOSCCents(i, p->osc[i].cents);
	}
}

void Synthesizer::setFrequency(float f)
{
	AudioNoInterrupts();
	for(int i = 0; i < 3; i++)
	{
		osc[i]->frequency(f);
	}
	AudioInterrupts();
}

void Synthesizer::noteOn()
{
	AudioNoInterrupts();
	for(int i = 0; i < 3; i++)
	{
		osc[i]->begin(preset.osc[i].amplitude, currentFreq, preset.osc[i].waveform);
	}
	AudioInterrupts();
}

void Synthesizer::noteOff()
{
	AudioNoInterrupts();
	for(int i = 0; i < 3; i++)
	{
		osc[i]->amplitude(0);
	}
	AudioInterrupts();
}

// OSC
void Synthesizer::setOSCAmplitude(int index, float amp)
{
	preset.osc[index].amplitude = amp;
	osc[index]->amplitude(amp);
}

void Synthesizer::setOSCCents(int index, int8_t i)
{
	preset.osc[index].cents = i;
	//TODO: update immediately
}

void Synthesizer::setOSCDutycycle(int index, float f)
{
	preset.osc[index].duty = f;
	osc[index]->pulseWidth(f);
}

void Synthesizer::setOSCSemitones(int index, int8_t i)
{
	preset.osc[index].semitones = i;
	//TODO: update immediately
}

void Synthesizer::setOSCWaveform(int index, int wave)
{
	preset.osc[index].waveform = wave;

	//TODO: update immediately
}

void Synthesizer::setOSCWavetablePosition(int index, uint8_t wtpos)
{
	preset.osc[index].wavetable_position = wtpos;
	osc[index]->phase((float) wtpos * 3.6f);
}

// FILTER
void Synthesizer::setFilterSerParCrossfade(float f)
{
	preset.filter.serParCF = f;
	float coeff = 1.f - f;
	serParMixer.gain(0, f);
	serParMixer.gain(1, coeff);
	fltSum.gain(0, f);
}

void Synthesizer::setFilterDryWet(float f)
{
	preset.filter.dryWet = f;
	float coeff = 1.f - f;
	fltDryWet.gain(0, f);
	fltDryWet.gain(1, coeff);
}

void Synthesizer::setFilter1Freq(float f)
{
	flt1.frequency(f);
}

void Synthesizer::setFilter2Freq(float f)
{
}

OSCWaveform * Synthesizer::getOSC(int index)
{
	return osc[index];
}

