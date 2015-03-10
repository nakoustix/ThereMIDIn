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
{
	if(enabled)
	{
		AudioFilterStateVariable::update();
	}
}

