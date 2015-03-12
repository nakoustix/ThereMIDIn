/*
 * LCDST7565.cpp
 *
 *  Created on: 10.03.2015
 *      Author: mem
 */

#include "LCDST7565.h"
#include <stdio.h>

LCDST7565::LCDST7565()
	: ST7565(PIN_ST7565_SID,PIN_ST7565_SCLK,PIN_ST7565_A0,PIN_ST7565_RST,PIN_ST7565_CS)
{
	synth = 0;
	selectedPart = selectedPartIndex = 0;
	valueMenuActive = false;

	// Menu
	  _title[0]='\0';  // So it will be blank if not set
	  // Set all flags/counters to 0:
	  _n_items = _current_line = _draw_index = _item_index = 0;
	  _has_draw_funct = false;
	  currentMenu = MENU_HOME;
	  lastDrawIndex = lastItemIndex = 0;
	  historyPos = 0;
	  for(int i = 0; i < MENU_HISTORY_SIZE; i++)
	  {
		  menuHistory[i] = MENU_HOME;
		  itemIndexHistory[i] = 0;
		  drawIndexHistory[i] = 0;
		  clineHistory[i] = 0;
	  }

	  contrast = 0x18;
	  begin(0x18);
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
	case BUT_UP:
	{
		this->drawbitmap(x, GUI_BUTTON_Y, button_arrow_left, GUI_BUTTON_WIDTH, GUI_BUTTON_HEIGHT, BLACK);
		break;
	}
	case BUT_DOWN:
	{
		this->drawbitmap(x, GUI_BUTTON_Y, button_arrow_right, GUI_BUTTON_WIDTH, GUI_BUTTON_HEIGHT, BLACK);
		break;
	}
	}
}

void LCDST7565::enterMenu(int m)
{
	if(historyPos > MENU_HISTORY_SIZE - 1) return;
	menuHistory[historyPos] = currentMenu;
	itemIndexHistory[historyPos] = _item_index;
	drawIndexHistory[historyPos] = _draw_index;
	clineHistory[historyPos] = _current_line;
	historyPos++;
	currentMenu = m;
	// clear the current menu
	clearMenu();
	makeMenu(m);
	//lastDrawIndex = _draw_index;
	//lastItemIndex = _item_index;
	update();
}

