/*
 * BiquadFilter.cpp
 *
 *  Created on: 04.03.2015
 *      Author: mem
 */

#include "BiquadFilter.h"

BiquadFilter::BiquadFilter()
	: AudioFilterBiquad()
{
	enabled = true;
}

BiquadFilter::~BiquadFilter() {

}

void BiquadFilter::setEnabled(bool en)
{
	enabled = en;
}

void BiquadFilter::update()
{
	if(enabled)
	{
		AudioFilterBiquad::update();
	}
}
