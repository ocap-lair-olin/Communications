/*****************************************************************
XBee_Serial_Passthrough.ino

Set up a software serial port to pass data between an XBee Shield
and the serial monitor.

Hardware Hookup:
  The XBee Shield makes all of the connections you'll need
  between Arduino and XBee. If you have the shield make
  sure the SWITCH IS IN THE "DLINE" POSITION. That will connect
  the XBee's DOUT and DIN pins to Arduino pins 2 and 3.

*****************************************************************/
// We'll use SoftwareSerial to communicate with the XBee:
#include <SoftwareSerial.h>
#include <SD.h>
#include <SPI.h>
const int chipSelect = 4;
volatile long incomingByte = -1;   // for incoming serial data
SoftwareSerial XBee(2, 3); // RX, TX
void setup(){
  XBee.begin(9600);
  Serial.begin(9600);
  while(!SD.begin(chipSelect)){
  Serial.println("SD not found");
  delay(500);
  } 
  Serial.println("card is ready");
  SD.remove("datalog.txt");
  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  dataFile.close();
}

void loop(){
  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  while (XBee.available()) { // If data comes in from XBee, send it out to serial monitor
    //incomingByte = XBee.read();
    //if(dataFile && incomingByte >= 0){
    dataFile.write(XBee.read());
    //Serial.write(XBee.read());
  //  }
  }
   dataFile.close();
}