void LCDST7565::makeMenu(int m)
{
	switch(m)
	{
	case MENU_MAIN:
	{
		setMenuTitle("Mainmenu");
		addMenuItem("Operating Mode", (int) MENU_OPERATING_MODE, &LCDST7565::enterMenu);
		addMenuItem("Calibrate Antennas", (int) MENU_ANTENNA_CALIB, &LCDST7565::calibrateAntennas);
		addMenuItem("MIDI Settings", (int) MENU_MIDI, &LCDST7565::enterMenu);
		addMenuItem("Synth Settings", (int) MENU_SYNTH, &LCDST7565::enterMenu);
		addMenuItem("Display Settings", (int) MENU_DISP, &LCDST7565::enterMenu);
		break;
	}
	case MENU_OPERATING_MODE:
	{
		setMenuTitle("Operating Mode");
		addMenuItem("MIDI / USB-MIDI", (int) OPMODE_MIDI, &LCDST7565::setOperatingMode);
		addMenuItem("Standalone", (int) OPMODE_STANDALONE, &LCDST7565::setOperatingMode);
		break;
	}
	case MENU_DISP:
	{
		setMenuTitle("Display Settings");
		addMenuItem("Background Color", (int) MENU_DISP_COLOR, &LCDST7565::enterValueMenu);
		addMenuItem("Brightness", (int) MENU_DISP_BRIGHT, &LCDST7565::enterValueMenu);
		addMenuItem("Contrast", (int) MENU_DISP_CONTRAST, &LCDST7565::enterValueMenu);
		break;
	}
	case MENU_MIDI:
	{
		setMenuTitle("MIDI Settings");
		addMenuItem("MIDI Channel", MENU_MIDI_CHANNEL, &LCDST7565::enterValueMenu);
		addMenuItem("MIDI Note", MENU_MIDI_NOTE, &LCDST7565::enterValueMenu);
		addMenuItem("MIDI CC Pitch", MENU_MIDI_CC_PITCH, &LCDST7565::enterMenu);
		addMenuItem("MIDI CC Volume", MENU_MIDI_CC_VOL, &LCDST7565::enterMenu);
		break;
	}
	case MENU_MIDI_CC_PITCH:
	{
		setMenuTitle("MIDI CC Pitch");
		//addMenuItem("14Bit Pitch-Bend",
		addMenuItem("Standard MIDI CC", MENU_MIDI_CC_STANDARD, &LCDST7565::enterValueMenu);
		break;
	}
	case MENU_MIDI_CC_VOL:
	{
		setMenuTitle("MIDI CC Volume");
		addMenuItem("Standard MIDI CC", MENU_MIDI_CC_STANDARD, &LCDST7565::enterValueMenu);
		break;
	}
	case MENU_SYNTH:
	{
		setMenuTitle("Synthesizer");
		addMenuItem("OSC1", (int) MENU_SYNTH_OSC1, &LCDST7565::enterMenu);
		addMenuItem("OSC2", (int) MENU_SYNTH_OSC2, &LCDST7565::enterMenu);
		addMenuItem("OSC3", MENU_SYNTH_OSC3, &LCDST7565::enterMenu);
		addMenuItem("Filter", MENU_SYNTH_FILTER, &LCDST7565::enterMenu);
		break;
	}
	case MENU_SYNTH_OSC1:
	case MENU_SYNTH_OSC2:
	case MENU_SYNTH_OSC3:
	{
		selectedPart = m;
		if(m == MENU_SYNTH_OSC1)
		{
			setMenuTitle("OSC1");
			selectedPartIndex = 0;
		}
		else if(m == MENU_SYNTH_OSC2)
		{
			setMenuTitle("OSC2");
			selectedPartIndex = 1;
		}
		else
		{
			setMenuTitle("OSC3");
			selectedPartIndex = 2;
		}
		addMenuItem("Waveform", MENU_SYNTH_OSC_WAVEFORM, &LCDST7565::enterMenu);
		addMenuItem("Wavetable-Position", MENU_SYNTH_OSC_WTPOS, &LCDST7565::enterValueMenu);
		addMenuItem("Amplitude", MENU_SYNTH_OSC_AMPLITUDE, &LCDST7565::enterValueMenu);
		addMenuItem("Semitones", MENU_SYNTH_OSC_SEMI, &LCDST7565::enterValueMenu);
		addMenuItem("Cents", MENU_SYNTH_OSC_CENT, &LCDST7565::enterValueMenu);
		addMenuItem("Dutycycle", MENU_SYNTH_OSC_DUTY, &LCDST7565::enterValueMenu);
		break;
	}
	case MENU_SYNTH_OSC_WAVEFORM:
	{
		setMenuTitle("Waveform");
		addMenuItem("Sine", SINE, &LCDST7565::setSynthProperty);
		addMenuItem("Sawtooth", SAWTOOTH, &LCDST7565::setSynthProperty);
		addMenuItem("Square", SQUARE, &LCDST7565::setSynthProperty);
		addMenuItem("Triangle", TRIANGLE, &LCDST7565::setSynthProperty);
		addMenuItem("Sine", SINE, &LCDST7565::setSynthProperty);
		addMenuItem("Sawtooth", SAWTOOTH, &LCDST7565::setSynthProperty);
		addMenuItem("Square", SQUARE, &LCDST7565::setSynthProperty);
		addMenuItem("Triangle", TRIANGLE, &LCDST7565::setSynthProperty);
		break;
	}
	}
}

