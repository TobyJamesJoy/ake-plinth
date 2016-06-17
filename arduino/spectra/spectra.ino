#include "LPD8806.h"
#include "SPI.h"


#define BAUD_RATE 115200
#define CHANNELS 3
#define CLOCKPIN 3
#define DATAPIN 2
#define NUM_LEDS 34
#define OFF 0
#define OUTPIN 13
#define TIMEOUT 5

LPD8806 strip = LPD8806(NUM_LEDS, DATAPIN, CLOCKPIN);

enum colours {
        R, O = 3, Y = 6, CH = 9, G = 12, GC = 15, 
        C = 18, BC = 21, B = 24, V = 27, M = 30, RM = 33
};

enum channels {
        RED, GREEN, BLUE
};

enum routines {
        OFFF, SINGLE, SPECTRUM, SPECTRUM_CHASE, DOUBLE_SPECTRUM,
        DIAD, TRIAD, TRIAD_CHASE, TETRAD, SKINNY_TETRAD,
        MONO_STRIP, HEMISPHERE, TRISECT, QUADRANT,
        OUTSIDER, OUTSIDER_TRIAD, OUTSIDER_TETRAD,
        SINGLE_WHITE, DIAD_WHITE, TRIAD_WHITE, TETRAD_WHITE, FULL_WHITE
};

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
}
        
void 
loop() {
        unsigned rout, pix, lum, t_pix, out0, out1, out2;

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

                t_pix = Serial.parseInt();
                Serial.write(t_pix);

                out0 = Serial.parseInt();
                Serial.write(out0);

                out1 = Serial.parseInt();
                Serial.write(out1);

                out2 = Serial.parseInt();
                Serial.write(out2);
                
                switch (rout) {
                case OFF:
                        off();
                        break;
                case SINGLE:
                        off();
                        mk_pix(pix, lum);
                        strip.show();
                        break;
                case SPECTRUM:
                        spectrum(pix, lum);
                        break;
                case SPECTRUM_CHASE:
                        spectrum_chase(pix, lum);
                        break;
                case DOUBLE_SPECTRUM:
                        spectrum_double_chase(pix, lum);
                        break;
                case DIAD:
                        diad(pix, lum);
                        break;
                case TRIAD:
                        triad(pix, lum);
                        break;
                case TRIAD_CHASE:
                        triad_chase(pix, lum, t_pix);
                        break;
                case TETRAD:
                        tetrad(pix, lum);
                        break;
                case SKINNY_TETRAD:
                        skinny_tetrad(pix, lum);
                        break;
                case MONO_STRIP:
                        mono_strip(pix, lum);
                        break;
                case HEMISPHERE:
                        hemisphere(pix, lum);
                        break;
                case TRISECT:
                        trisect(pix, lum);
                        break;
                case QUADRANT:
                        quadrant(pix, lum);
                        break;
                case OUTSIDER:
                        outsider(pix, lum, out0);
                        break;
                case OUTSIDER_TRIAD:
                        outsider_triad(pix, lum, out0, out1);
                        break;
                case OUTSIDER_TETRAD:
                        outsider_tetrad(pix, lum, out0, out1, out2);
                        break;
                case SINGLE_WHITE:
                        single_white(pix, lum);
                        break;
                case DIAD_WHITE:
                        diad_white(pix, lum);
                        break;
                case TRIAD_WHITE:
                        triad_white(pix, lum);
                        break;
                case TETRAD_WHITE:
                        tetrad_white(pix, lum);
                        break;
                case FULL_WHITE:
                        full_white(lum);
                }

        }
}

void
off(void)
{
      unsigned cnt;
      
      for (cnt = 0; cnt < NUM_LEDS; ++cnt)
              strip.setPixelColor(cnt, OFF);
}
void
set_clr(unsigned pix, unsigned clr[CHANNELS])
{
        strip.setPixelColor(pix, strip.Color(clr[RED], clr[GREEN], clr[BLUE]));
}

