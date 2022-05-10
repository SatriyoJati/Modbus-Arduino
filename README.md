# Modbus-Arduino
The Arduino Nano is used as a communication relay between sensor and PLC. It serve as a master for a slave sensor and a slave for master PLC. I use Autonics PLC.
Modbus project
I use two libraries, Arduino as a Master (Client) and also Slave(Server)
Arduino as a Master :https://github.com/4-20ma/ModbusMaster
Arduino as a Slave  : https://github.com/yaacov/ArduinoModbusSlave
 
I use two modules MAX485, one for sensor and other for PLC
I don't use default Serial pin since it'll interfere my modbus comms, So I use Software Serial
Sensor is IMU WITMOTION SIDNT-485, I only use two register (xAngle, yAngle). yAngle is not used yet here.
Sensor ---> Arduino ---> PLC
