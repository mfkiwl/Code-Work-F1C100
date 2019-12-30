#ifndef CONTROL_H
#define CONTROL_H

#include "stdint.h"
#include "stdbool.h"

int PLAY(void);
int PAUSE(void);
int NEXT(void);
int PREVIOUS(void);
int ADD_VOLUME(void);
int SUB_VOLUME(void);
int SET_VOLUME(uint8_t);
int PLAY_OR_PAUSE(void);
int STOP(void);
int PLAY_OR_PAUSE(void);
void PLAYER_Init(void);


#endif
