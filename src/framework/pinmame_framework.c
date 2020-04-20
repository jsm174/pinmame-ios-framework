#include "pinmame_framework.h"

#include "driver.h"
#include "fileio.h"
#include "mame.h"
#include "core.h"
#include "vpintf.h"
#include "rc.h"
#include "sound.h"
#include "alias.h"
#include "config.h"

PinMAMELogCallback g_log_callback;
PinMAMEKeycodeStatusCallback g_keycode_status_callback;

extern unsigned char g_raw_dmdbuffer[DMD_MAXY * DMD_MAXX];
extern unsigned int g_raw_dmdx;
extern unsigned int g_raw_dmdy;
extern unsigned int g_needs_DMD_update;

volatile int g_fPause = 0;

char g_szGameName[256] = "";

extern int channels;

struct rc_struct *rc;

char g_vpmPath[2048];
int g_sampleRate = 48000;
int g_isGameReady = 0;
int vector_updates;

void pinmame_log(const char *fmt, ...) {
    if (g_log_callback) {
        va_list args;
        va_start(args, fmt);
        g_log_callback(fmt, args);
        va_end(args);
    }
}

int get_game_num(const char* name) {
    int gamenum = 0;
    while (drivers[gamenum]) {
        if (strcasecmp(drivers[gamenum]->name, name) == 0)
            break;
        gamenum++;
    }
    if (!drivers[gamenum])
        return -1;
    else
        return gamenum;
}

char* compose_path(char* path, char* file) {
    size_t pathl = strlen(path);
    size_t filel = strlen(file);
    char *r = (char*)malloc(pathl + filel + 4);
    
    strcpy(r, path);
    strcpy(r+pathl, file);
    return r;
}

int set_option(const char *name, const char *arg, int priority) {
    return rc_set_option(rc, name, arg, priority);
}

/**
 * Callbacks functions
 */

void pinmame_set_log_callback(PinMAMELogCallback callback) {
    g_log_callback = callback;
}

void pinmame_set_keycode_status_callback(PinMAMEKeycodeStatusCallback callback) {
    g_keycode_status_callback = callback;
}

/**
 * Setup functions
 */

void pinmame_set_vpm_path(const char* path) {
    strcpy(g_vpmPath, path);
    strcat(g_vpmPath, "/");
}

void pinmame_set_sample_rate(int sampleRate) {
    g_sampleRate = sampleRate;
}

/**
 * Game releated Functions
 */

int pinmame_start_game(const char* gameNameOrg, int showConsole) {
    rc = cli_rc_create();
    
    strcpy(g_szGameName, gameNameOrg);
    const char* const gameName = checkGameAlias(g_szGameName);
    
    const int game_index = get_game_num(gameName);
    
    if (game_index < 0) {
        return -1;
    }
    
    //options.skip_disclaimer = 1;
    //options.skip_gameinfo = 1;
    options.samplerate = g_sampleRate;
    
    g_fPause = 0;
    
    set_option("throttle", "1", 0);
    set_option("sleep", "1", 0);
    set_option("autoframeskip", "0", 0);
    set_option("skip_gameinfo", "1", 0);
    set_option("skip_disclaimer", "1", 0);
    
    pinmame_log("VPM path: %s\n", g_vpmPath);
    
    setPath(FILETYPE_ROM, compose_path(g_vpmPath, "roms"));
    setPath(FILETYPE_NVRAM, compose_path(g_vpmPath, "nvram"));
    setPath(FILETYPE_SAMPLE, compose_path(g_vpmPath, "samples"));
    setPath(FILETYPE_CONFIG, compose_path(g_vpmPath, "cfg"));
    setPath(FILETYPE_HIGHSCORE, compose_path(g_vpmPath, "hi"));
    setPath(FILETYPE_INPUTLOG, compose_path(g_vpmPath, "inp"));
    setPath(FILETYPE_MEMCARD, compose_path(g_vpmPath, "memcard"));
    setPath(FILETYPE_STATE, compose_path(g_vpmPath, "sta"));
    
    if (game_index != -1) {
        run_game(game_index);
    }
    
    return game_index;
}

