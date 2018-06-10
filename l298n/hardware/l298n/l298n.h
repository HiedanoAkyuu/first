
#ifndef __l298n_h
#define __l298n_h

#include <sys.h>

#define ENA PAout(4)
#define IN1 PAout(5)
#define IN2 PAout(6)

void l298n_roll(void);
void l298n_init(void);

#endif

