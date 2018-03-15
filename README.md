# Hoike
> A audio visualizer and teaching tool for the ukulele. Hōkū is Hawaiian for display.

![Hoike Logo](https://imgur.com/ZAtH7ig.png)

![YouTube Demo](#)

Hoike adds turns your analog ukulele into a audio visualizer and learning tool. With LED strips attached to the neck and
body of the ukulele, you'll be able introduce a light show to your acoustic melodies.

### Visualization Mode

Hoike reads in the current audio with a microphone and will produce a hash that gets interpretted as a HEX value for the LED strip. This will ensure that each string/chord can produce a unique and repeatable color. Below are a few examples of the recorded audio spectrum:

#### G String
![G String](https://i.imgur.com/rGY9TQ0.jpg)

#### C String
![C String](https://imgur.com/opUocq5.jpg)

#### E String
![E String](https://imgur.com/sHS8DIM.jpg)

#### A String
![A String](https://imgur.com/EISc07D.jpg)

#### C Chord
![C Chord](https://imgur.com/UPGMNuJ.jpg)

### Training Mode

Hoike also teaches the user how to play the Ukulele. With pre-programmed songs, Hoike will walk the user through how to play different chords. The LEDs will light up under the fret where the user has to put their fingers.

![Training Simulation](https://imgur.com/3pOsYPl.gif)

## Parts List
* [Ukulele](https://www.amazon.com/gp/product/B00TUMK0CM/ref=oh_aui_detailpage_o03_s00?ie=UTF8&psc=1)
* [LED Strip](https://www.adafruit.com/product/1138)
* [ATmega1284 Microcontroller](http://www.microchip.com/wwwproducts/en/ATmega1284)
* [Buttons](https://www.adafruit.com/product/1010)
* [Electret Microphone](https://www.adafruit.com/product/1713)
* [Breadboard](https://www.adafruit.com/product/65)
* LCD Screen
* Arduino Uno
* Red, Blue, Green, and Yellow LEDs


## Pinout

## Software
![State Machine](https://imgur.com/tL3I6bu.jpg)

## Complexities

### Completed Complexities:
* Hash function to map sound to red, blue, green, and brightness values
* USART between the atmega 1284 and Arduino Uno
* Used the LCD screen to act as a menu and provide user information
* Using EEPROM to store the position of learning mode. THis way the user doesn't have to skip forward to go back to where they left off.

### Incomplete Complexities:
* Addressing individual LEDs on the LED strip

## Known Bugs and Shortcomings
* Some LEDs in the fret board were blown while testing so many chord combinations cannot be shown
  * Can be fixed by using the correct resistors and rewiring
* LED strip sometimes does not turn off after switching from visualization mode to home 
  * Need to send zero'd values before switching from visualization mode to home mode over USART
* Microphone input has a lot of noise, even in a quiet room the lights still react
  * Read in a baseline value (most likely in amplitude) and only visualize the difference between the baseline and the current amplitude

## Future Work
* Making the wiring nicer and more compact so the Ukulele can be portable
* Adding a tuner, lights will be red if you're flat, blue if you're sharp, and green if you're in tune
