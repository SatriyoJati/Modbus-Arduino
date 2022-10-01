# Modbus-Arduino
The Arduino Nano is used as a communication relay between sensor and PLC. It serve as a master for a slave sensor and a slave for master PLC. I use Autonics PLC.
Modbus project

I use two libraries, Arduino as a Master (Client) and a Slave(Server)
 ![1649947678089](https://user-images.githubusercontent.com/58125687/193417171-d404b9e9-83ee-4afe-97b2-a90434eb2058.jpg)
 
 ![1649947678179](https://user-images.githubusercontent.com/58125687/193417211-0391b772-425e-439b-b489-bc3983271edb.jpg)

Arduino as a Master :https://github.com/4-20ma/ModbusMaster

Arduino as a Slave  : https://github.com/yaacov/ArduinoModbusSlave


I use two modules MAX485, one is for sensor and other for PLC
I don't use default Serial pin since it'll interfere my modbus communication, So I employ Software Serial
Sensor is IMU WITMOTION SIDNT-485, I only use two register (xAngle, yAngle). yAngle is not used yet here.
Sensor ---> Arduino ---> PLC

