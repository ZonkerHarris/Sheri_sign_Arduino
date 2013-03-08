Sheri_sign_Arduino
==================

Using SPI, and MaceTech Shiftbrite shift-register LEDs

As a tip, try downloading the repository, rather than just copy-n-pasting from the code displays. I've found hidden-character artifacts trying to copy from GIT display windows, which caused me grief. 
Avoid the grief by downloading the bundle. :-)

This was the project which pushed me through the learning curve for SPI, and these amazing LEDs from MaceTech.

The sign included 35 LEDs, forming 5 letters (spelling "Sheri"). 
Pictures at  https://www.dropbox.com/sh/7zkbe0wx45x5hkk/uxpgX4-rap

Some animations work on the full range, others treat each letter individually. A couple treat the "S" one way, and the rest of the letters another way.

Because of these different conditions, most of the functions take low- and high-pixel arguments, which makes them flexible!

In my rush to create the animations, I guessed about color values. But, I guessed wrong. Since I was on a short deadline, I fudged a bit.
Most of the animation here only use the middle-three colors in each 5-color array. They look OK.
After the sign was delivered, I built a "color-tuner", which let me use three potentiometers to pick a shade/hue/brightness.
Using an CLD display, I could note the colors. And I used 6 Shiftbrites, so I could compare some existing values to the new color I was tuning.
The result of that tuning project was an improved color library, which I've included here.
