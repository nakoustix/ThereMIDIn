// Do not remove the include below
#include "Display_2.h"
#include "LCDST7565.h"
#include "Bounce.h"
#include "Synthesizer.h"

Bounce left = Bounce(14, 5);
Bounce back = Bounce(3, 5);
Bounce right = Bounce(15, 5);
Bounce enter = Bounce(16, 5);

LCDST7565 lcd;
Synthesizer synth;
synth_preset_t synth_preset;

void setup()
{
	pinMode(3, INPUT_PULLUP);
	pinMode(14, INPUT_PULLUP);
	pinMode(15, INPUT_PULLUP);
	pinMode(16, INPUT_PULLUP);


	synth_preset.osc[OSC1].amplitude = 0.707f;
	synth_preset.osc[OSC1].waveform = SINE;
	synth_preset.osc[OSC1].wavetable_position = 0;
	synth_preset.osc[OSC1].duty = 0.5f;
	synth_preset.osc[OSC1].semitones = 0;
	synth_preset.osc[OSC1].cents = 0;

	synth_preset.osc[OSC2].amplitude = 0.3;
	synth_preset.osc[OSC2].waveform = SQUARE;
	synth_preset.osc[OSC2].wavetable_position = 0;
	synth_preset.osc[OSC2].duty = 0.5f;
	synth_preset.osc[OSC2].semitones = 0;
	synth_preset.osc[OSC2].cents = 0;

	synth_preset.osc[OSC3].amplitude = 0.6;
	synth_preset.osc[OSC3].waveform = SAWTOOTH;
	synth_preset.osc[OSC3].wavetable_position = 0;
	synth_preset.osc[OSC3].duty = 0.5f;
	synth_preset.osc[OSC3].semitones = 0;
	synth_preset.osc[OSC3].cents = 0;

	synth_preset.filter.dryWet = 1.f;
	synth_preset.filter.serParCF = 1.f;
	synth_preset.filter.flt1.frequency = 800;
	synth_preset.filter.flt2.frequency = 1200;

	synth.setPreset(&synth_preset);

	/*synth.enable(0.4f);
	synth.noteOn();
	synth.setFrequency(500);*/

	lcd.setSynthesizerInstance(&synth);
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
