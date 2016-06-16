#include "ake_common.h"

void
check_int(int *i, int minimum, int maximum)
{
#if MOD_OVERFLOW
        while (*i < minimum)
                *i += maximum;
        *i = *i % maximum;
#else         
        if (*i < minimum) {
                *i = minimum;
                return;
        }
        
        if (*i > maximum)
                *i = maximum;
#endif
}

void
get_width(int *width)
{
        double unit = (double)NUM_LEDS / (double)(MIDI_MAX + 1);
        *width *= (int)unit;
}

void
set_col(struct rgb *col, int pix, int width)
{
        unsigned cnt;
        int pix1;
        
        check_int(&pix, OFF, NUM_LEDS);
        pix1 = pix;
        set_pix(col, pix);
        
        for (cnt = 0; cnt < width / 2; ++cnt, ++pix, --pix1) {
                check_int(&pix, OFF, NUM_LEDS);
                check_int(&pix1, OFF, NUM_LEDS);
                set_pix(col, pix);
                set_pix(col, pix1); 
        }
}
