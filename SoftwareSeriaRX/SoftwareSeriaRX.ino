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
//For Atmega328P's
// XBee's DOUT (TX) is connected to pin 2 (Arduino's Software RX)
// XBee's DIN (RX) is connected to pin 3 (Arduino's Software TX)
#include <SoftwareSerial.h>
#include <SD.h>
#include <SPI.h>
const int chipSelect = 4;
SoftwareSerial XBee(2, 3); // RX, TX
struct inData { //struct used for SRAM allocative efficiency 
  volatile uint8_t imgvect; 
};
void setup(){
  XBee.begin(9600);
 //DEBUG: Serial.begin(9600);
  while(!SD.begin(chipSelect)){
  //DEBUG: Serial.println("SD not found");
    delay(500);
    } 
 // DEBUG:Serial.println("card is ready");
  SD.remove("datalog.txt"); //reset the datafile on the arduino
  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  dataFile.close();
}

void loop(){
  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  struct inData rxData;
  while (XBee.available()) { // as long as it recieves data it saves it 
    rxData.imgvect = XBee.read();
    dataFile.write((const uint8_t*)&rxData, sizeof(rxData)); //casts the struct data as an 8-bit number to make it as memory efficient as possible 
    
    }
   dataFile.close();
}
