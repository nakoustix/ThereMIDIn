
#define GUI_BUTTON_WIDTH    32
#define GUI_BUTTON_HEIGHT   16
#define GUI_BUTTON_Y		48
#define GUI_SYNTHICON_WIDTH 24
#define GUI_SYNTHICON_HEIGHT 24

#define MENU_HISTORY_SIZE	6

#define GUI_VALBAR_X 14
#define GUI_VALBAR_Y 20
#define GUI_VALBAR_WIDTH 100
#define GUI_VALBAR_HALF	63
#define GUI_VALBAR_HEIGHT 8
#define GUI_VALBAR_STR_LINE	5
#define GUI_VALSTR_LINE 3

#define GUI_CHARS_PER_LINE 25
#define GUI_CHECKBOX_X 115
#define GUI_RADIO_X 118

#define GUI_INC_VAL_STEP 5

enum gui_bgcol_e
{
	BGCOL_RED,
	BGCOL_YELLOW,
	BGCOL_GREEN,
	BGCOL_CYAN,
	BGCOL_BLUE,
	BGCOL_VIOLET
};

enum gui_menu_e
{
	MENU_HOME,
	MENU_MAIN,
	MENU_DEBUG,
	MENU_MIDI,
	MENU_MIDI_RENOTE,
	MENU_MIDI_NOTEOFF,
	MENU_MIDI_ENABLE,
	MENU_MIDI_PATCH,
	MENU_MIDI_MUTE_PITCH,
	MENU_MIDI_MUTE_VOL,
	MENU_MIDI_CC_PITCH,
	MENU_MIDI_CC_VOL,
	MENU_MIDI_CC_STANDARD,
	MENU_MIDI_NOTE,
	MENU_MIDI_CHANNEL,
	MENU_MIDI_VELOCITY,
	MENU_ANTENNA_CALIB,
	MENU_DISP,
	MENU_DISP_COLOR,
	MENU_DISP_BRIGHT,
	MENU_DISP_CONTRAST,
	MENU_SYNTH,
	MENU_SYNTH_ENABLE,
	MENU_SYNTH_GAIN,
	MENU_SYNTH_BASEFREQ,
	MENU_SYNTH_SEMIRANGE,
	MENU_SYNTH_MATRIX,
	MENU_SYNTH_MOD_SRC_PITCH,
	MENU_SYNTH_MOD_SRC_VOL,
	MENU_SYNTH_OSC1,
	MENU_SYNTH_OSC2,
	MENU_SYNTH_OSC3,
	MENU_SYNTH_OSC_WAVEFORM,
	MENU_SYNTH_OSC_WTPOS,
	MENU_SYNTH_OSC_AMPLITUDE,
	MENU_SYNTH_OSC_SEMI,
	MENU_SYNTH_OSC_CENT,
	MENU_SYNTH_OSC_DUTY,
	MENU_SYNTH_FILTER,
	MENU_SYNTH_FILTER1,
	MENU_SYNTH_FILTER2,
	MENU_SYNTH_FILTER_SERPAR,
	MENU_SYNTH_FILTER1_FREQ,
	MENU_SYNTH_FILTER1_RESO,
	MENU_SYNTH_FILTER1_TYPE

};


typedef enum
{
	BUT_DOWN = 0,
	BUT_UP,
	BUT_BACK,
	BUT_OK,
	BUT_LEFT,
	BUT_RIGHT,
	BUT_SET,
	BUT_SYNTH,
	BUT_MENU,
} gui_menubutton_e;


typedef enum
{
	PRESS_EVENT,
	RELEASE_EVENT,
	REPEATED_PRESS_EVENT
} button_event_e;

typedef struct
{
	uint8_t buttonIndex;
	uint8_t event;
} button_event_t;



