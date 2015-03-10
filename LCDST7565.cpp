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
	selectedPart = 0;

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
	showMenu(m);
	//lastDrawIndex = _draw_index;
	//lastItemIndex = _item_index;
	update();
}

void LCDST7565::showMenu(int m)
{
	switch(m)
	{
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
		}
		else if(m == MENU_SYNTH_OSC2)
		{
			setMenuTitle("OSC2");
		}
		else
		{
			setMenuTitle("OSC3");
		}
		addMenuItem("Waveform", MENU_SYNTH_OSC_WAVEFORM, &LCDST7565::enterMenu);
		addMenuItem("Wavetable-Position", MENU_SYNTH_OSC_WTPOS, &LCDST7565::enterMenu);
		addMenuItem("Amplitude", MENU_SYNTH_OSC_AMPLITUDE, &LCDST7565::enterMenu);
		break;
	}
	case MENU_SYNTH_OSC_WAVEFORM:
	{
		setMenuTitle("Waveform");
		addMenuItem("Sine", SINE, &LCDST7565::setSynthValue);
		addMenuItem("Sawtooth", SAWTOOTH, &LCDST7565::setSynthValue);
		addMenuItem("Square", SQUARE, &LCDST7565::setSynthValue);
		addMenuItem("Triangle", TRIANGLE, &LCDST7565::setSynthValue);
		addMenuItem("Sine", SINE, &LCDST7565::setSynthValue);
		addMenuItem("Sawtooth", SAWTOOTH, &LCDST7565::setSynthValue);
		addMenuItem("Square", SQUARE, &LCDST7565::setSynthValue);
		addMenuItem("Triangle", TRIANGLE, &LCDST7565::setSynthValue);
		break;
	}
	}
}

void LCDST7565::setSynthValue(int value)
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

}

void LCDST7565::buttonEvent(int buttonIndex, button_event_e event)
{
	if(currentMenu == MENU_HOME)
	{

	}
	else // We are in a menu or sub menu
	{
		if(event == PRESS_EVENT)
		{
			switch(buttonIndex)
			{
			case BUT_BACK:
			{
				menuButtonBack();
				break;
			}
			case BUT_UP:
			{
				menuButtonUp();
				break;
			}
			case BUT_DOWN:
			{
				menuButtonDown();
				break;
			}
			case BUT_OK:
			{
				menuButtonSelect();
				break;
			}
			}
		}
	}
}

//=================== Menu ================ Menu ================ Menu =============
void LCDST7565::update() {

  drawMenu();
}

void LCDST7565::menuButtonBack()
{
	if(historyPos <= 0) return;
	historyPos--;
	currentMenu = menuHistory[historyPos];
	// clear the current menu
	clearMenu();
	showMenu(currentMenu);
	_item_index = itemIndexHistory[historyPos];
	_draw_index = drawIndexHistory[historyPos];
	_current_line = clineHistory[historyPos];
	update();
}

void LCDST7565::menuButtonUp()
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

void LCDST7565::menuButtonDown()
{
	if ((_current_line < N_LINES-2) & (_current_line < _n_items-1)) _current_line++;
	else scroll(1); // We're at the bottom line, scroll down if possible
	if (_item_index < _n_items-1)
	{
		_item_index++;
	}
	update();
}

void LCDST7565::menuButtonSelect()
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

void LCDST7565::drawMenu() {
  uint8_t i, label_len;
  this->clear();
  //this->fillrect(0, 0, 128, 7, BLACK);
  this->drawstring(0, 0, _title);this->drawstring(LEFT_MARGIN, i+1,
                                         _menu_items[_draw_index+i].label, BLACK);
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

void LCDST7565::clearMenu() {
  _n_items = _current_line = _draw_index = _item_index = 0;
  _has_draw_funct = 0;
  _title[0]='\0';  // So it will be blank if set_title() not called
  this->clear();
  //this->display();
}
