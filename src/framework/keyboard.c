#include "driver.h"
#include "window.h"
#include "keyboard.h"
#include "rc.h"

#import "pinmame_framework.h"

extern PinMAMEKeycodeStatusCallback g_keycode_status_callback;

static struct KeyboardInfo keylist[] = {
	{ "ESC",		KEY_ESC,	       KEYCODE_ESC },
    { "F1",         KEY_F1,            KEYCODE_F1 },
    { "F2",         KEY_F2,            KEYCODE_F2 },
    { "F3",         KEY_F3,            KEYCODE_F3 },
    { "F4",         KEY_F4,            KEYCODE_F4 },
    { "F5",         KEY_F5,            KEYCODE_F5 },
    { "F6",         KEY_F6,            KEYCODE_F6 },
    { "F7",         KEY_F7,            KEYCODE_F7 },
    { "F8",         KEY_F8,            KEYCODE_F8 },
    { "F9",         KEY_F9,            KEYCODE_F9 },
    { "F10",        KEY_F10,           KEYCODE_F10 },
    { "F11",        KEY_F11,           KEYCODE_F11 },
    { "F12",        KEY_F12,           KEYCODE_F12 },

    { "~",          KEY_TILDE,         KEYCODE_TILDE },
    { "1",          KEY_1,             KEYCODE_1 },
    { "2",          KEY_2,             KEYCODE_2 },
    { "3",          KEY_3,             KEYCODE_3 },
    { "4",          KEY_4,             KEYCODE_4 },
    { "5",          KEY_5,             KEYCODE_5 },
    { "6",          KEY_6,             KEYCODE_6 },
    { "7",          KEY_7,             KEYCODE_7 },
    { "8",          KEY_8,             KEYCODE_8 },
    { "9",          KEY_9,             KEYCODE_9 },
    { "0",          KEY_0,             KEYCODE_0 },
    { "-",          KEY_MINUS,         KEYCODE_MINUS },
    { "=",          KEY_EQUALS,        KEYCODE_EQUALS },
    { "DELETE",     KEY_BACKSPACE,     KEYCODE_BACKSPACE },
   
    { "HOME",       KEY_HOME,          KEYCODE_HOME },
    { "PGUP",       KEY_PGUP,          KEYCODE_PGUP },
   
    { "DEL",        KEY_DEL_PAD,       KEYCODE_DEL_PAD },
    { "/",          KEY_SLASH_PAD,     KEYCODE_SLASH_PAD },
    
    { "TAB",		KEY_TAB,		   KEYCODE_TAB },
    { "Q",			KEY_Q,			   KEYCODE_Q },
    { "W",			KEY_W,			   KEYCODE_W },
    { "E",			KEY_E,			   KEYCODE_E },
    { "R",			KEY_R,			   KEYCODE_R },
    { "T",			KEY_T,			   KEYCODE_T },
    { "Y",			KEY_Y,		   	   KEYCODE_Y },
    { "U",			KEY_U,			   KEYCODE_U },
    { "I",			KEY_I,			   KEYCODE_I },
    { "O",			KEY_O,			   KEYCODE_O },
    { "P",			KEY_P,			   KEYCODE_P },
    { "[",          KEY_OPENBRACE,     KEYCODE_OPENBRACE },
    { "]",          KEY_CLOSEBRACE,    KEYCODE_CLOSEBRACE },
    { "\\",         KEY_BACKSLASH,     KEYCODE_BACKSLASH },
    
    { "DEL",        KEY_DEL,           KEYCODE_DEL },
    { "END",        KEY_END,           KEYCODE_END },
    { "PGDN",       KEY_PGDN,          KEYCODE_PGDN },
    
    { "7",          KEY_7_PAD,         KEYCODE_7_PAD },
    { "8",          KEY_8_PAD,         KEYCODE_8_PAD },
    { "9",          KEY_9_PAD,         KEYCODE_9_PAD },
    { "-",          KEY_MINUS_PAD,     KEYCODE_MINUS_PAD },
   