void pinmame_stop_game(int locking) {
}

void pinmame_reset_game() {
    machine_reset();
}

int pinmame_is_game_ready() {
    return g_isGameReady;
}

/**
 * Pause related functions
 */

void pinmame_pause() {
    g_fPause = 1;
}

void pinmame_continue() {
    g_fPause = 0;
}

int pinmame_is_paused() {
    return g_fPause > 0;
}

/**
 * DMD related functions
 */

int pinmame_needs_dmd_update() {
    return !!g_needs_DMD_update;
}

int pinmame_get_raw_dmd_width() {
    return g_raw_dmdx;
}

int pinmame_get_raw_dmd_height() {
    return g_raw_dmdy;
}

int pinmame_get_raw_dmd_pixels(unsigned char* buffer) {
    if (g_raw_dmdx < 0 || g_raw_dmdy < 0)
        return -1;
    memcpy(buffer, g_raw_dmdbuffer, g_raw_dmdx*g_raw_dmdy * sizeof(unsigned char));
    return g_raw_dmdx*g_raw_dmdy;
}

/**
 * Audio related functions
 */

int pinmame_get_audio_channels() {
    return channels;
}


int pinmame_get_pending_audio_samples(float* buffer, int outChannels, int maxNumber) {
    return fillAudioBuffer(buffer, outChannels, maxNumber);
}

/**
 * Switch related functions
 */

int pinmame_get_switch(int slot) {
    return vp_getSwitch(slot) != 0;
}

void pinmame_set_switch(int slot, int state) {
    vp_putSwitch(slot, state ? 1 : 0);
}

/**
 * Lamps related functions
 */

int pinmame_get_max_lamps() {
    return CORE_MAXLAMPCOL * 8;
}

int pinmame_get_changed_lamps(int* changedStates) {
    vp_tChgLamps chgLamps;
    const int uCount = vp_getChangedLamps(chgLamps);
    
    int* out = changedStates;
    for (int i = 0; i < uCount; i++) {
        *(out++) = chgLamps[i].lampNo;
        *(out++) = chgLamps[i].currStat;
    }
    return uCount;
}

/**
 * Solenoids related functions
 */

int pinmame_get_max_solenoids() {
    return 64;
}

int pinmame_get_changed_solenoids(int* changedStates) {
    vp_tChgSols chgSols;
    const int uCount = vp_getChangedSolenoids(chgSols);
    
    int* out = changedStates;
    for (int i = 0; i < uCount; i++) {
        *(out++) = chgSols[i].solNo;
        *(out++) = chgSols[i].currStat;
    }
    return uCount;
}

/**
 * GI strings related functions
 */

int pinmame_get_max_gi_strings() {
    return CORE_MAXGI;
}

int pinmame_get_changed_gi_strings(int* changedStates) {
    vp_tChgGIs chgGIs;
    const int uCount = vp_getChangedGI(chgGIs);
    
    int* out = changedStates;
    for (int i = 0; i < uCount; i++) {
        *(out++) = chgGIs[i].giNo;
        *(out++) = chgGIs[i].currStat;
    }
    return uCount;
}

int pinmame_run_game(const char* gamename) {
    options.samplerate = 44100;
    options.gamma = 1;
    options.pause_bright= .65;
    options.brightness = 1;
    options.skip_disclaimer = 1;
    options.skip_gameinfo = 1;
    options.brightness = 1;
    
    pinmame_log("pinmame_run_game(): gamename=%s", gamename);
    
    int game_index = -1;
    
    if (game_index == -1) {
        for (int i = 0; drivers[i]; i++) {
            if (strcasecmp(gamename, (char*)drivers[i]->name) == 0) {
                game_index = i;
                break;
            }
        }
    }
    
    if (game_index != -1) {
        run_game(game_index);
    }
    
    return 0;
}

/**
 * OSD related functions
 */

int osd_init(void) {
    pinmame_log("osd_init\n");
    return 0;
}

void osd_exit(void) {
    pinmame_log("osd_exit\n");
}
