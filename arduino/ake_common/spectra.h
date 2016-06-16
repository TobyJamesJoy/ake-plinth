#ifndef _SPECTRA_H
#define _SPECTRA_H

#include <stdlib.h>

#include "ake_common.h"

#define COLOURS 12
#define NUM_ROUTS 9

enum colours {
        R, O, Y, CH, G, GC, C, BC, B, V, M, RM
};

enum channels {
        RED, GREEN, BLUE
};

enum routines {
        OFFF, MONO, SPECTRUM, DIAD, TRIAD, TETRAD, 
        OUTSIDER, OUTSIDER_TRIAD, OUTSIDER_TETRAD
};

void def_palette(void);
void diad(int, int, int);
void get_clr(int, int, struct rgb *);
void mk_pix(int, int);
void mono(int, int, int);
void outsider(int, int, int);
void outsider_tetrad(int, int, int);
void outsider_triad(int, int, int);
void set_pix(struct rgb *, int);
void spectrum(int, int, int); 
void tetrad(int, int, int);
void triad(int, int, int);

#endif
