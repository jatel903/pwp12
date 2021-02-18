# PWP12
## v1.0
### Automatic control of the disinfection cabin pump
###### jatel.sk

- For PRESEN-FSR v1.0 board, which is compatible with Arduino NANO
- Sensor used: PIR sensor adapted for this purpose. Another sensor can be used, such as e.g. infrared barrier.

### Program functionality:

**The program has two modes:**  

- operating mode: DIP1-OFF
- test mode: DIP1-ON

**Operating mode:**  

The program responds to the leading edge of the PIR sensor at input 1 (A1). After this edge, the relay intended for switching the pump of the disinfection cabin nozzles is switched on. At the same time, the timer tmrRUN is started, which is currently set to 5s. After the timer overflows, the relay turns off and the tmrSTOP timer starts. It is currently set to 5s. During this interval, the program ignores the input pulses. It's time to leave the cabin after disinfection.
The program sends data to the serial port such as name, version, set intervals and the number of disinfection cycles performed. A USB -> RS TTL converter is required to connect to a PC. Communication parameters are: ** 9600 1 N **

**Test mode:**  

In this mode, the relay copies the status of the sensor. In this way, it is possible to determine the real behavior of the sensor.