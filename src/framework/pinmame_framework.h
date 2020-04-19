#ifndef __PINMAME_FRAMEWORK_H_
#define __PINMAME_FRAMEWORK_H_

#include <stdarg.h>

typedef void (*PinMAMELogCallback)(const char *fmt, va_list arg);
void pinmame_set_log_callback(PinMAMELogCallback);

typedef int (*PinMAMEKeycodeStatusCallback)(int);
void pinmame_set_keycode_status_callback(PinMAMEKeycodeStatusCallback);

void pinmame_set_vpm_path(const char* path);
void pinmame_set_sample_rate(int sampleRate);

int pinmame_start_game(const char* gameName, int showConsole);
void pinmame_stop_game(int locking);
void pinmame_reset_game(void);
int pinmame_is_game_ready(void);

void pinmame_pause(void);
void pinmame_continue(void);
int pinmame_is_paused(void);

int pinmame_needs_dmd_update(void);
int pinmame_get_raw_dmd_width(void);
int pinmame_get_raw_dmd_height(void);
int pinmame_get_raw_dmd_pixels(unsigned char* buffer);
    
int pinmame_get_audio_channels(void);
int pinmame_get_pending_audio_samples(float* buffer, int outChannels, int maxNumber);
    
int pinmame_get_switch(int slot);
void pinmame_set_switch(int slot, int state);

int pinmame_get_max_lamps(void);
int pinmame_get_changed_lamps(int* changedStates);

int pinmame_get_max_solenoids(void);
int pinmame_get_changed_solenoids(int* changedStates);

int pinmame_get_max_gi_strings(void);
int pinmame_get_changed_gi_strings(int* changedStates);

#endif /* __PINMAME_FRAMEWORK_H_ */
