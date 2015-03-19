// Do not remove the include below
#include "Display_2.h"
#include "LCDST7565.h"
#include "Bounce.h"
#include "Synthesizer.h"
#include "globals.h"

Bounce up = Bounce(14, 5);
Bounce down = Bounce(3, 5);
Bounce back = Bounce(15, 5);
Bounce enter = Bounce(16, 5);

LCDST7565 lcd;
Synthesizer synth;
synth_preset_t synth_preset;
midi_configuration_t midiConf;
thereMIDIn_configuration_t config;

int lastTime = 0;
void setup()
{
	pinMode(3, INPUT_PULLUP);
	pinMode(14, INPUT_PULLUP);
	pinMode(15, INPUT_PULLUP);
	pinMode(16, INPUT_PULLUP);
	pinMode(13, OUTPUT);

	config.synthConf.masterGain = 0.45f;
	config.synthConf.baseFreq = 500;

	config.synthConf.preset.osc[OSC1].enabled = true;
	config.synthConf.preset.osc[OSC1].amplitude = 0.707f;
	config.synthConf.preset.osc[OSC1].waveform = SINE;
	config.synthConf.preset.osc[OSC1].wavetable_position = 0;
	config.synthConf.preset.osc[OSC1].duty = 0.5f;
	config.synthConf.preset.osc[OSC1].semitones = 0;
	config.synthConf.preset.osc[OSC1].cents = 0;

	config.synthConf.preset.osc[OSC2].enabled = false;
	config.synthConf.preset.osc[OSC2].amplitude = 0.3;
	config.synthConf.preset.osc[OSC2].waveform = SQUARE;
	config.synthConf.preset.osc[OSC2].wavetable_position = 0;
	config.synthConf.preset.osc[OSC2].duty = 0.5f;
	config.synthConf.preset.osc[OSC2].semitones = 0;
	config.synthConf.preset.osc[OSC2].cents = 0;

	config.synthConf.preset.osc[OSC3].enabled = false;
	config.synthConf.preset.osc[OSC3].amplitude = 0.6;
	config.synthConf.preset.osc[OSC3].waveform = SAWTOOTH;
	config.synthConf.preset.osc[OSC3].wavetable_position = 0;
	config.synthConf.preset.osc[OSC3].duty = 0.5f;
	config.synthConf.preset.osc[OSC3].semitones = 0;
	config.synthConf.preset.osc[OSC3].cents = 0;

	config.synthConf.preset.filter.dryWet = 1.f;
	config.synthConf.preset.filter.serParCF = 0.5f;
	config.synthConf.preset.filter.flt1.enabled = true;
	config.synthConf.preset.filter.flt1.frequency = 800.f;
	config.synthConf.preset.filter.flt1.resonance = 1.0f;
	config.synthConf.preset.filter.flt2.frequency = 1200.f;

	synth.setPreset(&config.synthConf.preset);

	config.midiConf.pitch.midi_cc = 0;
	config.midiConf.pitch.use14Bit = true;
	config.midiConf.volume.midi_cc = 1;
	config.midiConf.volume.use14Bit = true;

	config.operatingMode = OPMODE_SYNTH;
	operatingModeChanged();

	lcd.setSynthesizerInstance(&synth);
	lcd.setMIDIConfig(&config.midiConf);
	lcd.enterMenu(MENU_MAIN);
	lcd.update();
}

uint8_t led = 0xFF;
int lastPressUp = 0;
int lastPressDown = 0;
bool repeatUp = false;
bool repeatDown = false;
#define REPEAT_SPEED 80
#define REPEAT_DELAY 500
void loop()
{
	if(up.fallingEdge())
	{
		lcd.buttonEvent(BUT_UP, PRESS_EVENT);
		lastPressUp = millis();
	}
	else if(!up.read())
	{
		if(repeatUp && millis() - lastPressUp >= REPEAT_SPEED)
		{
			lcd.buttonEvent(BUT_UP, REPEATED_PRESS_EVENT);
			lastPressUp = millis();
		}
		else if(millis() - lastPressUp >= REPEAT_DELAY)
		{
			lastPressUp = millis();
			repeatUp = true;
		}
	}
	else if(up.risingEdge())
	{
		repeatUp = false;
	}


	if(down.fallingEdge())
	{
		lcd.buttonEvent(BUT_DOWN, PRESS_EVENT);
		lastPressDown = millis();
	}
	else if(!down.read())
	{
		if(repeatDown && millis() - lastPressDown >= REPEAT_SPEED)
		{
			lcd.buttonEvent(BUT_DOWN, REPEATED_PRESS_EVENT);
			lastPressDown = millis();
		}
		else if(millis() - lastPressDown >= REPEAT_DELAY)
		{
			lastPressDown = millis();
			repeatDown = true;
		}
	}
	else if(down.risingEdge())
	{
		repeatDown = false;
	}


	if(back.fallingEdge())
	{
		lcd.buttonEvent(BUT_BACK, PRESS_EVENT);
	}
	if(enter.fallingEdge())
	{
		lcd.buttonEvent(BUT_OK, PRESS_EVENT);
	}

	up.update();
	back.update();
	down.update();
	enter.update();

	if(millis() - lastTime >= 500)
	{
		digitalWrite(13, led);
		if(led)
		{
			led = 0;
		}
		else
		{
			led = 0xFF;
		}
	}
}

void midiConfigChanged()
{

}

void operatingModeChanged()
{
	if(config.operatingMode == OPMODE_MIDI)
	{
		synth.gotoSleep();
	}
	else
	{
		synth.enable(config.synthConf.masterGain);
		synth.noteOn();
		synth.setFrequency(config.synthConf.baseFreq);
	}
}
