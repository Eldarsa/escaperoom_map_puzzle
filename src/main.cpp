#include <Arduino.h>


#include <SPI.h>
#include <MFRC522.h>

/*** IMPORTANT!
 * 
 *  Everything you need to know about this code can be found here:
 *  https://www.youtube.com/watch?v=f_f_5cL0Pd0&t=185s
 * 
*/

// DEFINES
// Provides debugging information if defined
#define DEBUG


// CONSTANTS
// Number of RFID readers
const byte numReaders = 5; 

// Each reader has a unique Slave Select pin
const byte slaveSelectPins[] = {2,3,4,5};

//They'll share the same reset pin
const byte resetPin = 8;

// Initialize an array of MRFC522 instances representing each reader
MFRC522 mfrc522[numReaders];

// The sequence of RFID tag IDs required to solve the puzzle
const String correctIDs[] = {"replace", "replace", "replace", "replace", "replace"};

// The tag IDs currently detected by each reader
String currentIDs[numReaders];

// This pin will be driven LOW to release the lock
const byte lockPin = A0;


void setup() {

  #ifdef DEBUG
  Serial.begin(9600);
  Serial.println(F("Serial communication started"));
  #endif

  // Set the lock pin high to secure the lock.
  pinMode(lockPin, OUTPUT);
  digitalWrite(lockPin, HIGH);

  //Initialize the SPI bus
  SPI.begin();

  // Initialze the readers
  for (uint8_t i=0; i<numReaders; i++) {

    //Initialize the reader
    // Note that SPI pins on the reader must always be connected to certain
    // Arduino pins (on an Uni, MOSI => pin 11, MISO => pin12, SCK => pin 13)

    mfrc522[i].PCD_Init(ssPins[i], resetPin);

  }

}

void loop() {
  // put your main code here, to run repeatedly:
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}