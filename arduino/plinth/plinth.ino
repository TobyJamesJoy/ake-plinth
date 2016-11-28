#include "LPD8806.h"
#include "SPI.h"
#include "ake_common.h"



LPD8806 strip = LPD8806(NUM_LEDS, DATAPIN, CLOCKPIN);

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
        struct rgb *col = (rgb *)malloc(sizeof(rgb));
        init_col(col);
        int cnt, pix, width;
        
        while (1) {
                /* 
                 * look for the next valid integer in the incoming 
                 * serial stream
                 */
                if (Serial.available()) {
                        col->r = Serial.parseInt(); //red
                        check_int(&col->r, OFF, MIDI_MAX);
                        
                        col->g = Serial.parseInt(); //green
                        check_int(&col->g, OFF, MIDI_MAX);
                        
                        col->b = Serial.parseInt(); //blue
                        check_int(&col->b, OFF, MIDI_MAX);
                        
                        pix = Serial.parseInt(); //pixel
                        check_int(&pix, OFF, (NUM_LEDS - 1));
                        
                        width = Serial.parseInt(); //width
                        check_int(&width, OFF, MIDI_MAX);
                        get_width(&width);
                }
                
                off();
                if (width)
                        set_col(col, pix, width);
                                
                strip.show();
        }
        free(col);
}

void
off()
{
	unsigned cnt;
        
        for (cnt = 0; cnt < NUM_LEDS; ++cnt)
                strip.setPixelColor(cnt, OFF);
}

void
set_pix(struct rgb* col, int pix)
{
        strip.setPixelColor(pix, col->r, col->g, col->b);
}
