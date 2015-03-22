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
#define UNIT_LEN 2
#define ST7565_MENU_VERSION "Menu UI v0.1"
//------------------


typedef enum
{
	VAL_TYPE_INT = 0,
	VAL_TYPE_FLOAT,
	VAL_TYPE_MIDI_CC,
	VAL_TYPE_MIDI_NOTE,
	VAL_TYPE_MIDI_CHANNEL
}synth_value_e;

typedef enum
{
	MENU_ITEM_TYPE_NORMAL,
	MENU_ITEM_TYPE_CHECK,
	MENU_ITEM_TYPE_RADIO
} menu_item_e;

typedef struct
{
	synth_value_e type;
	int min, max;
	int value;
	int step;
	float fstep;
	float fmin, fmax;
	float fvalue;
	int fdigits;
	char unit[UNIT_LEN];
	char name[LABEL_LEN];
	uint8_t incSpeed;
} synth_value_t;

class LCDST7565 : public ST7565 {
public:
	LCDST7565();

	// Struct used for all types of menu items
	typedef struct {
		menu_item_e type;
		char label[LABEL_LEN+1];
		int value, update_counter;
		bool pass_value, funct;
		bool checked;
		void (*function)(void);
		char* (*data_function)(void);
		void (LCDST7565::*value_function)(int);
	} MenuItem;



	void buttonEvent(int buttonIndex, button_event_e event);
	void drawMenuButton(gui_menubutton_e but, int slotIndex);

	void makeMenu(int m);
	void enterMenu(int m);

	void setSynthesizerInstance(Synthesizer *synthInstance) {synth = synthInstance; }
	void setMIDIConfig(midi_configuration_t *c) {midiConf = c;}

	void setBackgroundColor(int col);

	//===== Menu =====
    void update();
    void setMenuTitle(char *title);
    void addMenuItem(char *label, int value, void (LCDST7565::*function)(int));
    void addMenuItemCheckbox(char *label, int value, bool checked);
    void addMenuItemRadiobutton(char *label, int value);
    void drawMenu();
    void clearMenu();

private:
	void drawHome();
	void setSynthProperty(int value);
	void setOperatingMode(int value);
	void calibrateAntennas(int value);
	void menuBack();
	void menuUp();
	void menuDown();
	void menuSelect();
	void enterModulationMenu(int menu);
	void makeModulationMenu(int menu);
	void makeValueMenu(int menu);
	void enterValueMenu(int menu);
	void drawValueMenu();
	void valueMenuUp();
	void valueMenuDown();
	void updateValue();
	int centerString(char *c);
	void updateCheckbox(int val);
	void updateRadiobutton(int val);
	void selectRadioButton(int index);


	Synthesizer *synth;
	midi_configuration_t *midiConf;

	// selected indices
	int selectedPart;
	int selectedPartIndex;
	int currentMenu;
	synth_value_t cSynthVal;
	int menuHistory[MENU_HISTORY_SIZE];
	int itemIndexHistory[MENU_HISTORY_SIZE];
	int drawIndexHistory[MENU_HISTORY_SIZE];
	int clineHistory[MENU_HISTORY_SIZE];
	int historyPos;
	bool valueMenuActive;
	int valStepFactor;
	int valRepeatedCount;

	uint8_t contrast;

	//===== Menu =====
    MenuItem _menu_items[MAX_ITEMS];
    void scroll(int8_t dir);
    uint8_t _n_items, _item_index, _current_line, lastItemIndex;
    int8_t _draw_index, lastDrawIndex;
    char _title[LABEL_LEN];

};

#endif /* LCDST7565_H_ */
