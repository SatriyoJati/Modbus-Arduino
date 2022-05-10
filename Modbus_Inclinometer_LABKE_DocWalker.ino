
/* Modbus project (LAB KE inclinometer)
 * I use two libraries, Arduino as a Master (Client) and also Slave(Server)
 * Arduino as a Master :https://github.com/4-20ma/ModbusMaster
 * Arduino as a Slave  : https://github.com/yaacov/ArduinoModbusSlave
 * 
 * I use two modules MAX485, one for sensor and other for PLC
 * I don't use default Serial pin since it'll interfere my modbus comms, So I use Software Serial
 * Sensor is IMU WITMOTION SIDNT-485, I only use two register (xAngle, yAngle). yAngle is not used yet here.
 * 
 * Sensor ---> Arduino ---> PLC
 * 
*/

#include <ModbusMaster.h>
#include <SoftwareSerial.h>       //using Software Serial, not Hardware (limited to brate 19200)

#include <ModbusRtu.h> //Arduino slave
#define MAX485_DE      3  //control pin first MAX485  
#define MAX485_RE_NEG  2  //control pin first MAX485 
#define UPPER 6   //LED indicator
#define LOWER 7
#define TXEN  10

short xAngle = 0;
uint8_t result;
int16_t modbus_array[102];
unsigned long previousMillis = 0;
const long interval = 10;
  
//Creating modbus master object
ModbusMaster node;  
//Creating modbus slave 

SoftwareSerial myserial(4,5); //RX, TX for master arduino - slave sensor

//Creating modbus slave object using software serial, Slave ID : 1, control pin second MAX485  : 10 (RE & DE connected)
 Modbus bus(20,Serial,TXEN);

void preTransmission()
{
//  myserial.flush();
  digitalWrite(MAX485_RE_NEG, 1);
  digitalWrite(MAX485_DE, 1);
}

void postTransmission()
{
//  delay(10);
  digitalWrite(MAX485_RE_NEG, 0);
  digitalWrite(MAX485_DE, 0);
}

void processdata(short angle)
{
  if(angle >= 1){
    digitalWrite(UPPER,HIGH);
    digitalWrite(LOWER,LOW);
  }else if (angle <= -1){
    digitalWrite(UPPER,LOW);
    digitalWrite(LOWER,HIGH);
  }else{
    digitalWrite(UPPER,LOW);
    digitalWrite(LOWER,LOW);
  }
}

void setup() {
  // put your setup code here, to run once:
  //pinMode(10,OUTPUT);
  pinMode(MAX485_RE_NEG, OUTPUT);
  pinMode(MAX485_DE, OUTPUT);

  // Init in receive mode
  digitalWrite(MAX485_RE_NEG, 0);
  digitalWrite(MAX485_DE, 0);

  //baudrate for master arduino - slave sensor
  myserial.begin(19200);

  //Baudrate for default serial ( debugging only)
  Serial.begin(19200, SERIAL_8E1);
  bus.start();
  // Sensor Modbus slave ID is 0x50, I use "myserial" as a serial communication to first MAX485
  node.begin(0x50, myserial);
  
  // Callbacks allow us to configure the RS485 transceiver correctly
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);

}

void loop() {
  // put your main code here, to run repeatedly:

  //16-bit Sensor data on starting register address 0x3D, two contagious address requested 
  //Below how I begin requesting a query to slave(sensor) , it returns 0 if success, error code otherwise

  //poll every 10ms
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) 
  {
  previousMillis = currentMillis;
    //DO something
   modbus_array[100] = xAngle;
   bus.poll(modbus_array,102);
  }

  //Querying data from slave
  result = node.readHoldingRegisters(0x3E, 1);
  if ( result == node.ku8MBSuccess)
  {
    //Sensor conversion to degree, it uses short(int16_t) type to represent negative value
    xAngle = ((short)node.getResponseBuffer(0))/32768.0*180.0;
    processdata(xAngle);  
    //Serial.println(xAngle);
  }
  delay(60);
}
