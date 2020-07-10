/*****************************************************************
 * Last updated 7/10/2020
 * Code for an RF image transmitter based on the XBee and SparkFun XBee 
 * for use on an Arduino Uno
 * This was written to test RF attenuation in saltwater
 * for the 2020 summer LAIR Lab OCAP reaserch project
 * Plug the RX and boot it up first before booting the TX 
 * TODO?: Add a startup handshake
 * FOR DEBUG: UNCOMMENT SERIAL PORT COMMANDS FOR SD CARD AND I/O and look at serial Monitor
 */

// Import Software Serial and SD Card Libraries 
#include <SoftwareSerial.h>
#include <SPI.h>
#include <SD.h>

//For Atmega328P's
// XBee's DOUT (TX) is connected to pin 2 (Arduino's Software RX)
// XBee's DIN (RX) is connected to pin 3 (Arduino's Software TX)
SoftwareSerial XBee(2, 3); // RX, TX
const int chipSelect = 4;
int smooth = 0; // For the delay function
volatile long readData = -1; // this is not a great solution, but it works as a buffer between the read data 
void setup()
{
  // for the XBee. Make sure the baud rate matches the config setting of your XBee.
  XBee.begin(9600);
 // DEBUG: Serial.begin(9600);
 // while (!Serial) {
  //  ; // wait for serial port to connect. Needed for native USB port only
 // }


  //DEBUG: Serial.print("Initializing SD card...");
    while (!SD.begin(chipSelect)) {
    // DEBUG:Serial.println("Card failed, or not present");
    //  DEBUG: don't do anything more:
    delay (10000);
  }
 // DEBUG: Serial.println("card initialized.");
 

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("datalog.txt");

  // if the file is available, read it:
  if (dataFile) {
    while (dataFile.available()) {
      readData = dataFile.read();
      if (readData >= 0){
     // DEBUG: Serial.write(readData);
      XBee.write(readData);
      smooth++;
       if (smooth == 60){//flow control conditional
         delay(125);
         smooth = 0;
      }
      }
    }
    dataFile.close();
  }
  // if the file isn't open, pop up an error:
 // DEBUG: else {
  // DEBUG:  Serial.println("error opening datalog.txt");
 //DEBUG:  }
}

void loop()
{

}
