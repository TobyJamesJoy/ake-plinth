#ifndef _AKE_COMMON_H
#define _AKE_COMMON_H

#define BAUD_RATE 115200
#define CLOCKPIN 3
#define DATAPIN 2
#define MIDI_MAX 128
#define NUM_LEDS 34
#define OFF 0
#define TIMEOUT 5

typedef struct rgb {
        int r;
        int g;
        int b;
} rgb;

void check_int(int *, int, int, int);
void get_width(int *);
void init_col(struct rgb *);
void off(void);
void set_col(struct rgb *, int, int);
void set_pix(struct rgb *, int);

#endif
