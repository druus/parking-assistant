/**
 * test_receive
 *
 * Setup:
 * Arduino Uno -> Receiver module
 *          12 -> Data
 */

#include <RCSwitch.h>

// Pin for the radio receiver
#define RX_PIN  12
// Baud rate to use for the receiver
#define RX_BAUD 4000
// Baud rate for the serial port
#define SERIAL_BAUD 9600

RCSwitch testSwitch = RCSwitch();

void setup() {
  // Set up the serial port
  Serial.begin(9600);
  testSwitch.enableReceive(0);
  Serial.println("Ready");
}

void loop() {
  if (testSwitch.available()) {
    int result = testSwitch.getReceivedValue();
    
    Serial.print("Received: ");
    Serial.println(testSwitch.getReceivedValue());
    
    testSwitch.resetAvailable();
  }
}
