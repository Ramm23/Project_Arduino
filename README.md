
# Mailbox Authentication System 

Project for the course 34338 Telecommunication programming with Arduino. The project is intended to be used with an Arduino Uno and Esp8266 as the main boards, including the following components: photo-resistor, servo motor, RFID reader, numberpad, red and green LEDs . It is meant as a prototype of and RFID base mailbox security system with web-base features.


## Demo

YouTube link: https://youtube.com/shorts/znpJ9I9Wxrc?feature=share


## Installation

Install [Mailbox.ino](https://github.com/Ramm23/Project_Arduino/blob/main/Mailbox/Mailbox.ino) in the Esp8266 and the [I2C_Slave_v2.ino](https://github.com/Ramm23/Project_Arduino/blob/main/Master_slave%20code/I2C_Slave_v2/I2C_Slave_v2.ino) into the Arduino Uno. Follow the circuit layout as specified on  [Master_slave_setup.fzz](https://github.com/Ramm23/Project_Arduino/blob/main/Master_slave%20code/Master_slave_setup.fzz) and lastly modify the Authentication Tokens and ssd and password on the Mailbos.ino file.
    
## Features

- RFID Authentication
- Numpad Authentication
- Mail counter
- Self-lock mechanism
- UI in both Web Browser and Mobile App 


## Authors

- [@RomelMeza](https://github.com/Ramm23)
- [@FrederikTrier](https://github.com/FrederikTrier)
- [@JohanitaJanser](https://github.com/johanitajanse)
- [@MaxBrazhnyy](https://github.com/maxbrazhnyy)
- [@HenrikHynkemejer](https://github.com/HenrikHynkemejer)

