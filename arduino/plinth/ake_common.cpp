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
        double unit;

        if (*width == 1)
                return;
                
        if (*width == MIDI_MAX) {
                *width = NUM_LEDS;
                return;
        }

        /* don't bother if width is zero */
        if (*width) {
                unit = (double)NUM_LEDS / (double)(MIDI_MAX);
                *width = (int)(unit * *width);
                /* if given non-zero width make the minimum == 1 */
                if (*width == 0)
                        *width = 1;
        }
}

void
init_col(struct rgb *col)
{
        col->r = 0;
        col->g = 0;
        col->b = 0;
}

void
set_col(struct rgb *col, int pix, int width)
{
        unsigned cnt;
        int pix1;
        
        check_int(&pix, OFF, NUM_LEDS); 
        set_pix(col, pix) 

        /* don't bother with the rest if width == 1 */
        if (width == 1)
                return;
                
        pix1 = pix;
	++pix;
	--pix1;
       
        for (cnt = 0; cnt < width / 2; ++cnt, ++pix, --pix1) {
                check_int(&pix, OFF, NUM_LEDS);
                check_int(&pix1, OFF, NUM_LEDS);
                set_pix(col, pix);
                set_pix(col, pix1);
        }
}
