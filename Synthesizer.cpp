/*
 * Synthesizer.cpp
 *
 *  Created on: 04.03.2015
 *      Author: mem
 */

#include "Synthesizer.h"

Synthesizer::Synthesizer()
{
	preset = new synth_preset_t;
	config = new synth_configuration_t;
	osc[0] = &osc1;
	osc[1] = &osc2;
	osc[2] = &osc3;

	currentFreq = 300;
	freqOffset = 0;

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

void Synthesizer::setEnabled(bool en)
{
	config->enabled = en;
	if( en )
		enable(config->masterGain);
	else
		gotoSleep();
	/*if(! en)
	{
		AudioNoInterrupts();
		//audioShield.muteHeadphone();
		//audioShield.volume(0.f);
		AudioInterrupts();
	}*/
}

void Synthesizer::enable(float gain)
{
	audioShield.enable();
	audioShield.volume(gain);
	AudioProcessorUsageMaxReset();
	AudioMemoryUsageMaxReset();
}

void Synthesizer::gotoSleep()
{
	audioShield.volume(0.0);
	audioShield.disable();
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

void Synthesizer::setMasterGain(float g)
{
	audioShield.volume(g);
	config->masterGain = g;
}

void Synthesizer::setBaseFrequency(float f)
{
	config->baseFreq = f;
	updateFrequency();
}

void Synthesizer::setSemiRange(uint8_t range)
{
	config->semiRange = range;
	updateFrequency();
}

void Synthesizer::setOffset(control_type_e control, float o)
{
	if( ! config->enabled )
		return;
	switch( control )
	{
	case CT_PITCH:
	{
		float offset = o * config->semiRange;
		int semis = (int) offset;
		int cents = (int) (offset - semis) * 100;
		float freqOffset = config->baseFreq;
		if( semis < 0 )
		{
			semis *= -1;
			cents *= -1;
			if( semis > 0 )	freqOffset *= semif_minus[semis - 1];
			if( cents > 0 ) freqOffset *= centf_minus[cents - 1];
		}
		else
		{
			if( semis > 0 ) freqOffset *= semif_plus[semis - 1];
			if( cents > 0 ) freqOffset *= centf_plus[cents - 1];
		}
		updateFrequency();
		break;
	}
	case CT_VOLUME:
	{
		break;
	}
	}
}

void Synthesizer::updateFrequency()
{
	AudioNoInterrupts();
	currentFreq = config->baseFreq + freqOffset;
	for(int i = 0; i < 3; i++)
	{
		osc[i]->frequency(getOSCFrequency(i, currentFreq));
	}
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
		setOSCEnabled(i, p->osc[i].enabled);
		setOSCAmplitude(i, p->osc[i].amplitude);
		setOSCWaveform(i, p->osc[i].waveform);
		setOSCWavetablePosition(i, p->osc[i].wavetable_position);
		setOSCDutycycle(i, p->osc[i].duty);
		setOSCSemitones(i, p->osc[i].semitones);
		setOSCCents(i, p->osc[i].cents);
	}
	// Filter section

	preset = p;
}

void Synthesizer::setConfiguration(synth_configuration_t *config)
{
	//this->gotoSleep();
	this->config = config;
	this->setPreset(&config->preset);
	if(config->enabled)
	{
		enable(config->masterGain);
	}
}

void Synthesizer::noteOn()
{
	AudioNoInterrupts();
	for(int i = 0; i < 3; i++)
	{
		osc[i]->begin(preset->osc[i].amplitude, getOSCFrequency(i, currentFreq), preset->osc[i].waveform);
	}
	AudioInterrupts();
}


float Synthesizer::getOSCFrequency(int i, float baseFreq)
{
	int semi,cent;
	float f;
	semi = preset->osc[i].semitones;
	cent = preset->osc[i].cents;
	if(semi == 0)
	{
		f = baseFreq;
	}
	else if(semi > 0)
	{
		f = baseFreq * semif_plus[semi-1];
	}
	else
	{
		semi *= -1;
		f = baseFreq * semif_minus[semi-1];
	}
	if(cent > 0)
	{
		f *= centf_plus[cent-1];
	}
	else if(cent < 0)
	{
		cent *= -1;
		f *= centf_minus[cent-1];
	}
	return f;
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

void Synthesizer::setOSCEnabled(int index, bool en)
{
	preset->osc[index].enabled = en;
	osc[index]->setEnabled(en);
}

void Synthesizer::setOSCAmplitude(int index, float amp)
{
	preset->osc[index].amplitude = amp;
	osc[index]->amplitude(amp);
}

void Synthesizer::setOSCCents(int index, int8_t i)
{
	preset->osc[index].cents = i;
	osc[index]->frequency(getOSCFrequency(index, currentFreq));
}

void Synthesizer::setOSCDutycycle(int index, float f)
{
	preset->osc[index].duty = f;
	osc[index]->pulseWidth(f);
}

void Synthesizer::setOSCSemitones(int index, int s)
{
	preset->osc[index].semitones = s;
	osc[index]->frequency(getOSCFrequency(index, currentFreq));
}

void Synthesizer::setOSCWaveform(int index, int wave)
{
	preset->osc[index].waveform = wave;
	noteOn();
}

void Synthesizer::setOSCWavetablePosition(int index, float wtpos)
{
	preset->osc[index].wavetable_position = wtpos;
	osc[index]->phase((float) wtpos * 3.6f);
}

// FILTER
void Synthesizer::setFilterSerParCrossfade(float f)
{
	preset->filter.serParCF = f;
	float coeff = 1.f - f;
	serParMixer.gain(0, f);
	serParMixer.gain(1, coeff);
	fltSum.gain(0, f);
}

void Synthesizer::setFilterDryWet(float f)
{
	preset->filter.dryWet = f;
	float coeff = 1.f - f;
	fltDryWet.gain(0, f);
	fltDryWet.gain(1, coeff);
}

void Synthesizer::setFilter1Enabled(bool en)
{
	preset->filter.flt1.enabled = en;
	flt1.setEnabled(en);
}

void Synthesizer::setFilter1Freq(float f)
{
	preset->filter.flt1.frequency = f;
	flt1.frequency(f);
}

void Synthesizer::setFilter1Reso(float f)
{
	preset->filter.flt1.resonance = f;
	flt1.resonance(f);
}

void Synthesizer::setFilter2Freq(float f)
{
}


OSCWaveform * Synthesizer::getOSC(int index)
{
	return osc[index];
}

