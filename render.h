#if !defined _RENDER_H_
#define _RENDER_H_

#include "common.h"

bool sdl_init(void);
void sdl_finalize(void);
void sdl_refresh(void);

void sdl_dot(int x, int y);
void sdl_wait(void);

#endif 