void LCDST7565::enterValueMenu(int menu)
{

	if(historyPos > MENU_HISTORY_SIZE - 1) return;
	menuHistory[historyPos] = currentMenu;
	itemIndexHistory[historyPos] = _item_index;
	drawIndexHistory[historyPos] = _draw_index;
	clineHistory[historyPos] = _current_line;
	historyPos++;
	currentMenu = menu;

	valueMenuActive = true;
	clear();
	makeValueMenu(menu);
	update();
}

void LCDST7565::makeValueMenu(int menu)
{
	switch(menu)
	{
	//TODO: set current value when enter a valueMenu
		case MENU_SYNTH_OSC_AMPLITUDE:
		{
			cSynthVal.type = VAL_TYPE_INT;
			cSynthVal.min = 0;
			cSynthVal.max = 100;
			int osc = 0;
			if(selectedPart == MENU_SYNTH_OSC2) osc = 1;
			else if(selectedPart == MENU_SYNTH_OSC3) osc = 2;
			cSynthVal.value = (int) (synth->preset.osc[osc].amplitude * 100);
			cSynthVal.step = 1;
			strcpy(cSynthVal.name, "Amplitude");
			strcpy(cSynthVal.unit, "%");
			break;
		}
		case MENU_MIDI_CHANNEL:
		{
			cSynthVal.type = VAL_TYPE_MIDI_CHANNEL;
			cSynthVal.min = 0;
			cSynthVal.max = 15;
			cSynthVal.value = 0;
			cSynthVal.step = 1;
			strcpy(cSynthVal.name, "MIDI Channel");
			strcpy(cSynthVal.unit, "");
			break;
		}
		case MENU_MIDI_NOTE:
		{
			cSynthVal.type = VAL_TYPE_MIDI_NOTE;
			cSynthVal.min = 0;
			cSynthVal.max = 127;
			cSynthVal.value = 0;
			cSynthVal.step = 1;
			strcpy(cSynthVal.name, "MIDI Note");
			strcpy(cSynthVal.unit, "");
			break;
		}
		case MENU_MIDI_CC_PITCH:
		{
			cSynthVal.type = VAL_TYPE_MIDI_CC;
			cSynthVal.min = 0;
			cSynthVal.max = 127;
			cSynthVal.value = 0;
			cSynthVal.step = 1;
			strcpy(cSynthVal.name, "MIDI CC Pitch");
			strcpy(cSynthVal.unit, "");
			break;
		}
		case MENU_MIDI_CC_VOL:
		{
			cSynthVal.type = VAL_TYPE_MIDI_CC;
			cSynthVal.min = 0;
			cSynthVal.max = 127;
			cSynthVal.value = 0;
			cSynthVal.step = 1;
			strcpy(cSynthVal.name, "MIDI CC Volume");
			strcpy(cSynthVal.unit, "");
			break;
		}
		case MENU_DISP_CONTRAST:
		{
			cSynthVal.type = VAL_TYPE_INT;
			cSynthVal.min = 0;
			cSynthVal.max = 100;
			cSynthVal.value = contrast;
			cSynthVal.step = 1;
			strcpy(cSynthVal.name, "Contrast");
			strcpy(cSynthVal.unit, "%");
			break;
		}
		case MENU_SYNTH_OSC_SEMI:
		{
			cSynthVal.type = VAL_TYPE_INT;
			cSynthVal.min = -24;
			cSynthVal.max = 24;
			cSynthVal.value = synth->preset.osc[selectedPartIndex].semitones;
			cSynthVal.step = 1;
			strcpy(cSynthVal.name, "Semitones");
			strcpy(cSynthVal.unit, "");
			break;
		}
		case MENU_SYNTH_OSC_CENT:
		{
			cSynthVal.type = VAL_TYPE_INT;
			cSynthVal.min = -100;
			cSynthVal.max = 100;
			cSynthVal.value = synth->preset.osc[selectedPartIndex].cents;
			cSynthVal.step = 1;
			strcpy(cSynthVal.name, "Cents");
			strcpy(cSynthVal.unit, "");
			break;
		}
		case MENU_SYNTH_OSC_WTPOS:
		{
			cSynthVal.type = VAL_TYPE_INT;
			cSynthVal.min = 0;
			cSynthVal.max = 100;
			cSynthVal.value = synth->preset.osc[selectedPartIndex].wavetable_position;
			cSynthVal.step = 1;
			strcpy(cSynthVal.name, "Wavetable Position");
			strcpy(cSynthVal.unit, "%");
			break;
		}
		case MENU_SYNTH_OSC_DUTY:
		{
			cSynthVal.type = VAL_TYPE_INT;
			cSynthVal.min = 0;
			cSynthVal.max = 100;
			cSynthVal.value = synth->preset.osc[selectedPartIndex].wavetable_position;
			cSynthVal.step = 1;
			strcpy(cSynthVal.name, "Dutycycle");
			strcpy(cSynthVal.unit, "%");
			break;
		}
	}
}


