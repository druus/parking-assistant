/**
 * receiver
 
 The MIT License (MIT)
 
 Copyright (c) 2015 Daniel Ruus
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

#include <VirtualWire.h>
#include <LiquidCrystal.h>

// Define some constants
// Pins for the LCD module
#define LCD_ROWS   2
#define LCD_COLS   16
#define LCD_RS     9
#define LCD_ENABLE 8
#define LCD_DATA4  5
#define LCD_DATA5  4
#define LCD_DATA6  3
#define LCD_DATA7  2

// Pin for the radio receiver
#define RX_PIN  12
#define RX_BAUD 4000

// Pin for the frame error indicator LED
#define frameErrorPin 10

// Global variables
boolean isFrameError = false;

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(LCD_RS, LCD_ENABLE, LCD_DATA4, LCD_DATA5, LCD_DATA6, LCD_DATA7);

void setup()
{
    // Set up the serial port
    Serial.begin(9600);
    
    // set up the LCD's number of columns and rows:
    lcd.begin(LCD_COLS, LCD_ROWS);
    // Print a message to the LCD.
    lcd.write("Welcome!");  
    Serial.print("Hello, World\n");
    
    lcd.setCursor(0, 1);
    lcd.write("Version 0.1");
    delay(1500);
    
    vw_set_ptt_inverted(true);
    vw_set_rx_pin(12);
    vw_setup(4000);  // Bits per sec
    pinMode(13, OUTPUT);

    vw_rx_start();       // Start the receiver PLL running
    
    // Clear the display
    lcd.clear(); lcd.home();
    
    // Print a couple of "labels"
    lcd.setCursor(0, 0);
    lcd.write("Uptime:");
    lcd.setCursor(0, 1);
    lcd.write("Status:");
    lcd.setCursor(8, 1);
    lcd.write("?");
    
    // Initialize timer1 to be used for flashing an LED
    noInterrupts();  // Disable interrupts during configuration
    /*
    TCCR1A = 0;
    TCCR1B = 0;
  
    TCNT1 = 34286;            // preload timer 65536-16MHz/256/2Hz
    TCCR1B |= (1 << CS12);    // 256 prescaler 
    TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt
    */
    interrupts();             // enable all interrupts
    
    delay(1000);

}

ISR(TIMER1_OVF_vect)
{
  TCNT1 = 34286;            // preload timer
  if ( isFrameError ) {
    digitalWrite(frameErrorPin, digitalRead(frameErrorPin) ^ 1);   // toggle LED pin
  }
}

void loop()
{
    uint8_t buf[VW_MAX_MESSAGE_LEN];
    uint8_t buflen = VW_MAX_MESSAGE_LEN;
    char msg[VW_MAX_MESSAGE_LEN];
    String result;
    
    if (vw_get_message(buf, &buflen)) // Non-blocking
    {
      // For debug purposes - print result on serial port
      int i = 0;
      for (i = 0; i < buflen; i++) {
        msg[i] = buf[i];
        //Serial.print( buf[i] );
      }
      msg[++i] = '\0';
      Serial.print(msg);
      Serial.println();
      
      // Check if there is anything to do
      if(buf[0]=='0' && buf[2] == '1'){
        digitalWrite(13,1);
        result = String( msg );
        result = result.substring(4);
        result.toCharArray(msg, strlen(msg));
        lcd.setCursor(8, 1);
        lcd.write( msg );
        lcd.write( "cm" );
        Serial.print(msg);
        Serial.println("cm");
        isFrameError = false;
      }  
      /*
       if(buf[0]=='0' && buf[2] == '0'){
         digitalWrite(13,0);
         lcd.setCursor(8, 1);
         lcd.write("OFF");
         Serial.print("Turn LED OFF\n");
         isFrameError = false;
      }
      
      if (buf[0] == '0' && buf[2] == '2') {
        Serial.println("Received a fake frame error message - starting to flash LED");
        isFrameError = true;
      }
      */
    }
    
    lcd.setCursor(8, 0);
    lcd.print( millis() / 1000 );
    lcd.print("s");
    delay(50);
}
