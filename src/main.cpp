#include <Arduino.h>
#include <Wire.h>
#include <VCNL3040.h>

#define HostCom Serial
#define HostCom_BaudRate 115200
#define UPDATING_INTERVAL_MS 2000

VCNL3040 optSensor1, optSensor2;

void setup() {

  HostCom.begin(HostCom_BaudRate);
  while(!HostCom);
  HostCom.println("-- Hello Optical Sensors --");

  // Sensor 1 //
  Wire.begin();
  optSensor1.begin(Wire);

  // Sensor 2 //
  Wire1.setSDA(38);
  Wire1.setSCL(37);
  Wire1.begin();
  optSensor2.begin(Wire1);

}

void loop() {
  static unsigned long updating_interval = millis();
  unsigned long mtime = millis();
  static uint8_t counter=0;

  if(mtime >= updating_interval){
    
    HostCom.print(counter);
    HostCom.print(") ID: ");
    HostCom.print(optSensor1.getID(), HEX);
    HostCom.print("; Connected: ");
    HostCom.print(optSensor1.isConnected());
    HostCom.print("; Proximity: ");
    HostCom.print(optSensor1.getProximity());
    HostCom.println();

    HostCom.print(counter);
    HostCom.print(") ID: ");
    HostCom.print(optSensor2.getID(), HEX);
    HostCom.print("; Connected: ");
    HostCom.print(optSensor2.isConnected());
    HostCom.print("; Proximity: ");
    HostCom.print(optSensor2.getProximity());
    HostCom.println();

    HostCom.println();
    HostCom.flush();  

    counter++;
    updating_interval += UPDATING_INTERVAL_MS;
  }
}