void LCDST7565::valueMenuUp()
{
	switch(cSynthVal.type)
	{
	case VAL_TYPE_MIDI_CHANNEL:
	case VAL_TYPE_MIDI_NOTE:
	case VAL_TYPE_MIDI_CC:
	case VAL_TYPE_INT:
	{
		cSynthVal.value += cSynthVal.step;
		if(cSynthVal.value > cSynthVal.max)
		{
			cSynthVal.value = cSynthVal.max;
		}
		break;
	}
	case VAL_TYPE_FLOAT:
	{
		cSynthVal.fvalue += cSynthVal.fstep;
		if(cSynthVal.fvalue > cSynthVal.fmax)
		{
			cSynthVal.fvalue = cSynthVal.fmax;
		}
		break;
	}
	}
	updateValue();
	update();
}

void LCDST7565::valueMenuDown()
{
	switch(cSynthVal.type)
	{
	case VAL_TYPE_MIDI_CHANNEL:
	case VAL_TYPE_MIDI_NOTE:
	case VAL_TYPE_MIDI_CC:
	case VAL_TYPE_INT:
	{
		cSynthVal.value -= cSynthVal.step;
		if(cSynthVal.value < cSynthVal.min)
		{
			cSynthVal.value = cSynthVal.min;
		}
		break;
	}
	case VAL_TYPE_FLOAT:
	{
		cSynthVal.fvalue -= cSynthVal.fstep;
		if(cSynthVal.fvalue < cSynthVal.fmin)
		{
			cSynthVal.fvalue = cSynthVal.fmin;
		}
		break;
	}
	}
	updateValue();
	update();
}

void LCDST7565::updateValue()
{
	switch(currentMenu)
	{
	case MENU_SYNTH_OSC_AMPLITUDE:
	{
		synth->setOSCAmplitude(selectedPartIndex, (float) cSynthVal.value / 100.f);
		break;
	}
	case MENU_SYNTH_OSC_SEMI:
	{
		synth->setOSCSemitones(selectedPartIndex, cSynthVal.value);
		break;
	}
	case MENU_SYNTH_OSC_CENT:
	{
		synth->setOSCCents(selectedPartIndex, cSynthVal.value);
		break;
	}
	case MENU_SYNTH_OSC_DUTY:
	{
		synth->setOSCDutycycle(selectedPartIndex, cSynthVal.value);
		break;
	}
	case MENU_DISP_CONTRAST:
	{
		st7565_set_brightness(cSynthVal.value * 63 / 100);
		break;
	}
	}
}

