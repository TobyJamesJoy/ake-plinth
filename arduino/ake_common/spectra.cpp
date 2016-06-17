#include "spectra.h"

static unsigned *palette;

void
def_palette()
{
        unsigned i;
        double unit;
        
        palette = (unsigned *)malloc(sizeof(unsigned) * COLOURS);
        for (i = 1; i < COLOURS; ++i) {
	        unit = (((double)NUM_LEDS / (double)COLOURS) * (double)i);
                palette[i] = (unsigned)unit;
        }
        palette[0] = 0;
}

void
diad(int pix, int lum, int width)
{
        mono(pix, lum, width / 2);
        mono(pix + (NUM_LEDS / 2), lum, width / 2);
}

void 
get_clr(int pix, int lum, struct rgb *col)
{            
        if (pix < palette[O]) {
                col->r = lum;
        } else if (pix < palette[Y]) {
                col->r = lum;
                col->g = lum / 2;
        } else if (pix < palette[CH]) {
                col->r = lum;
                col->g = lum;
        } else if (pix < palette[G]) {
                col->r = lum / 2;
                col->g = lum;
        } else if (pix < palette[GC]) {
                col->g = lum;
        } else if (pix < palette[C]) {
                col->g = lum;
                col->b = lum / 2;
        } else if (pix < palette[BC]) {
                col->g = lum;
                col->b = lum;
        } else if (pix < palette[B]) {
                col->g = lum / 2;
                col->b = lum;
        } else if (pix < palette[V]) {
                col->b = lum;
        } else if (pix < palette[M]) {
                col->r = lum / 2;
                col->b = lum;
        } else if (pix < palette[RM]) {
                col->r = lum;
                col->b = lum;
        } else {
                col->r = lum;
                col->b = lum / 2;
        }
}

void
mk_pix(int pix, int lum)
{
        struct rgb *col = (struct rgb *)malloc(sizeof(struct rgb*));
        init_col(col);
 
        check_int(&pix, OFF, NUM_LEDS);       
        get_clr(pix, lum, col);
        set_pix(col, pix);
        free(col);        
}

void
mono(int pix, int lum, int width)
{
        struct rgb *col = (struct rgb *)malloc(sizeof(struct rgb *));
        init_col(col);
        
        get_clr(pix, lum, col);
        set_col(col, pix, width);
        free(col);
}

void
outsider(int pix, int lum, int width)
{
          int cnt, out;
          struct rgb *out_col = (struct rgb *)malloc(sizeof(struct rgb *));
          init_col(out_col);
          
          mono(pix, lum / 8, NUM_LEDS - 1);
          
          out = pix + (NUM_LEDS / 2);
          check_int(&out, OFF, (NUM_LEDS - 1));
          get_clr(out, lum, out_col);
                    
          out = rand();
          check_int(&out, OFF, (NUM_LEDS - 1));
          set_pix(out_col, out);
          free(out_col);         
}

void
outsider_tetrad(int pix, int lum, int width)
{
          int out0, out1, out2;
          unsigned cnt;
          struct rgb *out_col0 = (struct rgb *)malloc(sizeof(struct rgb *));
          struct rgb *out_col1 = (struct rgb *)malloc(sizeof(struct rgb *));
          struct rgb *out_col2 = (struct rgb *)malloc(sizeof(struct rgb *));
          
          init_col(out_col0);
          init_col(out_col1);
          init_col(out_col2);
          
          mono(pix, lum / 8, NUM_LEDS - 1);
          
          out0 = pix + (NUM_LEDS / 4);
          check_int(&out0, OFF, (NUM_LEDS - 1));
          get_clr(out0, lum, out_col0);
          
          out1 = pix + (NUM_LEDS / 2);
          check_int(&out1, OFF, (NUM_LEDS - 1));
          get_clr(out1, lum, out_col1);
          
          out2 = pix + (NUM_LEDS / 4);
          check_int(&out2, OFF, (NUM_LEDS - 1));
          get_clr(out2, lum, out_col2);
          
          out0 = rand();
          check_int(&out0, OFF, (NUM_LEDS - 1));
          
          do {
                  out1 = rand();
                  check_int(&out1, OFF, (NUM_LEDS - 1));
          } while (out1 == out0);
          
          do {
                  out2 = rand();
                  check_int(&out2, OFF, (NUM_LEDS - 1));
          } while (out2 == out0 || out2 == out1);
          
          set_pix(out_col0, out0);
          set_pix(out_col1, out1);
          set_pix(out_col2, out2);         
          
          free(out_col0);
          free(out_col1);
          free(out_col2);
}

void
outsider_triad(int pix, int lum, int width)
{
          int out0, out1;
          unsigned cnt;
          struct rgb *out_col0 = (struct rgb *)malloc(sizeof(struct rgb *));
          struct rgb *out_col1 = (struct rgb *)malloc(sizeof(struct rgb *));
          
          init_col(out_col0);
          init_col(out_col1);
          
          mono(pix, lum / 8, NUM_LEDS - 1);
          
          out0 = pix + (NUM_LEDS / 3);
          check_int(&out0, OFF, (NUM_LEDS - 1));
          get_clr(out0, lum, out_col0);
          
          out1 = pix - (NUM_LEDS / 3);
          check_int(&out1, OFF, (NUM_LEDS - 1));
          get_clr(out1, lum, out_col1);
          
          out0 = rand();
          check_int(&out0, OFF, (NUM_LEDS - 1));
          
          do {
                  out1 = rand();
                  check_int(&out1, OFF, (NUM_LEDS - 1));
          } while (out1 == out0);
          
          set_pix(out_col0, out0);
          set_pix(out_col1, out1);

          free(out_col0);
          free(out_col1);
}

void
spectrum(int pix, int lum, int width) 
{
        unsigned cnt;
        int pix1, pix2;
        
        
        pix1 = pix;
        pix2 = pix1;

        
        for (cnt = 0; cnt <= (width / 2) + 1; ++cnt, ++pix1, --pix2) {
                mk_pix(pix1, lum);
                mk_pix(pix2, lum);
        }
}

void
tetrad(int pix, int lum, int width)
{
        mono(pix, lum, width / 4);
        mono(pix + (NUM_LEDS / 4), lum, width / 4);
        mono(pix + (NUM_LEDS / 2), lum, width / 4);
        mono(pix - (NUM_LEDS / 4), lum, width / 4);
}

void
triad(int pix, int lum, int width)
{
        mono(pix, lum, width / 3);
        mono(pix + (NUM_LEDS / 3), lum, width / 3);
        mono(pix - (NUM_LEDS / 3), lum, width / 3);
}

