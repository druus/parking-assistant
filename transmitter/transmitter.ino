/**
 * transmitter
 */

#include <VirtualWire.h>
#include <NewPing.h>

#define TRIGGER_PIN  12  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     11  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

// Define constants
const int buttonPin = 2;
const int ledPin    = 12;
const int echoPin   = 7;     // Echo pin of distance sensor
const int trigPin   = 8;     // Trigger pin

// Variables
int buttonState = 0;         // variable for reading the pushbutton status
int maximumRange = 200; // Maximum range needed
int minimumRange = 0; // Minimum range needed
unsigned long duration, distance; // Duration used to calculate distance

char controller[8];

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

void setup()
{
  // Set up the serial port
  Serial.begin(9600);
  
  pinMode(13,OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);      
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT); 
  
  vw_set_ptt_inverted(true); //
  vw_set_tx_pin(10);
  vw_setup(4000);// speed of data transfer Kbps
}

void loop()
{
  
  char b[16];
  String str;
  
  unsigned int uS = sonar.ping(); // Send ping, get ping time in microseconds (uS).

  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

  // Read the distance sensor
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2); 

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); 
 
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
 
  //Calculate the distance (in cm) based on the speed of sound.
  distance = duration/58.2;
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println("cm");
  
  if ( buttonState == HIGH ) {
    strcpy( controller, "0:2:8" );
    vw_send((uint8_t *)controller, strlen(controller));
    vw_wait_tx(); // Wait until the whole message is gone
  }
  
  strcpy( controller, "0:0:4" );
  vw_send((uint8_t *)controller, strlen(controller));
  vw_wait_tx(); // Wait until the whole message is gone
  digitalWrite(13,1);
  delay(100);

  //strcpy( controller, "0:1:7" );
  str = "0:1:";
  str += String(distance); // Convert integer to string
  str.toCharArray(b,16);
  
  Serial.print("Sending frame: ");
  Serial.println(b);
  vw_send((uint8_t *)b, strlen(b));
  vw_wait_tx(); // Wait until the whole message is gone
  digitalWrite(13,0);

  delay(1000);

}
