#ifndef __CONFIG_H_
#define __CONFIG_H_


#include "rc.h"

// Initializes and exits the configuration system
int  cli_frontend_init (int argc, char **argv);
void cli_frontend_exit (void);

// Creates an RC object
struct rc_struct *cli_rc_create(void);


#endif /* __CONFIG_H_ */