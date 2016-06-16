#include "LPD8806.h"
#include "SPI.h"
#include "ake_common.h"
#include "spectra.h"

LPD8806 strip = LPD8806(NUM_LEDS, DATAPIN, CLOCKPIN);

void 
setup() 
{
        // Start up the LED strip
        strip.begin();
        // Update the strip, to start they are all 'off'
        strip.show();
        
        def_palette();

        pinMode(OUTPIN, OUTPUT);

        // opens serial port, sets data rate to 9600 bps
        Serial.begin(BAUD_RATE);
        Serial.setTimeout(TIMEOUT);
}
        
void 
loop() {
        struct rgb *col = (struct rgb*)malloc(sizeof(struct rgb*));
        int rout, lum, width, pix;

        while (1) {
                if (Serial.available()) {
                        /* 
                         * look for the next valid integer in the 
                         * incoming serial stream
                         */
                        rout = Serial.parseInt(); //color
                        check_int(&rout, OFF, NUM_ROUTS);
               
                        lum = Serial.parseInt(); //luminescence
                        check_int(&lum, OFF, MIDI_MAX);
                        
                        pix = Serial.parseInt(); //pixel
                
                        width = Serial.parseInt(); //width
                        check_int(&width, OFF, MIDI_MAX);
                        get_width(&width);
                }
                off();
                if (width) {
                        switch (rout) {
                        case OFF:
                                off();
                                break;
                        case MONO:
                                mono(pix, lum, width);
                                break;
                        case SPECTRUM:
                                spectrum(pix, lum, width);
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
                strip.show();
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
set_pix(struct rgb *col, int pix)
{
        strip.setPixelColor(pix, strip.Color(col->r, col->g, col->b));
        
}


