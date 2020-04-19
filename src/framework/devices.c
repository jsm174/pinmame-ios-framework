#include "rc.h"

struct rc_option input_opts[] =
{
    { "Input device options", NULL, rc_seperator, NULL, NULL, 0, 0, NULL, NULL },
    { NULL,    NULL, rc_end, NULL, NULL, 0, 0,    NULL, NULL }
};

static struct JoystickInfo joylist[] = {
    { 0, 0, 0 }
};

/**
 * osd_get_joy_list
 */

const struct JoystickInfo* osd_get_joy_list(void) {
    return joylist;
}

/**
 * osd_customize_inputport_defaults
 */

void osd_customize_inputport_defaults(struct ipd *defaults) {
}

/**
 * osd_analogjoy_read
 */

void osd_analogjoy_read(int player, int analog_axis[MAX_ANALOG_AXES], InputCode analogjoy_input[MAX_ANALOG_AXES]) {
}

/**
 * osd_trak_read
 */

void osd_trak_read(int player, int *deltax, int *deltay) {
}

/**
 * osd_lightgun_read
 */

void osd_lightgun_read(int player, int *deltax, int *deltay) {
}

/**
 * osd_is_joy_pressed
 */

int osd_is_joy_pressed(int joycode) {
    return 0;
}

/**
 * osd_is_joystick_axis_code
 */

int osd_is_joystick_axis_code(int joycode) {
    return 0;
}

/**
 * osd_joystick_needs_calibration
 */

int osd_joystick_needs_calibration(void) {
    return 0;
}

/**
 * osd_joystick_start_calibration
 */

void osd_joystick_start_calibration(void) {
}

/**
 * osd_joystick_calibrate
 */

void osd_joystick_calibrate(void) {
}

/**
 * osd_joystick_calibrate_next
 */

const char* osd_joystick_calibrate_next(void) {
    return NULL;
}

/**
 * osd_joystick_end_calibration
 */

void osd_joystick_end_calibration(void) {
}