void LCDST7565::setSynthProperty(int value)
{
	char c[19];
	//if(synth == NULL)
		//return;
	this->clear();
	sprintf(c, "menu = %i",currentMenu);
	this->drawstring(0, 0, c);
	sprintf(c, "part = %i",selectedPart);
	this->drawstring(0, 1, c);
	sprintf(c, "value = %i", value);
	this->drawstring(0,2,c);
	this->display();
	while(1);

	switch(currentMenu)
	{
	case MENU_SYNTH_OSC_WAVEFORM:
	{
		switch(selectedPart)
		{
		case MENU_SYNTH_OSC1: synth->setOSCWaveform(0, value); break;
		case MENU_SYNTH_OSC2:
		{
			synth->setOSCWaveform(1, value);
			this->drawstring(0, 3, "Set OSC2.Waveform to:");
			switch(value)
			{
			case SINE: this->drawstring(30, 5, "SINE"); break;
			case SAWTOOTH: this->drawstring(30, 5, "SAWTOOTH"); break;
			case SQUARE: this->drawstring(30, 5, "SQUARE"); break;
			case TRIANGLE: this->drawstring(30, 5, "TRIANGLE"); break;
			}
			break;
		}
		case MENU_SYNTH_OSC3: synth->setOSCWaveform(2, value); break;
		}
	}
	}
	this->display();
}

void LCDST7565::drawHome()
{
	//TODO: Homescreen has to be drawn
}

void LCDST7565::setOperatingMode(int opmode)
{
	//TODO: Change operating mode
}

void LCDST7565::calibrateAntennas(int value)
{
	//TODO: Calibrate antennas
}

void LCDST7565::buttonEvent(int buttonIndex, button_event_e event)
{
	if(currentMenu == MENU_HOME)
	{

	}
	else // We are in a menu or sub menu
	{
		if(event == PRESS_EVENT || event == REPEATED_PRESS_EVENT)
		{
			switch(buttonIndex)
			{
			case BUT_BACK:
			{
				menuBack();
				break;
			}
			case BUT_UP:
			{
				if(valueMenuActive)
				{
					valueMenuUp();
				}
				else
				{
					menuUp();
				}
				break;
			}
			case BUT_DOWN:
			{
				if(valueMenuActive)
				{
					valueMenuDown();
				}
				else
				{
					menuDown();
				}
				break;
			}
			case BUT_OK:
			{
				if(valueMenuActive)
				{
					menuBack();
				}
				else
				{
					menuSelect();
				}
				break;
			}
			}
		}
	}
}

//=================== Menu ================ Menu ================ Menu =============
void LCDST7565::update() {

	if(valueMenuActive)
	{
		drawValueMenu();
	}
	else
	{
		drawMenu();
	}
}

void LCDST7565::menuBack()
{
	if(historyPos <= 1) return;
	historyPos--;
	currentMenu = menuHistory[historyPos];
	// clear the current menu
	valueMenuActive = false;
	clearMenu();
	makeMenu(currentMenu);
	_item_index = itemIndexHistory[historyPos];
	_draw_index = drawIndexHistory[historyPos];
	_current_line = clineHistory[historyPos];
	update();
}

void LCDST7565::menuUp()
{
	// Only move once until button release or MAX_HOLD_COUNT reached
	if (_current_line > 0) _current_line--;
	else scroll(-1); // We're at the top line, scroll up if possible
	if (_item_index > 0)
	{
	  _item_index--;
	}
	update();
}

void LCDST7565::menuDown()
{
	if ((_current_line < N_LINES-2) & (_current_line < _n_items-1)) _current_line++;
	else scroll(1); // We're at the bottom line, scroll down if possible
	if (_item_index < _n_items-1)
	{
		_item_index++;
	}
	update();
}

void LCDST7565::menuSelect()
{
	(this->*_menu_items[_item_index].value_function)(_menu_items[_item_index].value);
	update();
}

void LCDST7565::setMenuTitle(char *label) {
  strcpy(_title, label); // First line of display, blank if not set
}

void LCDST7565::addMenuItem(char *label) {
  if (_n_items >= MAX_ITEMS) return;
  if (strlen(label) > LABEL_LEN) return;
  MenuItem i;
  strcpy(i.label, label);
  i.pass_value = i.funct = false; // Calls no function
  _menu_items[_n_items++] = i;
}

