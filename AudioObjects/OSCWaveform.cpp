/*
 * OSCWaveform.cpp
 *
 *  Created on: 04.03.2015
 *      Author: mem
 */

#include "OSCWaveform.h"

OSCWaveform::OSCWaveform()
	: AudioSynthWaveform()
{
	enabled = true;
}

OSCWaveform::~OSCWaveform() {

}

void OSCWaveform::setEnabled(bool e)
{
	enabled = e;
}

void OSCWaveform::update()
{
	if(enabled)
	{
		AudioSynthWaveform::update();
	}
}

