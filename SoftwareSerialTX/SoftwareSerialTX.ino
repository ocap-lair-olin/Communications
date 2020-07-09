
// We'll use SoftwareSerial to communicate with the XBee:
#include <SoftwareSerial.h>
#include <SPI.h>
#include <SD.h>

//For Atmega328P's
// XBee's DOUT (TX) is connected to pin 2 (Arduino's Software RX)
// XBee's DIN (RX) is connected to pin 3 (Arduino's Software TX)
SoftwareSerial XBee(2, 3); // RX, TX
const int chipSelect = 4;
int smooth = 0;
volatile long readData = -1;
void setup()
{
  // Set up both ports at 9600 baud. This value is most important
  // for the XBee. Make sure the baud rate matches the config
  // setting of your XBee.
  XBee.begin(9600);
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  Serial.print("Initializing SD card...");
    while (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    delay (10000);
  }
  Serial.println("card initialized.");
 

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("datalog.txt");

  // if the file is available, write to it:
  if (dataFile) {
    while (dataFile.available()) {
      readData = dataFile.read();
      if (readData >= 0){
     // Serial.write(readData);
      XBee.write(readData);
     // delay(100);
      smooth++;
      if (smooth == 50){
      delay(300);
      smooth = 0;
      }
      }
    }
    dataFile.close();
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }
}

void loop()
{

}