void LCDST7565::addMenuItem(char *label, void (*function)(void)) {
  if (_n_items >= MAX_ITEMS) return;
  if (strlen(label) > LABEL_LEN) return;
  MenuItem i;
  strcpy(i.label, label);
  i.pass_value = false;   // No value to pass to function
  i.funct = true;         // Has a function to call on select
  i.function = function;  // Function to call
  _menu_items[_n_items++] = i;
}

void LCDST7565::addMenuItem(char *label, int value,  void (LCDST7565::*function)(int)) {
  if (_n_items >= MAX_ITEMS) return;
  if (strlen(label) > LABEL_LEN) return;
  MenuItem i;
  strcpy(i.label, label);
  i.pass_value = i.funct = true; // Has function and should pass it a value
  i.value = value;               // Value to pass to function on select
  i.value_function = function;   // Function to call
  _menu_items[_n_items++] = i;
}

void LCDST7565::addMenuDrawFunction(void (*function)(void)) {
  _has_draw_funct = 1;       // Should call function before (*lcd).display()
  _draw_function = function; // Function to call
}


void LCDST7565::scroll(int8_t dir) {
  // Only scrolls screen if possible, doesn't change _item_index
  _draw_index = _draw_index + dir;
  if (_draw_index < 0) _draw_index = 0;
  if (_n_items >= N_LINES) {
    if (_draw_index > _n_items-N_LINES+1) _draw_index = _n_items-N_LINES+1;
  }
  else _draw_index = 0;
}


//========== DRAW ================= DRAW =================== DRAW ====================
void LCDST7565::drawMenu() {
  uint8_t i, label_len;
  this->clear();
  //this->fillrect(0, 0, 128, 7, BLACK);
  this->drawstring(centerString(_title), 0, _title);this->drawstring(LEFT_MARGIN, i+1,
                                         _menu_items[_draw_index+i].label);
  this->invertRect(0,0, 128, 8);
  i=0;
  while ((i<N_LINES-1) & (i<_n_items)) {
    this->drawstring(LEFT_MARGIN, i+1, _menu_items[_draw_index+i].label);
    i++;
  }

  // Draw arrow to indicate current item:
  this->drawline(0, 11+(8*_current_line), LEFT_MARGIN-5,
                                                11+(8*_current_line), BLACK);
  this->drawline(LEFT_MARGIN-5, 8+(8*_current_line), LEFT_MARGIN-2,
                                                11+(8*_current_line), BLACK);
  this->drawline(LEFT_MARGIN-5, 14+(8*_current_line), LEFT_MARGIN-2,
                                                11+(8*_current_line), BLACK);
  this->drawline(LEFT_MARGIN-5, 14+(8*_current_line), LEFT_MARGIN-5,
                                                8+(8*_current_line), BLACK);


  // Draw up arrow if there are items above view
  if (_draw_index > 0) {
    this->drawline(123, 11, 125, 8, BLACK);
    this->drawline(127, 11, 125, 8, BLACK);
  }
  // Draw down arrow if there are items below view
  if ((_n_items - _draw_index) >= N_LINES) {
    this->drawline(123, DOWN_ARROW_Y, 125, DOWN_ARROW_Y + 3, BLACK);
    this->drawline(127, DOWN_ARROW_Y, 125, DOWN_ARROW_Y + 3, BLACK);
  }

  if (_has_draw_funct) _draw_function();
  //this->drawrect(20,20,20,20, BLACK);
  this->drawMenuButton(BUT_UP, 0);
  this->drawMenuButton(BUT_DOWN, 1);
  this->drawMenuButton(BUT_BACK, 2);
  this->drawMenuButton(BUT_OK, 3);
  this->display();
}



