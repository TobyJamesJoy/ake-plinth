#include "LPD8806.h"
#include "SPI.h"


#define BAUD_RATE 115200
#define CHANNELS 3
#define CLOCKPIN 3
#define COLOURS 12
#define DATAPIN 2
#define MIDI_MAX 127
#define NUM_LEDS 34
#define OFF 0
#define OUTPIN 13
#define TIMEOUT 5

LPD8806 strip = LPD8806(NUM_LEDS, DATAPIN, CLOCKPIN);

enum colours {
        R, O, Y, CH, G, GC, C, BC, B, V, M, RM
};

enum channels {
        RED, GREEN, BLUE
};

enum routines {
        OFFF, MONO, SPECTRUM, DOUBLE_SPECTRUM, DIAD, TRIAD, TETRAD, 
        OUTSIDER, OUTSIDER_TRIAD, OUTSIDER_TETRAD
};

unsigned palette[COLOURS];

void 
setup() 
{
        // Start up the LED strip
        strip.begin();
        // Update the strip, to start they are all 'off'
        strip.show();

        pinMode(OUTPIN, OUTPUT);

        // opens serial port, sets data rate to 9600 bps
        Serial.begin(BAUD_RATE);
        Serial.setTimeout(TIMEOUT);

        def_palette(palette);
}
        
void 
loop() {
        unsigned rout, lum, width;
        int pix;

        while (Serial.available() > OFF) {
                /* 
                 * look for the next valid integer in the incoming 
                 * serial stream
                 */
                rout = Serial.parseInt(); //color
                Serial.write(rout);
               
                lum = Serial.parseInt(); //luminescence
                Serial.write(lum);
                
                pix = Serial.parseInt(); //pixel
                Serial.write(pix);
                
                
                width = Serial.parseInt(); //width
                Serial.write(width);
                width = get_width(width);
                
                switch (rout) {
                case OFF:
                        off();
                        break;
                case MONO:
                        mono(pix, lum, width);
                        strip.show();
                        break;
                case SPECTRUM:
                        spectrum(pix, lum, width);
                        break;
                case DOUBLE_SPECTRUM:
                        spectrum_double_chase(pix, lum, width);
                        break;
                case DIAD:
                        diad(pix, lum, width);
                        break;
                case TRIAD:
                        triad(pix, lum, width);
                        break;
                case TETRAD:
                        tetrad(pix, lum, width);
                        break;
                case OUTSIDER:
                        outsider(pix, lum, width);
                        break;
                case OUTSIDER_TRIAD:
                        outsider_triad(pix, lum, width);
                        break;
                case OUTSIDER_TETRAD:
                        outsider_tetrad(pix, lum, width);
                }

        }
}

void
def_palette(unsigned palette[COLOURS])
{
        unsigned i;
        for (i = 1; i < COLOURS; ++i) 
	        palette[i] = (NUM_LEDS / COLOURS) * i;
        palette[0] = 0;
}

unsigned
get_width(unsigned width)
{
        double unit = (double)NUM_LEDS / (double)MIDI_MAX;
        width = (unsigned)(unit * width);
        if (width > 0)
                return width;
        else
                return 1;
}

void
off(void)
{
      unsigned cnt;
      
      for (cnt = 0; cnt < NUM_LEDS; ++cnt)
              strip.setPixelColor(cnt, OFF);
}

void
set_clr(int pix, unsigned clr[CHANNELS])
{
        strip.setPixelColor(pix, strip.Color(clr[RED], clr[GREEN], clr[BLUE]));
}

void get_clr(int pix, unsigned lum, unsigned clr[CHANNELS])
{          
  
        if (pix < palette[O]) {
                clr[RED] = lum;
        } else if (pix < palette[Y]) {
                clr[RED] = lum;
                clr[GREEN] = lum / 2;
        } else if (pix < palette[CH]) {
                clr[RED] = lum;
                clr[GREEN] = lum;
        } else if (pix < palette[G]) {
                clr[RED] = lum / 2;
                clr[GREEN] = lum;
        } else if (pix < palette[GC]) {
                clr[GREEN] = lum;
        } else if (pix < palette[C]) {
                clr[GREEN] = lum;
                clr[BLUE] = lum / 2;
        } else if (pix < palette[BC]) {
                clr[GREEN] = lum;
                clr[BLUE] = lum;
        } else if (pix < palette[B]) {
                clr[GREEN] = lum / 2;
                clr[BLUE] = lum;
        } else if (pix < palette[V]) {
                clr[BLUE] = lum;
        } else if (pix < palette[M]) {
                clr[RED] = lum / 2;
                clr[BLUE] = lum;
        } else if (pix < palette[RM]) {
                clr[RED] = lum;
                clr[BLUE] = lum;
        } else {
                clr[RED] = lum;
                clr[BLUE] = lum / 2;
        }
}

void
mk_pix(int pix, unsigned lum)
{
        unsigned clr[CHANNELS] = {OFF, OFF, OFF};
 
        pix = check_pix(pix);       
        get_clr(pix, lum, clr);
        set_clr(pix, clr);        
}

void
mono(int pix, unsigned lum, unsigned width)
{
        int cnt;
        unsigned clr[CHANNELS] = {OFF, OFF, OFF};
        
        get_clr(pix, lum, clr);
        
        off();
        for (cnt = 0; cnt < width; ++cnt, ++pix) {
                set_clr(pix, clr);
        }
}

