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
#define N_LINES 8
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
    void addMenuTimeoutFunction(int timeout, void (*function)(void));
    void drawMenu();
    void clearMenu();
    int inactive_count;

private:
	void drawHome();
	void setSynthOscWaveform(int waveform);
	//void (*synthValueCallback) (int synthPart, int newValue);

	Synthesizer *synth;

	// selected indices
	int selectedPart;

	//===== Menu =====
    MenuItem _menu_items[MAX_ITEMS];
    void scroll(int8_t dir);
    ST7565 *_lcd;
    void (*_draw_function)(void);
    void (*_timeout_function)(void);
    bool _up_pressed, _down_pressed, _select_pressed, _has_draw_funct,
         _has_timeout_funct;
    uint8_t _up, _down, _select, _n_items, _item_index, _current_line,
            _up_hold_counter, _down_hold_counter;
    int8_t _draw_index;
    int _active_timeout;
    char _title[LABEL_LEN];

};

#endif /* LCDST7565_H_ */
