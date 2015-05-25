// Do not remove the include below
#include "Display_2.h"
#include "LCDST7565.h"
#include "Bounce.h"
#include "Synthesizer.h"
#include "globals.h"
#include "MidiInterface.h"

Bounce up = Bounce(14, 12);
Bounce down = Bounce(3, 12);
Bounce back = Bounce(15, 12);
Bounce enter = Bounce(16, 12);

LCDST7565 lcd;
Synthesizer synth;
MidiInterface midi;
synth_preset_t synth_preset;
midi_configuration_t midiConf;
thereMIDIn_configuration_t config;

#define ADC_MAX		4095.f
#define ADC_HALF	2048.f
#define ADC_PIN_PITCH		2
#define ADC_PIN_VOLUME		3

#define MIDI_UPDATE_DEL 30

int pitchVal = 0;
int lastPitchVal = 0;
int volVal = 0;
int lastVolVal = 0;
int lastMidiUpdate = 0;

int lastTime = 0;
void setup()
{
	// Configure pins
	pinMode(3, INPUT_PULLUP);
	pinMode(14, INPUT_PULLUP);
	pinMode(15, INPUT_PULLUP);
	pinMode(16, INPUT_PULLUP);
	pinMode(13, OUTPUT);

	// === Initialize default configuration ===
	config.synthConf.masterGain = 0.45f;
	config.synthConf.baseFreq = 500;
	config.synthConf.enabled = true;

	// OSC 1
	config.synthConf.preset.osc[OSC1].enabled = true;
	config.synthConf.preset.osc[OSC1].amplitude = 0.707f;
	config.synthConf.preset.osc[OSC1].waveform = SINE;
	config.synthConf.preset.osc[OSC1].wavetable_position = 0;
	config.synthConf.preset.osc[OSC1].duty = 0.5f;
	config.synthConf.preset.osc[OSC1].semitones = 0;
	config.synthConf.preset.osc[OSC1].cents = 0;

	// OSC 2
	config.synthConf.preset.osc[OSC2].enabled = false;
	config.synthConf.preset.osc[OSC2].amplitude = 0.3;
	config.synthConf.preset.osc[OSC2].waveform = SQUARE;
	config.synthConf.preset.osc[OSC2].wavetable_position = 0;
	config.synthConf.preset.osc[OSC2].duty = 0.5f;
	config.synthConf.preset.osc[OSC2].semitones = 0;
	config.synthConf.preset.osc[OSC2].cents = 0;

	// OSC 3
	config.synthConf.preset.osc[OSC3].enabled = false;
	config.synthConf.preset.osc[OSC3].amplitude = 0.6;
	config.synthConf.preset.osc[OSC3].waveform = SAWTOOTH;
	config.synthConf.preset.osc[OSC3].wavetable_position = 0;
	config.synthConf.preset.osc[OSC3].duty = 0.5f;
	config.synthConf.preset.osc[OSC3].semitones = 0;
	config.synthConf.preset.osc[OSC3].cents = 0;

	// Filter
	config.synthConf.preset.filter.dryWet = 1.f;
	config.synthConf.preset.filter.serParCF = 0.5f;
	config.synthConf.preset.filter.flt1.enabled = true;
	config.synthConf.preset.filter.flt1.frequency = 800.f;
	config.synthConf.preset.filter.flt1.resonance = 1.0f;
	config.synthConf.preset.filter.flt2.frequency = 1200.f;

	// Set the synth configuration
	synth.setConfiguration( &config.synthConf );

	// MIDI
	config.midiConf.antenna[CT_PITCH].cc = 0;
	config.midiConf.antenna[CT_PITCH].use14Bit = true;
	config.midiConf.antenna[CT_PITCH].muted = true;
	config.midiConf.antenna[CT_VOLUME].cc = 1;
	config.midiConf.antenna[CT_VOLUME].use14Bit = false;
	config.midiConf.antenna[CT_VOLUME].muted = true;
	config.midiConf.patch = 0;
	config.midiConf.baseNote = 60;
	config.midiConf.velocity = 100;
	config.midiConf.enabled = true;

	// Set the MIDI configuration
	midi.setConfiguration( &config.midiConf );

	lcd.setSynthesizerInstance( &synth );
	lcd.setMidiInstance( &midi );
	lcd.enterMenu( MENU_MAIN );
	lcd.update();
}

uint8_t led = 0xFF;
int lastPressUp = 0;
int lastPressDown = 0;
bool repeatUp = false;
bool repeatDown = false;
int lastPressBack = 0;
int lastPressOk = 0;
bool repeatBack = false;
bool repeatOk = false;
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
		lcd.buttonEvent(BUT_UP, RELEASE_EVENT);
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
		lcd.buttonEvent(BUT_DOWN, RELEASE_EVENT);
	}


	if(back.fallingEdge())
	{
		lcd.buttonEvent(BUT_BACK, PRESS_EVENT);
		lastPressBack = millis();
	}
	else if(!back.read())
	{
		if(repeatBack && millis() - lastPressBack >= REPEAT_SPEED)
		{
			lcd.buttonEvent(BUT_BACK, REPEATED_PRESS_EVENT);
			lastPressBack = millis();
		}
		else if(millis() - lastPressBack >= REPEAT_DELAY)
		{
			lastPressBack = millis();
			repeatBack = true;
		}
	}
	else if(back.risingEdge())
	{
		repeatBack = false;
		lcd.buttonEvent(BUT_BACK, RELEASE_EVENT);
	}


	if(enter.fallingEdge())
	{
		lcd.buttonEvent(BUT_OK, PRESS_EVENT);
		lastPressOk = millis();
	}
	else if(!enter.read())
	{
		if(repeatOk && millis() - lastPressOk >= REPEAT_SPEED)
		{
			lcd.buttonEvent(BUT_OK, REPEATED_PRESS_EVENT);
			lastPressOk = millis();
		}
		else if(millis() - lastPressOk >= REPEAT_DELAY)
		{
			lastPressOk = millis();
			repeatOk = true;
		}
	}
	else if(enter.risingEdge())
	{
		repeatOk = false;
		lcd.buttonEvent(BUT_OK, RELEASE_EVENT);
	}

	up.update();
	back.update();
	down.update();
	enter.update();

	if(millis() - lastMidiUpdate > MIDI_UPDATE_DEL)
	{
		lastMidiUpdate = millis();
		pitchVal = analogRead(ADC_PIN_PITCH);
		if( pitchVal != lastPitchVal )
		{
			float offset = (float) (pitchVal - ADC_HALF) / ADC_MAX;
			lastPitchVal = pitchVal;
			midi.setOffset( CT_PITCH, offset);
			synth.setOffset( CT_PITCH, offset);
		}
		volVal = analogRead(ADC_PIN_VOLUME);
		if( volVal != lastVolVal )
		{
			float offset = (float) (volVal - ADC_HALF) / ADC_MAX;
			lastVolVal = volVal;
			midi.setOffset( CT_VOLUME, offset);
			synth.setOffset( CT_VOLUME, offset);
		}
	}
}