void get_clr(unsigned pix, unsigned lum, unsigned clr[CHANNELS])
{          
          switch (pix) {
          case R:
          case R + 1:
          case R + 2:
                  clr[RED] = lum;
                  break;
          case O:
          case O + 1:
          case O + 2:
                  clr[RED] = lum;
                  clr[GREEN] = lum / 2;
                  break;
          case Y:
          case Y + 1:
          case Y + 2:
                  clr[RED] = lum;
                  clr[GREEN] = lum;
                  break;
          case CH:
          case CH + 1:
          case CH + 2:
                  clr[RED] = lum / 2;
                  clr[GREEN] = lum;
                  break;
          case G:
          case G + 1:
          case G + 2:
                  clr[GREEN] = lum;
                  break;
          case GC:
          case GC + 1:
          case GC + 2:
                  clr[GREEN] = lum;
                  clr[BLUE] = lum / 2;
                  break;
          case C:
          case C + 1:
          case C + 2:
                  clr[GREEN] = lum;
                  clr[BLUE] = lum;
                  break;
          case BC:
          case BC + 1:
          case BC + 2:
                  clr[GREEN] = lum / 2;
                  clr[BLUE] = lum;
                  break;
          case B:
          case B + 1:
          case B + 2:
                  clr[BLUE] = lum;
                  break;
          case V:
          case V + 1:
          case V + 2:
                  clr[RED] = lum / 2;
                  clr[BLUE] = lum;
                  break;
          case M:
          case M + 1:
          case M + 2:
                  clr[RED] = lum;
                  clr[BLUE] = lum;
                  break;
          case RM:
          case RM + 1:
          case RM + 2:
                  clr[RED] = lum;
                  clr[BLUE] = lum / 2;
          }
}

void
mk_pix(unsigned pix, unsigned lum)
{
        unsigned clr[CHANNELS] = {OFF, OFF, OFF};
        
        get_clr(pix, lum, clr);
        set_clr(pix, clr);        
}

void
spectrum(unsigned pix, unsigned lum) 
{
        unsigned cnt;
        
        for (cnt = pix; cnt < NUM_LEDS; ++cnt)
                mk_pix(cnt, lum);
        strip.show();
}

void
spectrum_chase(unsigned pix, unsigned lum) 
{
        unsigned cnt;
        
	off();
  
        for (cnt = 0; cnt <= pix; ++cnt) 
                mk_pix(cnt, lum);
            
        strip.show();

}

void
spectrum_double_chase(unsigned pix, unsigned lum)
{
        unsigned cnt;

        off();
        
        mk_pix(0, lum);
        
        for (cnt = 1; cnt < pix; ++cnt) {
                mk_pix(check_pix(0 + cnt), lum);
                mk_pix(check_pix(0 - cnt), lum);
        }
        strip.show();
}

int
check_pix(int pix)
{
        if (pix > NUM_LEDS)
                pix -= NUM_LEDS;
        else if (pix < 0)
                pix += NUM_LEDS;

        return pix;
}

void
diad(unsigned pix, unsigned lum)
{
        unsigned cnt;
        int di = pix + NUM_LEDS / 2;
        
        di = check_pix(di);

        off();      
        mk_pix(pix, lum);
        mk_pix(di, lum);
        strip.show();
}

void 
mk_triad(unsigned pix, unsigned lum, unsigned offset) 
{
        mk_pix(pix, lum);
        mk_pix(check_pix((int)pix + offset), lum);
        mk_pix(check_pix((int)pix - offset), lum);
  
        strip.show();
}

void
triad(unsigned pix, unsigned lum)
{
        unsigned cnt;

        off();
        
        mk_triad(pix, lum, NUM_LEDS / 3);
}

void
triad_chase(unsigned pix, unsigned lum, unsigned t_pix)
{
        off();
        
        mk_triad(pix, lum, t_pix);
}

void
tetrad(unsigned pix, unsigned lum)
{
        int pint = (int)pix;

        off();
        
        mk_pix(pix, lum);
        
        pint += (NUM_LEDS / 4);
        pint = check_pix(pint);
        mk_pix(pint, lum);
        
        pint += (NUM_LEDS / 2);
        pint = check_pix(pint);
        mk_pix(pint, lum);
        
        pint -= (NUM_LEDS / 4);
        pint = check_pix(pint);
        mk_pix(pint, lum);
        
        strip.show();
}

void
skinny_tetrad(unsigned pix, unsigned lum)
{
        int pint = (int)pix;

        off();
        
        mk_pix(pix, lum);
        
        pint += (NUM_LEDS / 6);
        pint = check_pix(pint);
        mk_pix(pint, lum);
        
        pint += (NUM_LEDS / 2);
        pint = check_pix(pint);
        mk_pix(pint, lum);
        
        pint -= (NUM_LEDS / 6);
        pint = check_pix(pint);
        mk_pix(pint, lum);
        
        strip.show();
}

