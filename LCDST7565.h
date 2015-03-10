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
#include "ST7565_Menu.h"
#include "Synthesizer.h"
#include "globals.h"


class LCDST7565 : public ST7565 {
public:
	LCDST7565();

	void drawMenuButton(gui_menubutton_e but, int slotIndex);

	void showMenu(int m);

	void setSynthesizerInstance(Synthesizer *synthInstance) {synth = synthInstance; }

private:
	void drawHome();
	void setSynthOscWaveform(int waveform);
	//void (*synthValueCallback) (int synthPart, int newValue);

	Menu *menu;
	Synthesizer *synth;

	// selected indices
	int selectedPart;
};

#endif /* LCDST7565_H_ */
