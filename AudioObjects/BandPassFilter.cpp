/*
 * BandPassFilter.cpp
 *
 *  Created on: 04.03.2015
 *      Author: mem
 */

#include "BandPassFilter.h"

BandPassFilter::BandPassFilter()
	: AudioFilterStateVariable()
{
	enabled = true;
}

BandPassFilter::~BandPassFilter() {

}

void BandPassFilter::setEnabled(bool en)
{
	enabled = en;
}

void BandPassFilter::update()
{/*
	if(enabled)
	{
		AudioFilterStateVariable::update();
	}*/
	audio_block_t *input_block=NULL, *control_block=NULL;
	audio_block_t *lowpass_block=NULL, *bandpass_block=NULL, *highpass_block=NULL;

	input_block = receiveReadOnly(0);
	control_block = receiveReadOnly(1);
	if (!input_block) {
		if (control_block) release(control_block);
		return;
	}
	if(enabled)
	{
		lowpass_block = allocate();
		if (!lowpass_block) {
			release(input_block);
			if (control_block) release(control_block);
			return;
		}
		bandpass_block = allocate();
		if (!bandpass_block) {
			release(input_block);
			release(lowpass_block);
			if (control_block) release(control_block);
			return;
		}
		highpass_block = allocate();
		if (!highpass_block) {
			release(input_block);
			release(lowpass_block);
			release(bandpass_block);
			if (control_block) release(control_block);
			return;
		}

		if (control_block) {
			AudioFilterStateVariable::update_variable(input_block->data,
				 control_block->data,
				 lowpass_block->data,
				 bandpass_block->data,
				 highpass_block->data);
			release(control_block);
		} else {
			AudioFilterStateVariable::update_fixed(input_block->data,
				 lowpass_block->data,
				 bandpass_block->data,
				 highpass_block->data);
		}
		release(input_block);
		transmit(lowpass_block, 0);
		release(lowpass_block);
		transmit(bandpass_block, 1);
		release(bandpass_block);
		transmit(highpass_block, 2);
		release(highpass_block);
		return;
	}
	else
	{
		transmit(input_block, 0);
		transmit(input_block, 1);
		transmit(input_block, 2);
		release(input_block);
	}
}