void LCDST7565::drawValueMenu()
{
	uint8_t strx;
	uint8_t strline = GUI_VALSTR_LINE;
	uint8_t slen = 0;
	char str[19];
	clear();/*
	drawstring(0,0, cSynthVal.name);
	char c[19];
	sprintf(c, "%i%s", cSynthVal.value, cSynthVal.unit);*/

	// draw title
	drawstring(centerString(cSynthVal.name),0, cSynthVal.name);
	invertRect(0,0,128,8);

	// Draw value bar for floats ints
	if(cSynthVal.type == VAL_TYPE_INT || cSynthVal.type == VAL_TYPE_FLOAT)
	{
		int fillw;
		if(cSynthVal.type == VAL_TYPE_INT)
		{
			int val;
			if(cSynthVal.min < 0)
			{
				val = cSynthVal.value - cSynthVal.min;
			}
			else
			{
				val = cSynthVal.value;
			}
			fillw = (GUI_VALBAR_WIDTH * val) / (cSynthVal.max - cSynthVal.min);
		}
		else
		{
			fillw = (int)  ((GUI_VALBAR_WIDTH * cSynthVal.fvalue) / (cSynthVal.fmax - cSynthVal.fmin));
		}

		drawrect(GUI_VALBAR_X-1, GUI_VALBAR_Y, GUI_VALBAR_WIDTH+2, GUI_VALBAR_HEIGHT, BLACK);
		fillrect(GUI_VALBAR_X, GUI_VALBAR_Y, fillw, GUI_VALBAR_HEIGHT, BLACK);

		for(int x = GUI_VALBAR_X + 9; x < GUI_VALBAR_X+GUI_VALBAR_WIDTH-1; x += 10)
		{
			setpixel(x, GUI_VALBAR_Y-1, BLACK);
			setpixel(x, GUI_VALBAR_Y + GUI_VALBAR_HEIGHT, BLACK);
		}
		//draw left marker
		drawline(GUI_VALBAR_X-2, GUI_VALBAR_Y-2, GUI_VALBAR_X-2, GUI_VALBAR_Y + GUI_VALBAR_HEIGHT+1, BLACK);
		//draw right marker
		drawline(GUI_VALBAR_X + GUI_VALBAR_WIDTH, GUI_VALBAR_Y-2, GUI_VALBAR_X + GUI_VALBAR_WIDTH, GUI_VALBAR_Y + GUI_VALBAR_HEIGHT+1, BLACK);
		//draw middle marker
		//setpixel(GUI_VALBAR_HALF, GUI_VALBAR_Y-1,BLACK);
		setpixel(GUI_VALBAR_HALF+1, GUI_VALBAR_Y-1, BLACK);
		setpixel(GUI_VALBAR_HALF+1, GUI_VALBAR_Y + GUI_VALBAR_HEIGHT, BLACK);
		strline = GUI_VALBAR_STR_LINE;
	}

	// draw value string
	switch(cSynthVal.type)
	{
	case VAL_TYPE_MIDI_CHANNEL:
	case VAL_TYPE_MIDI_NOTE:
	case VAL_TYPE_MIDI_CC:
	case VAL_TYPE_INT:
	{
		sprintf(str, "%i%s", cSynthVal.value, cSynthVal.unit);
		break;
	}
	case VAL_TYPE_FLOAT:
	{
		sprintf(str, "%f%s", cSynthVal.fvalue, cSynthVal.unit);
		break;
	}
	}
	//strx = GUI_CHARS_PER_LINE / 2 - (strlen(str) * 5) / 2 + 1;
	strx = centerString(str);
	drawstring(strx, strline, str);

	this->drawMenuButton(BUT_UP, 0);
	this->drawMenuButton(BUT_DOWN, 1);
	this->drawMenuButton(BUT_BACK, 2);
	this->drawMenuButton(BUT_OK, 3);
	display();
}


void LCDST7565::clearMenu() {
  _n_items = _current_line = _draw_index = _item_index = 0;
  _has_draw_funct = 0;
  _title[0]='\0';  // So it will be blank if set_title() not called
  this->clear();
  //this->display();
}

int LCDST7565::centerString(char *c)
{
	int len = strlen(c);
	/*clear();
	sprintf(c, "%i", len);
	drawstring(0,0,c);
	display();
	while(1);*/
	return GUI_CHARS_PER_LINE *5 / 2 - len * 5 / 2;
}

