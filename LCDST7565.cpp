/*
 * LCDST7565.cpp
 *
 *  Created on: 10.03.2015
 *      Author: mem
 */

#include "LCDST7565.h"

LCDST7565::LCDST7565()
	: ST7565(0,0,0,0,0)
{
	synth = 0;
	selectedPart = 0;
	menu = new Menu(0,0,0, this);
}

void LCDST7565::drawMenuButton(gui_menubutton_e but, int slotIndex)
{
	int x = slotIndex * GUI_BUTTON_WIDTH;

	switch(but)
	{
	case BUT_OK:
	{
		this->drawbitmap(x, GUI_BUTTON_Y, button_ok, GUI_BUTTON_WIDTH, GUI_BUTTON_HEIGHT, BLACK);
		break;
	}
	case BUT_BACK:
	{
		this->drawbitmap(x, GUI_BUTTON_Y, button_back, GUI_BUTTON_WIDTH, GUI_BUTTON_HEIGHT, BLACK);
		break;
	}
	case BUT_MENU:
	{
		this->drawbitmap(x, GUI_BUTTON_Y, button_menu, GUI_BUTTON_WIDTH, GUI_BUTTON_HEIGHT, BLACK);
		break;
	}
	case BUT_SYNTH:
	{
		this->drawbitmap(x, GUI_BUTTON_Y, button_synth, GUI_BUTTON_WIDTH, GUI_BUTTON_HEIGHT, BLACK);
		break;
	}
	case BUT_LEFT:
	{
		this->drawbitmap(x, GUI_BUTTON_Y, button_arrow_left, GUI_BUTTON_WIDTH, GUI_BUTTON_HEIGHT, BLACK);
		break;
	}
	case BUT_RIGHT:
	{
		this->drawbitmap(x, GUI_BUTTON_Y, button_arrow_right, GUI_BUTTON_WIDTH, GUI_BUTTON_HEIGHT, BLACK);
		break;
	}
	}
}

void LCDST7565::showMenu(int m)
{
	// clear the current menu
	menu->clear();
	switch(m)
	{
	case MENU_SYNTH:
	{
		menu->setTitle("Synthesizer");
		menu->addItem("OSC1", MENU_SYNTH_OSC1, showMenu);
		menu->addItem("OSC2", MENU_SYNTH_OSC2, showMenu);
		menu->addItem("OSC3", MENU_SYNTH_OSC3, showMenu);
		menu->addItem("Filter", MENU_SYNTH_FILTER, showMenu);
		break;
	}
	case MENU_SYNTH_OSC1:
	case MENU_SYNTH_OSC2:
	case MENU_SYNTH_OSC3:
	{
		selectedPart = m;
		if(m == MENU_SYNTH_OSC1)
		{
			menu->setTitle("OSC1");
		}
		else if(m == MENU_SYNTH_OSC2)
		{
			menu->setTitle("OSC2");
		}
		else
		{
			menu->setTitle("OSC3");
		}
		menu->addItem("Waveform", MENU_SYNTH_OSC_WAVEFORM, showMenu);
		menu->addItem("Wavetable-Pos.", MENU_SYNTH_OSC_WTPOS, showMenu);
		menu->addItem("Amplitude", MENU_SYNTH_OSC_AMPLITUDE, showMenu);
		break;
	}
	case MENU_SYNTH_OSC_WAVEFORM:
	{
		menu->setTitle("Waveform");
		menu->addItem("Sine", SINE, setSynthOscWaveform);
		break;
	}
	}
}

void LCDST7565::setSynthOscWaveform(int waveform)
{
	if(synth == NULL)
		return;

	switch(selectedPart)
	{
	case MENU_SYNTH_OSC1: synth->setOSCWaveform(0, waveform); break;
	case MENU_SYNTH_OSC2: synth->setOSCWaveform(1, waveform); break;
	case MENU_SYNTH_OSC3: synth->setOSCWaveform(2, waveform); break;
	}
}

void LCDST7565::drawHome()
{

}