const unsigned char button_ok [] = {
0x00, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0x70, 0x70, 0x70, 0x70, 0xF0, 0xF0,
0xF0, 0x70, 0xF0, 0xF0, 0xF0, 0x70, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0x00,
0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0, 0xDF, 0xDF, 0xDF, 0xDF, 0xE0, 0xFF,
0xFF, 0xC0, 0xFB, 0xFD, 0xF2, 0xCF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00
};


const unsigned char button_menu [] = {
0x00, 0xFF, 0x07, 0xCF, 0x3F, 0xFF, 0x3F, 0xCF, 0x07, 0xFF, 0xFF, 0x07, 0xB7, 0xB7, 0xB7, 0xB7,
0xFF, 0xFF, 0x07, 0xCF, 0xBF, 0x7F, 0x07, 0xFF, 0xFF, 0x07, 0xFF, 0xFF, 0xFF, 0x07, 0xFF, 0x00,
0x00, 0x0F, 0x0C, 0x0F, 0x0F, 0x0C, 0x0F, 0x0F, 0x0C, 0x0F, 0x0F, 0x0C, 0x0D, 0x0D, 0x6D, 0x7D
};

const unsigned char button_back [] = {
0x00, 0xF0, 0xF0, 0x70, 0x70, 0x70, 0x70, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0x70, 0xF0, 0xF0, 0xF0,
0xF0, 0xF0, 0x70, 0x70, 0x70, 0xF0, 0xF0, 0xF0, 0x70, 0xF0, 0xF0, 0xF0, 0x70, 0xF0, 0xF0, 0x00,
0x00, 0xFF, 0xFF, 0xC0, 0xDB, 0xDB, 0xDB, 0xE4, 0xFF, 0xDF, 0xE3, 0xF4, 0xF7, 0xF4, 0xE3, 0xDF,
0xFF, 0xE0, 0xDF, 0xDF, 0xDF, 0xEE, 0xFF, 0xFF, 0xC0, 0xFB, 0xFD, 0xF2, 0xCF, 0xFF, 0xFF, 0x00
};

const unsigned char button_arrow_right [] = {
0x00, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
0xF0, 0xF0, 0xF0, 0xF0, 0x10, 0x10, 0x30, 0x70, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0x00,
0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF9, 0xF9, 0xF9, 0xF9, 0xF9, 0xF9, 0xF9, 0xF9, 0xF9, 0xF9,
0xF9, 0xF9, 0xF9, 0xF9, 0x80, 0x80, 0xC0, 0xE0, 0xF0, 0xF9, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00
};

const unsigned char button_arrow_left [] = {
0x00, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0x70, 0x30, 0x10, 0x10, 0xF0, 0xF0, 0xF0, 0xF0,
0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0x00,
0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF9, 0xF0, 0xE0, 0xC0, 0x80, 0x80, 0xF9, 0xF9, 0xF9, 0xF9,
0xF9, 0xF9, 0xF9, 0xF9, 0xF9, 0xF9, 0xF9, 0xF9, 0xF9, 0xF9, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00
};

const unsigned char button_arrow_up [] = {
0x00, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0x70,
0x70, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0x00,
0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xDF, 0xCF, 0xC7, 0xC3, 0xC1, 0xC0, 0xC0,
0xC0, 0xC0, 0xC1, 0xC3, 0xC7, 0xCF, 0xDF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00
};

const unsigned char button_arrow_down [] = {
		0x00, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70,
		0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0x00,
		0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFC, 0xF8, 0xF0, 0xE0, 0xC0,
		0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00
};

const unsigned char button_set [] = {
0x00, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0x70, 0x70, 0x70, 0xF0, 0xF0, 0xF0, 0x70, 0x70,
0x70, 0x70, 0x70, 0xF0, 0xF0, 0x70, 0x70, 0x70, 0x70, 0x70, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0x00,
0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xEC, 0xDB, 0xDB, 0xDB, 0xE6, 0xFF, 0xFF, 0xC0, 0xDB,
0xDB, 0xDB, 0xDB, 0xFF, 0xFF, 0xFF, 0xFF, 0xC0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00
};
