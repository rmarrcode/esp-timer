# esp-timer
## Obtain MAC address of timers -> measure latency of esp32 timers -> achieve accurate timing resuts. This project was used for timing mousetrap cars for division B science olympiad.
### the control sends a signal to the timer which will wait for 10 seconds and send a signal back. This time is then compared to an ideal 10 seconds to determine latency of devices
# To use:
* Establish which of the ESP modules will have which roles. 
* Obtain the MAC adresses using getmac.ino. 
* Replace addresses of timeraddr and controladdr to accomodate this
* The timer ESP is to be started first so that once the control sends the first packet it is ready to recieve it and handle the movement of data from there. The amount of times the ESPs will send data back and forth is dependent on the TEST_REPEAT global in control.ino.  

