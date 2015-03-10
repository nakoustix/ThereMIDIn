/*
 * LCDST7565.h
 *
 *  Created on: 10.03.2015
 *      Author: mem
 */

#ifndef LCDST7565_H_
#define LCDST7565_H_

#include "ST7565.h"
#include "gui.h"
#include "Synthesizer.h"
#include "globals.h"

//--- Configuration: ---
#define LEFT_MARGIN       10 // Should be >4
#define MAX_ITEMS         12
#define MAX_HOLD_COUNT    3  // Loops of holding up or down button
                             //  before continuing to scroll
#define HOLD_SCROLL_SPEED 3
//----------------------

//--- Constants: ---
#define N_LINES 6
#define DOWN_ARROW_Y	46
#define LABEL_LEN 19
#define ST7565_MENU_VERSION "Menu UI v0.1"
//------------------



class LCDST7565 : public ST7565 {
public:
	LCDST7565();

	// Struct used for all types of menu items
	typedef struct {
	  char label[LABEL_LEN+1];
	  int value, update_counter;
	  bool pass_value, funct;
	  void (*function)(void);
	  char* (*data_function)(void);
	  void (LCDST7565::*value_function)(int);
	} MenuItem;


	void buttonEvent(int buttonIndex, button_event_e event);
	void drawMenuButton(gui_menubutton_e but, int slotIndex);

	void showMenu(int m);

	void setSynthesizerInstance(Synthesizer *synthInstance) {synth = synthInstance; }

	//===== Menu =====
    void update();
    void setMenuTitle(char *title);
    void addMenuItem(char *label);
    void addMenuItem(char *label, void (*function)(void));
    //void addMenuItem(char *label, int value, void (*function)(int));
    void addMenuItem(char *label, int value, void (LCDST7565::*function)(int));
    void addMenuDrawFunction(void (*function)(void));
    void drawMenu();
    void clearMenu();

private:
	void drawHome();
	void setSynthValue(int value);
	void menuButtonBack();
	void menuButtonUp();
	void menuButtonDown();
	void menuButtonSelect();
	//void (*synthValueCallback) (int synthPart, int newValue);

	Synthesizer *synth;

	// selected indices
	int selectedPart;
	int currentMenu, lastMenu;

	//===== Menu =====
    MenuItem _menu_items[MAX_ITEMS];
    void scroll(int8_t dir);
    void (*_draw_function)(void);
    bool _has_draw_funct;
    uint8_t _n_items, _item_index, _current_line, lastItemIndex;
    int8_t _draw_index, lastDrawIndex;
    char _title[LABEL_LEN];

};

#endif /* LCDST7565_H_ */
