# parking-assistant
An Arduino-based parking assistant helping the driver to park in a dark and tight garage.

## What is it?
The parking assistant consist of two Arduino-based devices communicating using a couple of radio modules operating on the 433MHz frequency, one sender and a corresponding receiver.

### Sender
A couple of ultrasound distance measuring sensor devices attach to the sender device. The sender constantly measures the distance between the sensors, which are attached to the wall of the garage, and transmit the data to the receiver.

### Receiver
The receiver listens to data received by the radio module. This data contains the distance measured by the sender and is displayed on a LCD-module.
