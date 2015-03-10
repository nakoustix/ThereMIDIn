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

	// Menu
	  _title[0]='\0';  // So it will be blank if not set
	  // Set all flags/counters to 0:
	  _n_items = _current_line = _draw_index = _item_index = 0;
	  _has_draw_funct = false;
	  currentMenu = lastMenu = MENU_HOME;
	  lastDrawIndex = lastItemIndex = 0;
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
	lastMenu = currentMenu;
	currentMenu = m;
	lastDrawIndex = _draw_index;
	lastItemIndex = _item_index;
	// clear the current menu
	clearMenu();
	switch(m)
	{
	case MENU_SYNTH:
	{
		setMenuTitle("Synthesizer");
		addMenuItem("OSC1", (int) MENU_SYNTH_OSC1, &LCDST7565::showMenu);
		addMenuItem("OSC2", (int) MENU_SYNTH_OSC2, &LCDST7565::showMenu);
		addMenuItem("OSC3", MENU_SYNTH_OSC3, &LCDST7565::showMenu);
		addMenuItem("Filter", MENU_SYNTH_FILTER, &LCDST7565::showMenu);
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
		addMenuItem("Waveform", MENU_SYNTH_OSC_WAVEFORM, &LCDST7565::showMenu);
		addMenuItem("Wavetable-Pos.", MENU_SYNTH_OSC_WTPOS, &LCDST7565::showMenu);
		addMenuItem("Amplitude", MENU_SYNTH_OSC_AMPLITUDE, &LCDST7565::showMenu);
		break;
	}
	case MENU_SYNTH_OSC_WAVEFORM:
	{
		setMenuTitle("Waveform");
		addMenuItem("Sine", SINE, &LCDST7565::setSynthOscWaveform);
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

void LCDST7565::buttonEvent(button_event_t event)
{
	if(currentMenu == MENU_HOME)
	{

	}
	else // We are in a menu or sub menu
	{
		if(event.event == PRESS_EVENT)
		{
			switch(event.buttonIndex)
			{
			case 0:
			{
				break;
			}
			case 1:
			{
				menuButtonUp();
				break;
			}
			case 2:
			{
				menuButtonDown();
				break;
			}
			case 3:
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
  this->drawstring(0, 0, _title);this->drawstring(LEFT_MARGIN, i+1,
                                         _menu_items[_draw_index+i].label);
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
    this->drawline(123, 60, 125, 63, BLACK);
    this->drawline(127, 60, 125, 63, BLACK);
  }

  if (_has_draw_funct) _draw_function();
  this->display();
}

void LCDST7565::clearMenu() {
  _n_items = _current_line = _draw_index = _item_index = 0;
  _has_draw_funct = 0;
  _title[0]='\0';  // So it will be blank if set_title() not called
  this->clear();
  //this->display();
}
