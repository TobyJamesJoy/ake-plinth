#ifndef _AKE_COMMON_H
#define _AKE_COMMON_H

/*
 * determines how to handle over/underflow in check_int() 
 * 0: crunch to min/max 
 * 1: modulo
 */
#define MOD_OVERFLOW 1

#define BAUD_RATE 115200
#define CLOCKPIN 3
#define DATAPIN 2
#define MIDI_MAX 127
#define NUM_LEDS 128
#define OFF 0
#define OUTPIN 13
#define TIMEOUT 5

typedef struct rgb {
        int r;
        int g;
        int b;
} rgb;

void check_int(int *, int, int);
void get_width(int *);
void off(void);
void set_col(struct rgb *, int, int);
void set_pix(struct rgb*, int);

#endif