    { "CAPS",       KEY_CAPSLOCK,      KEYCODE_CAPSLOCK },
    { "A",			KEY_A,			   KEYCODE_A },
    { "S",			KEY_S,			   KEYCODE_S },
    { "D",			KEY_D,			   KEYCODE_D },
    { "F",			KEY_F,			   KEYCODE_F },
 	{ "G",			KEY_G,			   KEYCODE_G },
    { "H",			KEY_H,			   KEYCODE_H },
    { "J",			KEY_J,			   KEYCODE_J },
    { "K",			KEY_K,			   KEYCODE_K },
    { "L",			KEY_L,		  	   KEYCODE_L },
    { ";",			KEY_COLON,	       KEYCODE_COLON },
    { "'",          KEY_QUOTE,         KEYCODE_QUOTE },
    { "ENTER" ,     KEY_ENTER,         KEYCODE_ENTER },
      
    { "4",          KEY_4_PAD   ,      KEYCODE_4_PAD },
    { "5",          KEY_5_PAD,         KEYCODE_5_PAD },
    { "6",          KEY_6_PAD,         KEYCODE_6_PAD },
    { "+",          KEY_PLUS_PAD,      KEYCODE_PLUS_PAD },
    
    { "SHIFT",		KEY_LSHIFT,		   KEYCODE_LSHIFT },
    { "Z",			KEY_Z,			   KEYCODE_Z },
    { "X",			KEY_X,			   KEYCODE_X },
 	{ "C",			KEY_C,			   KEYCODE_C },
    { "V",			KEY_V,			   KEYCODE_V },
    { "B",			KEY_B,			   KEYCODE_B },
    { "N",			KEY_N,			   KEYCODE_N },
    { "M",			KEY_M,			   KEYCODE_M },
    { ",",			KEY_COMMA,	       KEYCODE_COMMA },
    { ".",			KEY_STOP,	       KEYCODE_STOP },
    { "/",			KEY_SLASH,	       KEYCODE_SLASH },
    { "SHIFT",	    KEY_RSHIFT,		   KEYCODE_RSHIFT },

    { "UP",         KEY_UP,            KEYCODE_UP },

    { "1",          KEY_1_PAD,         KEYCODE_1_PAD },
    { "2",          KEY_2_PAD,         KEYCODE_2_PAD },
    { "3",          KEY_3_PAD,         KEYCODE_3_PAD },
    { "ENTER",      KEY_ENTER_PAD,     KEYCODE_ENTER_PAD },

    { "CONTROL",    KEY_LCONTROL,      KEYCODE_LCONTROL },
    { "ALT",		KEY_LALT,		   KEYCODE_LALT },
    { "COMMAND",	KEY_LWIN,		   KEYCODE_LWIN },
    { "SPACE",      KEY_SPACE,         KEYCODE_SPACE },
    { "COMMAND",	KEY_RWIN,		   KEYCODE_RWIN },
    { "ALT",		KEY_RALT,		   KEYCODE_RALT },
    { "CONTROL",	KEY_RCONTROL,      KEYCODE_RCONTROL },

    { "LEFT",       KEY_LEFT,          KEYCODE_LEFT },
    { "DOWN",       KEY_DOWN,          KEYCODE_DOWN },
    { "RIGHT",      KEY_RIGHT,         KEYCODE_RIGHT },
    
    { "0",          KEY_0_PAD,         KEYCODE_0_PAD },
    
 	{ 0, 0, 0 }	
};

volatile int trying_to_quit=0;

/**
 * osd_get_key_list
 */

const struct KeyboardInfo* osd_get_key_list(void) {
	return keylist;
}

/**
 * osd_is_key_pressed
 */

int osd_is_key_pressed(int keycode) {
    if (keycode >= KEY_MAX)
        return 0;

    /* special case: if we're trying to quit, fake up/down/up/down */
    if (keycode == KEY_ESC && trying_to_quit)
    {
        static int dummy_state = 1;
        return dummy_state ^= 1;
    }
    
    return g_keycode_status_callback ? g_keycode_status_callback(keycode) : 0;
}
