
#define GUI_BUTTON_WIDTH    32
#define GUI_BUTTON_HEIGHT   12
#define GUI_BUTTON_Y		52
#define GUI_SYNTHICON_WIDTH 24
#define GUI_SYNTHICON_HEIGHT 24

enum gui_menu_e
{
	MENU_HOME,
	MENU_MAIN,
	MENU_SYNTH,
	MENU_SYNTH_OSC1,
	MENU_SYNTH_OSC2,
	MENU_SYNTH_OSC3,
	MENU_SYNTH_OSC_WAVEFORM,
	MENU_SYNTH_OSC_WTPOS,
	MENU_SYNTH_OSC_AMPLITUDE,
	MENU_SYNTH_FILTER,
	MENU_SYNTH_FILTER1,
	MENU_SYNTH_FILTER2,

};


typedef enum
{
	BUT_OK,
	BUT_BACK,
	BUT_SYNTH,
	BUT_MENU,
	BUT_LEFT,
	BUT_RIGHT,
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


const unsigned char synthicon_filter [] = {
0xF0, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xC0, 0xE0, 0x60,
0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x80, 0xF0, 0xF8,
0x1C, 0x0E, 0x07, 0x03, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x1F, 0x1F, 0x18, 0x18, 0x18, 0x1F, 0x1F, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 
};

const unsigned char button_synth [] = {
0x00, 0xFF, 0xCF, 0xB7, 0xB7, 0xB7, 0x6F, 0xFF, 0xE7, 0xDF, 0x3F, 0xDF, 0xE7, 0xFF, 0x07, 0xCF,
0xBF, 0x7F, 0x07, 0xF7, 0xF7, 0x07, 0xF7, 0xF7, 0x07, 0xBF, 0xBF, 0xBF, 0x07, 0xFF, 0xFF, 0x00,
0x00, 0x0F, 0x0E, 0x0D, 0x0D, 0x0D, 0x0E, 0x0F, 0x0F, 0x0F, 0x0C, 0x0F, 0x6F, 0x0F, 0x6C, 0x6F
};

const unsigned char button_ok [] = {
0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0xF7, 0xF7, 0xF7, 0xF7, 0x0F, 0xFF,
0xFF, 0x07, 0xBF, 0xDF, 0x2F, 0xF7, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00,
0x00, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0E, 0x0D, 0x0D, 0x0D, 0x0D, 0x7E, 0x6F
};


const unsigned char button_menu [] = {
0x00, 0xFF, 0x07, 0xCF, 0x3F, 0xFF, 0x3F, 0xCF, 0x07, 0xFF, 0xFF, 0x07, 0xB7, 0xB7, 0xB7, 0xB7,
0xFF, 0xFF, 0x07, 0xCF, 0xBF, 0x7F, 0x07, 0xFF, 0xFF, 0x07, 0xFF, 0xFF, 0xFF, 0x07, 0xFF, 0x00,
0x00, 0x0F, 0x0C, 0x0F, 0x0F, 0x0C, 0x0F, 0x0F, 0x0C, 0x0F, 0x0F, 0x0C, 0x0D, 0x0D, 0x6D, 0x7D
};

const unsigned char button_back [] = {
0x00, 0xFF, 0xFF, 0x07, 0xB7, 0xB7, 0xB7, 0x4F, 0xFF, 0xFF, 0x3F, 0x4F, 0x77, 0x4F, 0x3F, 0xFF,
0xFF, 0x0F, 0xF7, 0xF7, 0xF7, 0xEF, 0xFF, 0xFF, 0x07, 0xBF, 0xDF, 0x2F, 0xF7, 0xFF, 0xFF, 0x00,
0x00, 0x0F, 0x0F, 0x0C, 0x0D, 0x0D, 0x0D, 0x0E, 0x0F, 0x0D, 0x0E, 0x0F, 0x0F, 0x0F, 0x6E, 0x7D
};

const unsigned char button_arrow_right [] = {
0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x9F, 0x9F, 0x9F, 0x9F, 0x9F, 0x9F, 0x9F, 0x9F, 0x9F, 0x9F,
0x9F, 0x9F, 0x9F, 0x9F, 0x01, 0x01, 0x03, 0x07, 0x0F, 0x9F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00,
0x00, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x7F, 0x5F, 0x7F, 0x6F
};

const unsigned char button_arrow_left [] = {
0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x9F, 0x0F, 0x07, 0x03, 0x01, 0x01, 0x9F, 0x9F, 0x9F, 0x9F,
0x9F, 0x9F, 0x9F, 0x9F, 0x9F, 0x9F, 0x9F, 0x9F, 0x9F, 0x9F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00,
0x00, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0E, 0x0C, 0x08, 0x08, 0x7F, 0x5F, 0x6F, 0x6F
};

const unsigned char synthicon_osc [] = {
0x00, 0x00, 0x80, 0xC0, 0xE0, 0x60, 0x30, 0x30, 0x70, 0xE0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x3C, 0x0F, 0x03, 0x00, 0x00, 0x00, 0x00,
0x00, 0x01, 0x3F, 0xFE, 0xF0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x70,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x0F, 0x1F, 0x38, 0x38,
0x60, 0x60, 0x60, 0x30, 0x3C, 0x0F, 0x07, 0x00, 
};
