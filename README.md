# bottleTimer
arduino project to build timer and counter for my modified soda machine

this code is for a device that uses an arduino nano, a 4 bit 8 segment display, 5 buttons, a buzzer, and a red LED.


I 3d printed a case, and will include a link to pictures once they are up.


the device keeps track of how many completed cycles there have been.


initial state is on displaying 0000,
press the left front button and start a 60 second timer that ends with a beep and turning on the indicator LED,
then press the right front button and start a 90 second timer ending with 2 beeps and turning off the indicator and incrementing the counter by 1.


pressing the left rear button will decrease the count, and pressing the right rear button will increase it.

after each of these changes the total is stored in EEPROM so the count maintains through power failure.


holding the left button triggers a counter reset and 8899 shows on the display indicating a confirmation request, left front cancels and leaves the count the same, right front confirms and sets the count back to 0 and stores in EEPROM.


holding the right rear button triggers a counter set feature that will quickly scroll through numbers and can be cancelled or confirmed in the same way as above.


the right button is the power save button, currently it just turns off the display, but TODO: add in sleep functions.


this project makes use of the analog multi-button library by https://github.com/dxinteractive/AnalogMultiButton


