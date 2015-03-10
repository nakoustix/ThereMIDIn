// Do not remove the include below
#include "Display_2.h"
#include "LCDST7565.h"
#include "Bounce.h"

Bounce left = Bounce(14, 5);
Bounce back = Bounce(3, 5);
Bounce right = Bounce(15, 5);
Bounce enter = Bounce(16, 5);

LCDST7565 lcd;

void setup()
{
	pinMode(3, INPUT_PULLUP);
	pinMode(14, INPUT_PULLUP);
	pinMode(15, INPUT_PULLUP);
	pinMode(16, INPUT_PULLUP);

	lcd.showMenu(MENU_SYNTH);
	lcd.update();
}

// The loop function is called in an endless loop
void loop()
{
	if(left.fallingEdge())
	{
		lcd.buttonEvent(2, PRESS_EVENT);
	}
	if(back.fallingEdge())
	{
		lcd.buttonEvent(1, PRESS_EVENT);
	}
	if(enter.fallingEdge())
	{
		lcd.buttonEvent(3, PRESS_EVENT);
	}

	left.update();
	back.update();
	right.update();
	enter.update();
}
