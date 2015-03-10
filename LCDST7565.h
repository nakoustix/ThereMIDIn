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


class LCDST7565 : public ST7565 {
public:
	LCDST7565();

	void drawMenuButton(gui_menubutton_e but, int slotIndex);
};

#endif /* LCDST7565_H_ */
