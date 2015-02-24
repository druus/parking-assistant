/**
 * transmitter
 
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
#include <NewPing.h>

#define TRIGGER_PIN_1  12  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN_1     11  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define TRIGGER_PIN_2  8  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN_2     7  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar1(TRIGGER_PIN_1, ECHO_PIN_1, MAX_DISTANCE);
NewPing sonar2(TRIGGER_PIN_2, ECHO_PIN_2, MAX_DISTANCE);

void setup()
{
  // Set up the serial port
  Serial.begin(9600);

  vw_set_tx_pin(10);
  vw_setup(4000);// speed of data transfer
}

void loop()
{
  
  char b1[16], b2[16];
  String str1, str2;
 
  unsigned int distance1 = sonar1.convert_cm(sonar1.ping_median(5)); // Send ping, get ping time in microseconds (uS).
  delay(100);
  unsigned int distance2 = sonar2.convert_cm(sonar2.ping_median(5));
  
  Serial.print("Distance 1: ");
  Serial.print(distance1);
  Serial.print("cm");
  Serial.print("\tDistance 2: ");
  Serial.print(distance2);
  Serial.println("cm");

  memset(b1, 0, sizeof(b1));
  str1 = "0:1:";
  str1 += String(distance1); // Convert integer to string
  str1.toCharArray(b1,16);
  
  Serial.print("Sending frame 1: ");
  Serial.println(b1);
  vw_send((uint8_t *)b1, strlen(b1));
  vw_wait_tx(); // Wait until the whole message is gone
  
  delay(50);
  memset(b2, 0, sizeof(b2));
  str2 = "0:2:";
  str2 += String(distance2); // Convert integer to string
  str2.toCharArray(b2,16);

  Serial.print("Sending frame 2: ");
  Serial.println(b2);
  vw_send((uint8_t *)b2, strlen(b2));
  vw_wait_tx(); // Wait until the whole message is gone
  
  delay(50);

}