void
set_mono(unsigned pix, unsigned lum, unsigned clr[CHANNELS])
{
        unsigned cnt;
        
        get_clr(pix, lum, clr);
        
        for (cnt = 0; cnt < NUM_LEDS; ++cnt)
                strip.setPixelColor(cnt, clr[RED], clr[GREEN], clr[BLUE]);
}

void
mono_strip(unsigned pix, unsigned lum)
{
        unsigned cnt;
        unsigned clr[CHANNELS] = {OFF, OFF, OFF};
        
        set_mono(pix, lum, clr);
        strip.show();
}

void
hemisphere(unsigned pix, unsigned lum)
{
          int p;
          unsigned cnt;
          unsigned clr[CHANNELS] = {OFF, OFF, OFF};

          get_clr(pix, lum, clr);
          p = pix;
          for (cnt = 0; cnt < NUM_LEDS / 2; ++cnt, ++p) {
                  p = check_pix(p);
                  strip.setPixelColor(p, clr[RED], clr[GREEN], clr[BLUE]);
          }
          
          for (cnt = 0; cnt < CHANNELS; ++cnt)
                  clr[cnt] = OFF;
          p = (pix + NUM_LEDS / 2);
          p = check_pix(p);
          get_clr(p, lum, clr);
          for (cnt = 0; cnt < NUM_LEDS / 2; ++cnt, ++p) {
                  p = check_pix(p);
                  strip.setPixelColor(p, clr[RED], clr[GREEN], clr[BLUE]);
          }
          
          strip.show();
}

void
trisect(unsigned pix, unsigned lum)
{
          int p;
          unsigned cnt;
          unsigned clr[CHANNELS] = {OFF, OFF, OFF};

          get_clr(pix, lum, clr);
          p = pix;
          for (cnt = 0; cnt < NUM_LEDS / 3; ++cnt, ++p) {
                  p = check_pix(p);
                  strip.setPixelColor(p, clr[RED], clr[GREEN], clr[BLUE]);
          }
          
          for (cnt = 0; cnt < CHANNELS; ++cnt)
                  clr[cnt] = OFF;
          p = (pix + NUM_LEDS / 3);
          p = check_pix(p);
          get_clr(p, lum, clr);
          for (cnt = 0; cnt < NUM_LEDS / 3; ++cnt, ++p) {
                  p = check_pix(p);
                  strip.setPixelColor(p, clr[RED], clr[GREEN], clr[BLUE]);
          }
          
          
          for (cnt = 0; cnt < CHANNELS; ++cnt)
                  clr[cnt] = OFF;
          p = (pix - NUM_LEDS / 3);
          p = check_pix(p);
          get_clr(p, lum, clr);
          for (cnt = 0; cnt < NUM_LEDS / 3; ++cnt, ++p) {
                  p = check_pix(p);
                  strip.setPixelColor(p, clr[RED], clr[GREEN], clr[BLUE]);
          }
          
          strip.show();
}

void
quadrant(unsigned pix, unsigned lum)
{
          int p;
          unsigned cnt;
          unsigned clr[CHANNELS] = {OFF, OFF, OFF};

          get_clr(pix, lum, clr);
          p = pix;
          for (cnt = 0; cnt < NUM_LEDS / 4; ++cnt, ++p) {
                  p = check_pix(p);
                  strip.setPixelColor(p, clr[RED], clr[GREEN], clr[BLUE]);
          }
          
          for (cnt = 0; cnt < CHANNELS; ++cnt)
                  clr[cnt] = OFF;
          p = pix + (NUM_LEDS / 4);
          p = check_pix(p);
          get_clr(p, lum, clr);
          for (cnt = 0; cnt < NUM_LEDS / 4; ++cnt, ++p) {
                  p = check_pix(p);
                  strip.setPixelColor(p, clr[RED], clr[GREEN], clr[BLUE]);
          }
          
          
          for (cnt = 0; cnt < CHANNELS; ++cnt)
                  clr[cnt] = OFF;
          p = pix +  (NUM_LEDS / 2);       
          p = check_pix(p);
          get_clr(p, lum, clr);
          for (cnt = 0; cnt < NUM_LEDS / 4; ++cnt, ++p) {
                  p = check_pix(p);
                  strip.setPixelColor(p, clr[RED], clr[GREEN], clr[BLUE]);
          }
          
          for (cnt = 0; cnt < CHANNELS; ++cnt)
                  clr[cnt] = OFF;
          p = (pix - NUM_LEDS / 4);
          p = check_pix(p);
          get_clr(p, lum, clr);
          for (cnt = 0; cnt < NUM_LEDS / 4; ++cnt, ++p) {
                  p = check_pix(p);
                  strip.setPixelColor(p, clr[RED], clr[GREEN], clr[BLUE]);
          }
          
          strip.show();
}