void
spectrum(int pix, unsigned lum, unsigned width) 
{
        unsigned cnt;
        
        off();
        for (cnt = pix; cnt < (pix + width); ++cnt)
                mk_pix(cnt, lum);
        strip.show();
}

void
spectrum_double_chase(int pix, unsigned lum, unsigned width)
{
        unsigned cnt;
        int pix1, pix2;
        
        
        pix1 = pix;
        pix2 = pix1;

        off();
        
        for (cnt = 0; cnt < width / 2; ++cnt, ++pix1, --pix2) {
                mk_pix(pix1, lum);
                mk_pix(pix2, lum);
        }
        strip.show();
}

int
check_pix(int pix)
{
        return pix % NUM_LEDS;
}

void
diad(int pix, unsigned lum, unsigned width)
{
        unsigned cnt;
        int di = pix + NUM_LEDS / 2;

        off();
        for (cnt = 0; cnt < width / 2; ++cnt, ++pix, ++ di) {
                mk_pix(pix, lum);
                mk_pix(di, lum);
        }
        strip.show();
}

void 
mk_triad(int pix, unsigned lum, unsigned width, unsigned offset) 
{
        int pint = pix;

        mk_pix(pix, lum);
        pint = check_pix(pint + offset);
        mk_pix(pint, lum);
        pint = check_pix(pix - offset);
        mk_pix(pint, lum);
        strip.show();
}

void
triad(int pix, unsigned lum, unsigned width)
{
        unsigned cnt;
        
        for (cnt = 0; cnt < NUM_LEDS; ++cnt)
                strip.setPixelColor(cnt, OFF);        

        mk_triad(pix, lum, width, NUM_LEDS / 3);
}

void
tetrad(int pix, unsigned lum, unsigned width)
{
        unsigned cnt;

        for (cnt = 0; cnt < NUM_LEDS; ++cnt)
                strip.setPixelColor(cnt, OFF);        
        
        mk_pix(pix, lum);
        
        pix += (NUM_LEDS / 4);
        pix = check_pix(pix);
        mk_pix(pix, lum);
        
        pix += (NUM_LEDS / 2);
        pix = check_pix(pix);
        mk_pix(pix, lum);
        
        pix -= (NUM_LEDS / 4);
        pix = check_pix(pix);
        mk_pix(pix, lum);
        
        strip.show();
}

void
outsider(int pix, unsigned lum, unsigned width)
{
          unsigned cnt, out;
          unsigned in_clr[CHANNELS] = {OFF, OFF, OFF};
          unsigned out_clr[CHANNELS] = {OFF, OFF, OFF};
          
          mono(pix, lum / 8, MIDI_MAX);
          
          out = check_pix(pix + NUM_LEDS / 2);
          get_clr(out, lum, out_clr);
                    
          out = random(NUM_LEDS);
          strip.setPixelColor(out, out_clr[RED], out_clr[GREEN], out_clr[BLUE]);         
          strip.show();
}

void
outsider_triad(int pix, unsigned lum, unsigned width)
{
          int out0, out1;
          unsigned cnt;
          unsigned in_clr[CHANNELS] = {OFF, OFF, OFF};
          unsigned out_clr0[CHANNELS] = {OFF, OFF, OFF};
          unsigned out_clr1[CHANNELS] = {OFF, OFF, OFF};
          
          mono(pix, lum / 8, MIDI_MAX);
          
          out0 = check_pix(pix + (NUM_LEDS / 3));
          get_clr(out0, lum, out_clr0);
          
          out1 = check_pix(pix - (NUM_LEDS / 3));
          get_clr(out1, lum, out_clr1);
          
          out0 = random(NUM_LEDS);
          
          do {
                  out1 = random(NUM_LEDS);
          } while (out1 == out0);
          
          strip.setPixelColor(out0, out_clr0[RED], out_clr0[GREEN], out_clr0[BLUE]);         
          strip.setPixelColor(out1, out_clr1[RED], out_clr1[GREEN], out_clr1[BLUE]);         
          strip.show();
}

void
outsider_tetrad(unsigned pix, unsigned lum, unsigned width)
{
          int out0, out1, out2;
          unsigned cnt;
          unsigned in_clr[CHANNELS] = {OFF, OFF, OFF};
          unsigned out_clr0[CHANNELS] = {OFF, OFF, OFF};
          unsigned out_clr1[CHANNELS] = {OFF, OFF, OFF};
          unsigned out_clr2[CHANNELS] = {OFF, OFF, OFF};
          
          
          mono(pix, lum / 8, MIDI_MAX);
          
          out0 = pix + (NUM_LEDS / 4);
          out0 = check_pix(out0);
          get_clr(out0, lum, out_clr0);
          
          out1 = pix + (NUM_LEDS / 2);
          out1 = check_pix(out1);
          get_clr(out1, lum, out_clr1);
          
          out2 = pix + (NUM_LEDS / 4);
          out2 = check_pix(out2);
          get_clr(out2, lum, out_clr2);
          
          out0 = random(NUM_LEDS);
          
          do {
                  out1 = random(NUM_LEDS);
          } while (out1 == out0);
          
          do {
                  out2 = random(NUM_LEDS);
          } while (out2 == out0 || out2 == out1);
          
          strip.setPixelColor(out0, out_clr0[RED], out_clr0[GREEN], out_clr0[BLUE]);         
          strip.setPixelColor(out1, out_clr1[RED], out_clr1[GREEN], out_clr1[BLUE]);         
          strip.setPixelColor(out2, out_clr2[RED], out_clr2[GREEN], out_clr2[BLUE]);         
          strip.show();
}
