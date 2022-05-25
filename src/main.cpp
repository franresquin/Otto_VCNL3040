
//-- SAMPLE TIME IN MS --//
#define SAMPLE_TIME_MS 20

//Define pins of the alternative i2c bus //
#define SDA1 21
#define SCL1 4
#define NUMBER_SENSORS 2

#include <Wire.h>
#include "VCNL3040.h"

typedef struct {
  byte header;
  byte counter;
  unsigned short int sensor_data[NUMBER_SENSORS];
  byte tail;
}msg_t;

msg_t serial_msg;
VCNL3040 sensor1, sensor2;

void setup() {
  
  Serial.begin(115200);

  // Init the I2C bus - Sensor 1 //
  Wire.begin();       // Join default i2c bus

  // Init the I2C bus - Sensor 2 //
  Wire1.setSDA(38);   // SDA1
  Wire1.setSCL(37);   // SCL1
  Wire1.begin();

  if (sensor1.begin() == false){
    Serial.println("** Sensor1 not found. Please check wiring.");
    //while (1); //Freeze!
  }
  
  if (sensor2.begin(Wire1) == false){
    Serial.println("** Sensor2 not found. Please check wiring.");
    //while (1); //Freeze!
  }

  serial_msg.header = 10;
  serial_msg.tail = 10;

  sensor1.writeCommand(0x03, 0x080E); // Duty: 0; It: 8T; Bits = 16 Bits
  sensor2.writeCommand(0x03, 0x080E); // Duty: 0; It: 8T; Bits = 16 Bits
  
}

void loop()
{
  //Get proximity value. The value ranges from 0 to 65535 //
  unsigned short int proxValue[NUMBER_SENSORS];
  static unsigned long ttime = millis();
  static uint16_t counter=0;
  String dat="";

  if( (ttime+SAMPLE_TIME_MS) <= millis() ){

    if( sensor1.isConnected() ){
      proxValue[0] = sensor1.getProximity();
    }else{
      proxValue[0] = 0;
    }

    if( sensor2.isConnected() ){
      proxValue[1] = sensor2.getProximity();
    }else{
      proxValue[1] = 0;
    }

    // Send data to the GUI //
    serial_msg.counter = counter++;
    serial_msg.sensor_data[0] = proxValue[0];  // optical sensor 1
    serial_msg.sensor_data[1] = proxValue[1];  // optical sensor 2
    //Serial.write((byte*)&serial_msg, 7);
    
    dat.concat(counter);
    dat.concat('\t');
    dat.concat(proxValue[0]);
    dat.concat('\t');
    dat.concat(proxValue[1]);
    dat.concat('\t');
    Serial.println(dat);
    Serial.flush();

    ttime += SAMPLE_TIME_MS;
    
  }
  
}