# Robocop advanced mCore mbot example #

![](http://cdn2.bigcommerce.com/server2600/4r4weyxd/product_images/uploaded_images/assembly.gif?t=1439981022)

This is an advanced example for the [Makeblock mbot](http://www.makeblock.cc/mbot/) platform. It is designed to demonstrate multithreading and complex computing on the Arduino platform.

Note that this code is NOT built using the standard mbot SCRATCH based programming environment.  Instead, it is written in C++ using the standard Arduino IDE. 

## Requried libraries ##
You will need to install the following libraries into the Arduino environment in order to build this application ([Arduino library installation tutorial](http://learn.adafruit.com/adafruit-all-about-arduino-libraries-install-use "A tutorial on how to properly install a library in the Arduino IDE environment")):

* [Makeblock library for Arduino](http://learn.makeblock.com/Makeblock-library-for-Arduino/ "Makeblock library for Arduino")
* [Adafruit NeoPixel Library](https://learn.adafruit.com/adafruit-neopixel-uberguide/arduino-library-installation "Adafruit NeoPixel Library")

The following additional tools are already included in the repository:

* [protothreads](http://dunkels.com/adam/pt/ "Protothreads are extremely lightweight stackless threads designed for severely memory constrained systems") lightweight multitasking
* a simple array/queue template class (in array.h)
* a tool for easily creating LED matrix graphics (in matrix graphic design.xlsx)

## Functionality ##

The basic functionality is to create a robot that simultaneously does the following:

* drives around (either line following or obstacle avoidance)
* alternates red & blue lights (RGB LEDs)
* makes (a poor rendition of) a siren sound
* moves the 'eyes' around as appropriate to the activity (using the LED matrix)

Line following has been implemented but not obstacle avoidance.

Additional functionality planned, but not yet implemented includes a "beep beep beep" sound when backing up during obstacle avoidance and turning the light 'white' like backup lights.

