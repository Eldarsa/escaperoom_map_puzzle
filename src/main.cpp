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

// Each reader has a unique Slave Select (ss) pin
const byte ssPins[] = {2,3,4,5}; 

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

void onSolve();

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

    // Set the gain to max - not sure this makes any difference ..
    // mfrc522[i].PDC_SetAntennaGain(MFRC522::PCD_RxGain::RxGain_max);

    Serial.print(F("Reader #"));
    Serial.print(i);
    Serial.print(F(" initialized on pin "));
    Serial.print(String(ssPins[i]));
    Serial.print(F(". Antenna strength: "));
    Serial.print(mfrc522[i].PCD_GetAntennaGain());
    Serial.print(F(". Version : "));
    mfrc522[i].PCD_DumpVersionToSerial();

    // Slight delay before activating next reader
    delay(100);
  }

  Serial.println(F(" --- END SETUP ---"));

}

void loop() {
  
  // Assume the puzzle has been solved
  bool puzzleSolved = true;

  // Assume the tags have not changed since last reading
  bool changedValue = false;

  for (uint8_t i=0; i<numReaders; i++) {

      // Initialize the sensor
      mfrc522[i].PCD_Init();

      // String to hold the ID detected by each sensor
      String readRFID = "";

      // If the current reading is different from the last known reading
      if(readRFID != currentIDs[i])
      {
        // Set flag to show puzzle state has changed
        changedValue = true;
        // Update stored value for this sensor
        currentIDs[i] = readRFID;
      }

      // Sets false if at least one RFID is not correct.
      if(currentIDs[i] != correctIDs[i]){
        puzzleSolved = false;
      } 

      // Halt PICC
      mfrc522[i].PICC_HaltA();
      
      // Stop encryption on PCD
      mfrc522[i].PCD_StopCrypto1();

      // If at least one sensor has changed
      if(changedValue) {
        
        // Dump to serial state of all sensors
        for (uint8_t i=0; i<numReaders; i++) {
          Serial.print(F("Reader #"));
          Serial.print(String(i));
          Serial.print(F(" on Pin #"));
          Serial.print(String((ssPins[i])));
          Serial.print(F(" detected tag: "));
          Serial.println(currentIDs[i]);
        }
        Serial.println(F("---"));

      }

      if (puzzleSolved){
        onSolve();
      }

  }

}


void onSolve(){

  #ifdef DEBUG
    Serial.println(F("Puzzle Solved!"));
  #endif

  // Release the lock
  digitalWrite(lockPin, LOW);

  while(true){

  }
}
