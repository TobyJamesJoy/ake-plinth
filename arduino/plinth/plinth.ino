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
        OUTSIDER, OUTSIDER_TRIAD, OUTSIDER_TETRAD
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
        unsigned rout, pix, lum, del;

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
                
                
                del = Serial.parseInt(); //delay
                Serial.write(del);
                
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
                        spectrum(pix, lum, del);
                        break;
                case SPECTRUM_CHASE:
                        spectrum_chase(pix, lum, del);
                        break;
                case DOUBLE_SPECTRUM:
                        spectrum_double_chase(pix, lum, del);
                        break;
                case DIAD:
                        diad(pix, lum, del);
                        break;
                case TRIAD:
                        triad(pix, lum, del);
                        break;
                case TRIAD_CHASE:
                        triad_chase(pix, lum, del);
                        break;
                case TETRAD:
                        tetrad(pix, lum, del);
                        break;
                case SKINNY_TETRAD:
                        skinny_tetrad(pix, lum, del);
                        break;
                case MONO_STRIP:
                        mono_strip(pix, lum, del);
                        break;
                case HEMISPHERE:
                        hemisphere(pix, lum, del);
                        break;
                case TRISECT:
                        trisect(pix, lum, del);
                        break;
                case QUADRANT:
                        quadrant(pix, lum, del);
                        break;
                case OUTSIDER:
                        outsider(pix, lum, del);
                        break;
                case OUTSIDER_TRIAD:
                        outsider_triad(pix, lum, del);
                        break;
                case OUTSIDER_TETRAD:
                        outsider_tetrad(pix, lum, del);
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
spectrum(unsigned pix, unsigned lum, unsigned del) 
{
        unsigned cnt;
        
        for (cnt = pix; cnt < NUM_LEDS; ++cnt)
                mk_pix(cnt, lum);
        strip.show();
}

void
spectrum_chase(unsigned pix, unsigned lum, unsigned del) 
{
        unsigned cnt;
        int pix1 = (int)pix;
        
        for (cnt = 0; cnt < NUM_LEDS; ++cnt)
                strip.setPixelColor(cnt, OFF);
  
        for (cnt = 0; cnt < NUM_LEDS; ++cnt, ++pix1) {
                pix1 = check_pix(pix1);
                mk_pix(pix1, lum);
                strip.show();
        }     
}

void
spectrum_double_chase(unsigned pix, unsigned lum, unsigned del)
{
        unsigned cnt;
        int pix1, pix2;
        
        pix1 = (int)pix;
        pix2 = pix1;

        for (cnt = 0; cnt < NUM_LEDS; ++cnt)
                strip.setPixelColor(cnt, OFF);
        
        for (cnt = 0; cnt < NUM_LEDS / 2; ++cnt, ++pix1, --pix2) {
                pix1 = check_pix(pix1);
                pix2 = check_pix(pix2);
                mk_pix(pix1, lum);
                mk_pix(pix2, lum);
                strip.show();
        }
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
diad(unsigned pix, unsigned lum, unsigned del)
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
mk_triad(unsigned pix, unsigned lum, unsigned del, unsigned offset) 
{
        int pint = (int)pix;

        mk_pix(pix, lum);
        pint = check_pix(pint + offset);
        mk_pix(pint, lum);
        pint = check_pix((int)pix - offset);
        mk_pix(pint, lum);
        strip.show();
}

void
triad(unsigned pix, unsigned lum, unsigned del)
{
        unsigned cnt;
        
        for (cnt = 0; cnt < NUM_LEDS; ++cnt)
                strip.setPixelColor(cnt, OFF);        

        mk_triad(pix, lum, del, NUM_LEDS / 3);
}

void
triad_chase(unsigned pix, unsigned lum, unsigned del)
{
        unsigned cnt;
        
        for (cnt = 0; cnt < NUM_LEDS; ++cnt)
                strip.setPixelColor(cnt, OFF);        

        for (cnt = 1; cnt <= NUM_LEDS / 2; ++cnt)
                mk_triad(pix, lum, del, cnt);
}

void
tetrad(unsigned pix, unsigned lum, unsigned del)
{
        int pint = (int)pix;
        unsigned cnt;

        for (cnt = 0; cnt < NUM_LEDS; ++cnt)
                strip.setPixelColor(cnt, OFF);        
        
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
skinny_tetrad(unsigned pix, unsigned lum, unsigned del)
{
        int pint = (int)pix;
        unsigned cnt;

        for (cnt = 0; cnt < NUM_LEDS; ++cnt)
                strip.setPixelColor(cnt, OFF);        
        
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
mono_strip(unsigned pix, unsigned lum, unsigned del)
{
        unsigned cnt;
        unsigned clr[CHANNELS] = {OFF, OFF, OFF};
        
        set_mono(pix, lum, clr);
        strip.show();
}

void
hemisphere(unsigned pix, unsigned lum, unsigned del)
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
trisect(unsigned pix, unsigned lum, unsigned del)
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
quadrant(unsigned pix, unsigned lum, unsigned del)
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
outsider(unsigned pix, unsigned lum, unsigned del)
{
          unsigned cnt, out;
          unsigned in_clr[CHANNELS] = {OFF, OFF, OFF};
          unsigned out_clr[CHANNELS] = {OFF, OFF, OFF};
          
          set_mono(pix, lum / 8, in_clr);
          
          out = pix + NUM_LEDS / 2;
          if (out >= NUM_LEDS)
                  out -= NUM_LEDS;
          get_clr(out, lum, out_clr);
                    
          out = random(NUM_LEDS);
          strip.setPixelColor(out, out_clr[RED], out_clr[GREEN], out_clr[BLUE]);         
          strip.show();
}

void
outsider_triad(unsigned pix, unsigned lum, unsigned del)
{
          int out0, out1;
          unsigned cnt;
          unsigned in_clr[CHANNELS] = {OFF, OFF, OFF};
          unsigned out_clr0[CHANNELS] = {OFF, OFF, OFF};
          unsigned out_clr1[CHANNELS] = {OFF, OFF, OFF};
          
          set_mono(pix, lum / 8, in_clr);
          
          out0 = pix + (NUM_LEDS / 3);
          if (out0 >= NUM_LEDS)
                  out0 -= NUM_LEDS;
          get_clr(out0, lum, out_clr0);
          
          out1 = pix - (NUM_LEDS / 3);
          if (out1 < 0)
                  out1 += NUM_LEDS;
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
outsider_tetrad(unsigned pix, unsigned lum, unsigned del)
{
          int out0, out1, out2;
          unsigned cnt;
          unsigned in_clr[CHANNELS] = {OFF, OFF, OFF};
          unsigned out_clr0[CHANNELS] = {OFF, OFF, OFF};
          unsigned out_clr1[CHANNELS] = {OFF, OFF, OFF};
          unsigned out_clr2[CHANNELS] = {OFF, OFF, OFF};
          
          
          set_mono(pix, lum / 8, in_clr);
          
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
