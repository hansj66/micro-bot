# Micro:bit sumo robot
Firmware for the [TimeExpander.com](https://www.timeexpander.com/) Micro:bit based robot

The bot features two motor bridges, 3 x time of flight sensors (optional), 4 x reflective object sensors (optional) and a GPIO Expander. 

The jury is still out regarding the need for three time-of-flight sensors. The three front sensors allows the bot to home in on targets faster (without having to rotate on its axis and scanning for objects). The tradeoff being spending more time talking to sensors...

The bot is fully functional, but using PWM motor control seems to cause random crashes after a minute or two of running. I haven't been able to track this bug down yet (but, rest assured - I am going to ;)). It may be caused by a layout issue in the PCB, a missing decoupling capacitor or two, or a problem with the PWM driver in the framework. 

# Bill of materials
* 1 x Main PCB (TimeExpander.com)
* 4 x line sensor PCBs (TimeExpander.com)
* 2 x Wheels (AliExpress)
* 2 x N20 motors (AliExpress)
* 2 x N20 motor bracket assembly (AliExpress)
* 3 x VL53L0X time of flight sensors on a carrier PCB (AliExpress)
* 1 x TCA9539 IO expander (Digikey)
* 1 x TXB0106PWR Bi-directional level translator  (Digikey)
* 4 x QRE1113GR Reflective optical sensors  (Digikey)
* 1 x 100uF aluminium capacitor
* 1 x 2x5 pin 2.54mm female header (Digikey)
* 1 x battery pack for 4x AAA  (Digikey)
* 1 x JS202011SCQN slide switch  (Digikey)
* 1 x SOT223 LM1117 3.3V regulator (Digikey)
* 1 x Mini caster assembly (AliExpress + 3D printed bracket)
* 2 x 3D printed spacers (between motors and PCB)
* 1 x 12 pin male breakaway header  (Digikey)
* 3 x 6 pin RA breakaway header  (Digikey)
* 1 x hook and loop tape (for attaching battery pack)
* 2 x 100nf capacitors (motor noise suppression)

![Sumobot front](https://github.com/hansj66/micro-bot/raw/master/images/sumo_front.jpg)

![Sumobot front](https://github.com/hansj66/micro-bot/raw/master/images/sumo_rear.jpg)

![Sumobot front](https://github.com/hansj66/micro-bot/raw/master/images/sumo_bottom.jpg)