void
outsider(unsigned pix, unsigned lum, unsigned out)
{
          unsigned in_clr[CHANNELS] = {OFF, OFF, OFF};
          unsigned out_clr[CHANNELS] = {OFF, OFF, OFF};
          
          set_mono(pix, lum / 8, in_clr);
          
          get_clr(check_pix((int)pix + (NUM_LEDS / 2)), lum, out_clr);
                    
          strip.setPixelColor(out, out_clr[RED], out_clr[GREEN], out_clr[BLUE]);         
          strip.show();
}

void
outsider_triad(unsigned pix, unsigned lum, unsigned out0, unsigned out1)
{
          unsigned in_clr[CHANNELS] = {OFF, OFF, OFF};
          unsigned out_clr0[CHANNELS] = {OFF, OFF, OFF};
          unsigned out_clr1[CHANNELS] = {OFF, OFF, OFF};
          
          set_mono(pix, lum / 8, in_clr);
          
          get_clr(check_pix(pix + (NUM_LEDS / 3)), lum, out_clr0);
          get_clr(check_pix(pix - (NUM_LEDS / 3)), lum, out_clr1);
          
          strip.setPixelColor(out0, out_clr0[RED], out_clr0[GREEN], out_clr0[BLUE]);         
          strip.setPixelColor(out1, out_clr1[RED], out_clr1[GREEN], out_clr1[BLUE]);         
          strip.show();
}

void
outsider_tetrad(unsigned pix, unsigned lum, unsigned out0, unsigned out1, unsigned out2)
{
          unsigned in_clr[CHANNELS] = {OFF, OFF, OFF};
          unsigned out_clr0[CHANNELS] = {OFF, OFF, OFF};
          unsigned out_clr1[CHANNELS] = {OFF, OFF, OFF};
          unsigned out_clr2[CHANNELS] = {OFF, OFF, OFF};
          
          set_mono(pix, lum / 8, in_clr);
          
          get_clr(check_pix(pix + (NUM_LEDS / 4)), lum, out_clr0);
          get_clr(check_pix(pix + (NUM_LEDS / 2)), lum, out_clr1);
          get_clr(check_pix(pix - (NUM_LEDS / 4)), lum, out_clr2);
          
          strip.setPixelColor(out0, out_clr0[RED], out_clr0[GREEN], out_clr0[BLUE]);         
          strip.setPixelColor(out1, out_clr1[RED], out_clr1[GREEN], out_clr1[BLUE]);         
          strip.setPixelColor(out2, out_clr2[RED], out_clr2[GREEN], out_clr2[BLUE]);         
          strip.show();
}

void
single_white(unsigned pix, unsigned lum)
{
        off();      
        strip.setPixelColor(pix, lum, lum, lum);
        strip.show();
}

void
diad_white(unsigned pix, unsigned lum)
{
        off();
        strip.setPixelColor(pix, lum, lum, lum);
        strip.setPixelColor(check_pix((int)pix + (NUM_LEDS / 2)), lum, lum, lum);
        strip.show();
}

void
triad_white(unsigned pix, unsigned lum)
{
        off();
        strip.setPixelColor(pix, lum, lum, lum);
        strip.setPixelColor(check_pix((int)pix + (NUM_LEDS / 3)), lum, lum, lum);
        strip.setPixelColor(check_pix((int)pix - (NUM_LEDS / 3)), lum, lum, lum);
        strip.show();
}

void
tetrad_white(unsigned pix, unsigned lum)
{
        off();
        strip.setPixelColor(pix, lum, lum, lum);
        strip.setPixelColor(check_pix((int)pix + (NUM_LEDS / 4)), lum, lum, lum);
        strip.setPixelColor(check_pix((int)pix + (NUM_LEDS / 2)), lum, lum, lum);
        strip.setPixelColor(check_pix((int)pix - (NUM_LEDS / 4)), lum, lum, lum);
        strip.show();
}

void
full_white(unsigned lum)
{
        unsigned cnt;
      
        for (cnt = 0; cnt < NUM_LEDS; ++cnt)
                strip.setPixelColor(cnt, lum, lum, lum);
                
        strip.show();
}
