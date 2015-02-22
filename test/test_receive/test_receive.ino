/**
 * test_receive
 *
 * Setup:
 * Arduino Uno -> Receiver module
 *          12 -> Data
 */

#include <VirtualWire.h>

// Pin for the radio receiver
#define RX_PIN  12
// Baud rate to use for the receiver
#define RX_BAUD 4000
// Baud rate for the serial port
#define SERIAL_BAUD 9600

void setup() {
  // Set up the serial port
  Serial.begin(9600);

  // Set up the radio receiver module
  vw_set_ptt_inverted(true);
  vw_set_rx_pin(RX_PIN);
  vw_setup(RX_BAUD);
  
  // Start the receiver
  vw_rx_start();
  
  Serial.println("Ready");
}

void loop() {
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;
  char msg[VW_MAX_MESSAGE_LEN + 1];

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
  }
  
  delay(50);
}
