
/*
  Version 1.9  -  
  Expanding the Random calls, and add more colors
  Updated for the full sign, running on Arduino Pro Mini 326
  Standardized some naming schemes
  Random = pick random colors
  Write = scribe with white, each element at a time, fade to a color
  Pop = set all elements, then write the array at once
  Flash = Write all pixels white, write the array, then write the color and write the array
  ___ pixels = apply this to some subset of pixels, one at a time
  ___ letters = perform this function, one letter at a time
  ___ word = perform this function on all of the pixels
  
  This uses the original ShiftBrite demo code to write the array.
  It's using the default ShiftBrite Shield pinouts, and you do
  need the extra power if you are using more than three ShiftBrites.
  
  The sign will write characters the way the tip of the pen draws
  while you write (left to write). This means that the pixels
  must be wired right to left, in the inverse order of drawing. */

//Define constants which should NEVER change (maybe use "const" instead?)
#define clockpin 13 // CI
#define enablepin 10 // EI
#define latchpin 9 // LI
#define datapin 11 // DI
#define NumLEDs 35   // how many LEDs are defined in each section below? 

//Define 'Globals' (variables available to all functions)
// first, for the LED array...
int LEDChannels[NumLEDs][3] = {0};
int SB_CommandMode;
int SB_RedCommand;
int SB_GreenCommand;
int SB_BlueCommand;

// next, for the variable we want to pass around...
int delaytime = 50;  // in milliseconds, between shifts...
int ipDelay = 50;  // inter-pixel delay...
int icDelay = 750;  // inter-character delay...
int nameDelay = 2000; // delay between sequences...
int lpixel = 0;  //  Remember, lowest number will be the LAST in the chain!
int hpixel = 34;  // The highest number is clocked out last; the 1st in the chain!
int letter1l = 0;  // the first pixel in the first character... (S)
int letter1h = 7;  // the first pixel in the first character...
int letter2l = 8;  // the first pixel in the first character... (h)
int letter2h = 16;  // the first pixel in the first character...
int letter3l = 17;  // the first pixel in the first character... (e)
int letter3h = 24;  // the first pixel in the first character...
int letter4l = 25;  // the first pixel in the first character... (r)
int letter4h = 30;  // the first pixel in the first character...
int letter5l = 31;  // the first pixel in the first character... (i)
int letter5h = 34;  // the first pixel in the first character...
int myLight = (analogRead(3));  // we'll be calling this regularly, to see if it's dusk...
//  (This is a photo-diode from vcc to A3 input, with an internal pullup assigned)

/* Prepare the colors array
 myBase is modulo 15, with R-G-B values, from brightest to dimmest
 0=white, 15=red, 30=yellow, 45=green, 60=blue, 75=teal, 90=purple
 value 135=black (all zeroes).
 myColorCode is modulo 3, defining one shade of a given color in R-G-B.
 You can use any modulo-3 number in the array, from 0 (white) to 181 (black).
 18=almost-brightest red, 51 is medium-green, 72 is dimmest-blue. */
int myBase;
int myColorCode;
int myColors[] = {
  1023, 1023, 1023,
  800, 800, 800,
  600, 600, 600,
  400, 400, 400,
  200, 200, 200,
  1023, 1023, 700,
  1023, 900, 300,
  1023, 700, 0,
  700, 500, 0,
  300, 300, 0,
  900, 1023, 700,
  700, 900, 300,
  500, 700, 0,
  300, 500, 0,
  0, 300, 0,
  1023, 700, 700,
  900, 300, 300,
  800, 0, 0,
  600, 0, 0,
  300, 0, 0,
  1023, 700, 1023,
  900, 550, 900,
  700, 300, 700,
  500, 200, 500,
  300, 100, 300,
  1023, 00, 700,
  900, 0, 550,
  700, 0, 300,
  500, 0, 200,
  300, 0, 100,
  700, 1023, 900,
  300, 1023, 700,
  0, 1023, 500,
  0, 700, 300,
  0, 300, 0,
  1023, 1023, 700,
  900, 900, 300,
  700, 700, 0,
  500, 500, 0,
  300, 300, 0,
  700, 1023, 700,
  300, 900, 300,
  0, 900, 0,
  0, 600, 0,
  0, 300, 0,
  700, 1016, 200,
  300, 1008, 200,
  0, 1000, 200,
  0, 700, 130,
  0, 300, 60,
  700, 700, 1023,
  300, 300, 1023,
  0, 0, 1023,
  0, 0, 700,
  0, 0, 300,
  980, 700, 1023,
  940, 300, 1023,
  900, 0, 1023,
  600, 0, 700,
  200, 0, 300,
  0};

int lightPin = 3;   // Cadmium photo-cell (light detector)

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void setup() {
 
   pinMode (A0, INPUT);  // just an antenna wire, to get random results as a seen
   pinMode(A3, INPUT);  // IR Photo-diode tied to 5-volts
   digitalWrite(A3, LOW);  // set pulldown on analog pin 3 
   int randomSeed(analogRead(0)); // give us a fairly random seed to start...
   pinMode(datapin, OUTPUT);   // should be 11, for the MaceTech ShiftBrite Shield
   pinMode(latchpin, OUTPUT);   // should be 9...
   pinMode(enablepin, OUTPUT);   // should be 10
   pinMode(clockpin, OUTPUT);  // should be 13...
   SPCR = (1<<SPE)|(1<<MSTR)|(0<<SPR1)|(0<<SPR0);
   digitalWrite(latchpin, LOW);
   digitalWrite(enablepin, LOW);
   // Adding the serial code, to display which function is active
   Serial.begin(9600);    // setup the serial monitor
}

void loop() {
Serial.println("NormalPatern 1");
int myBase = randomSmall();
flashfade (myBase, lpixel, hpixel);
 delay(nameDelay);
Serial.println("NormalPatern 2");
myBase = randomSmall();
writefade (myBase, lpixel, hpixel);
 delay(icDelay);
Serial.println("NormalPatern 3");
myBase = randomSmall();
writecolor (myBase, lpixel, hpixel);
 delay(nameDelay);
Serial.println("NormalPatern 4");
randomletters (3, 15);  // loop count, inter-letter delay in ms
 delay(nameDelay);
Serial.println("NormalPatern 5");
myBase = randomSmall();
writecolor (myBase, lpixel, hpixel);
 delay(nameDelay);
Serial.println("NormalPatern 6");
myBase = randomSmall();
writecolor (myBase, lpixel, hpixel);
Serial.println("NormalPatern 7");
myBase = randomSmall();
flashfade (myBase, lpixel, hpixel);
 delay(nameDelay);
Serial.println("NormalPatern 8");
popcolor (lpixel, hpixel);
 delay (ipDelay);
writeblack(lpixel, hpixel);
 delay(nameDelay);
Serial.println("NormalPatern 9");
myBase = randomSmall();
flashfade (myBase, lpixel, hpixel);
 delay(nameDelay);
Serial.println("NormalPatern 10");
myBase = randomSpread();
flashpixels (myBase, lpixel, hpixel, 70);
Serial.println("NormalPatern 11");
myBase = randomSmall();
writecolor (myBase, 0, 7);
myBase = randomSmall();
writecolor (myBase, 8, 34);
 delay(nameDelay);
Serial.println("NormalPatern 12");
poppixels (120);
 delay(icDelay);
myBase = randomSpread();
writecolor (myBase, lpixel, hpixel);
 delay(nameDelay);
Serial.println("NormalPatern 13");
randomletters (4, 15);  // loop count, inter-letter delay in ms
 delay(nameDelay);
Serial.println("NormalPatern 14");
myBase = randomSpread();
flashcolor (myBase, lpixel, hpixel);
 delay(nameDelay);
Serial.println("NormalPatern 15");
popcolor (lpixel, hpixel);
writeblack(lpixel, hpixel);
 delay(nameDelay);
Serial.println("NormalPatern 16");
myBase = randomSmall();
writefade (myBase, lpixel, hpixel);
 delay(nameDelay);
Serial.println("NormalPatern 17");
myBase = randomSmall();
writefade (myBase, lpixel, hpixel);
 delay(nameDelay);
Serial.println("NormalPatern 18");
myBase = randomSpread();
writecolor (myBase, 0, 7);
myBase = randomSmall();
writecolor (myBase, 8, 34);
 delay(nameDelay);
Serial.println("NormalPatern 19");
myBase = randomSmall();
writecolor (myBase, lpixel, hpixel);
 delay(nameDelay);
Serial.println("NormalPatern 20");
poppixels (120);
 delay(icDelay);
writeblack(lpixel, hpixel);
 delay(nameDelay);
Serial.println("NormalPatern 21");
myBase = randomSpread();
flashpixels (myBase, lpixel, hpixel, 70);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int randomSmall() {
  int result;
  result = (random(1, 11) * 15);
  return result;
}
  
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int randomBig() {
  int result;
  result = (random(5, 59) * 3);
  return result;
}
  
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int randomSpread() {
  int result;
  int mySpread = (2, 4);
  result = (random(1,11) * 15) + (3 * mySpread);
  return result;
}
  
  // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void flashcolor(int myBaseColor, int myLpixel, int myHpixel) {
  /* Write white to the array, then write a random color to the array
     suggesting int myBaseColor = (random(1, 34) * 3); */
Serial.print("flashcolor\t");
Serial.println(myBaseColor);
for (int pixel = myLpixel; pixel < myHpixel + 1; pixel++)  
  {
    LEDChannels[pixel][0] = 1023;
    LEDChannels[pixel][1] = 1023;
    LEDChannels[pixel][2] = 1023;
  }
WriteLEDArray();
 delay(icDelay);
for (int pixel = myLpixel; pixel < myHpixel + 1; pixel++)  
  {
    LEDChannels[pixel][0] = myColors[myBaseColor];
    LEDChannels[pixel][1] = myColors[myBaseColor + 1];
    LEDChannels[pixel][2] = myColors[myBaseColor + 2]
    ;
  }
WriteLEDArray();
delay(nameDelay);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void flashfade(int myBaseColor, int myLpixel, int myHpixel) {
/* We take in a base color, and a range of LEDs, likely the fulll string.
   We then write "white" to the array, then the brightest color to
   the the array, and then the second-brightest color to the array. 
   Then we pause for nameDelay, and then, we write the next dimmest
   color to the array, then the dimmest color, and then black
   At the end of the function, all elements in the range should be off.*/
Serial.print("flashfade\t");
Serial.println(myBaseColor);
for (int pixel = myLpixel; pixel < myHpixel + 1; pixel++)  
  {
    LEDChannels[pixel][0] = 1023;
    LEDChannels[pixel][1] = 1023;
    LEDChannels[pixel][2] = 1023;
  }
WriteLEDArray();
 delay(ipDelay);
for (int pixel = myLpixel; pixel < myHpixel + 1; pixel++)  
  {
    LEDChannels[pixel][0] = myColors[myBaseColor];
    LEDChannels[pixel][1] = myColors[myBaseColor + 1];
    LEDChannels[pixel][2] = myColors[myBaseColor + 2];
  }
WriteLEDArray();
 delay(ipDelay);
for (int pixel = myLpixel; pixel < myHpixel + 1; pixel++)  
  {
    LEDChannels[pixel][0] = myColors[myBaseColor + 3];
    LEDChannels[pixel][1] = myColors[myBaseColor + 4];
    LEDChannels[pixel][2] = myColors[myBaseColor + 5];
  }
WriteLEDArray();
 delay(ipDelay);
for (int pixel = myLpixel; pixel < myHpixel + 1; pixel++)  
  {
    LEDChannels[pixel][0] = myColors[myBaseColor + 6];
    LEDChannels[pixel][1] = myColors[myBaseColor + 7];
    LEDChannels[pixel][2] = myColors[myBaseColor + 8];
  }
WriteLEDArray();
 delay(ipDelay);
for (int pixel = myLpixel; pixel < myHpixel + 1; pixel++)  
  {
    LEDChannels[pixel][0] = myColors[myBaseColor + 9];
    LEDChannels[pixel][1] = myColors[myBaseColor + 10];
    LEDChannels[pixel][2] = myColors[myBaseColor + 11];
  }
WriteLEDArray();
 delay(ipDelay);
for (int pixel = myLpixel; pixel < myHpixel + 1; pixel++)  
  {
    LEDChannels[pixel][0] = myColors[myBaseColor + 12];
    LEDChannels[pixel][1] = myColors[myBaseColor + 13];
    LEDChannels[pixel][2] = myColors[myBaseColor + 14];
  }
WriteLEDArray();
 delay(nameDelay);
for (int pixel = myLpixel; pixel < myHpixel + 1; pixel++)  
  {
    LEDChannels[pixel][0] = 0;
    LEDChannels[pixel][1] = 0;
    LEDChannels[pixel][2] = 0;
  }
WriteLEDArray();
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void flashpixels(int myBaseColor, int myLpixel, int myHpixel, int count) {
/* Tell me the base color, range, and how many pixels to flash. I'll write the
  color to the array range (one at a time), and then I'll pick random
  picels to Flash (set to white, wait, then set back to the color. */
Serial.print("flashpixels, color = ");
Serial.print(myBaseColor);
Serial.print(" ,  count = ");
Serial.println(count);
for (int pixel = myLpixel; pixel < myHpixel + 1; pixel++)  
  {
    LEDChannels[pixel][0] = myColors[myBaseColor];
    LEDChannels[pixel][1] = myColors[myBaseColor + 1];
    LEDChannels[pixel][2] = myColors[myBaseColor + 2];
    WriteLEDArray();
  }

for (int counter = 0; counter < count; counter++)
  {
     int pixel = random(myLpixel, myHpixel+1);
     LEDChannels[pixel][0] = 900;
     LEDChannels[pixel][1] = 900;
     LEDChannels[pixel][2] = 900;
     WriteLEDArray();
     delay(ipDelay);
     LEDChannels[pixel][0] = myColors[myBaseColor];
     LEDChannels[pixel][1] = myColors[myBaseColor + 1];
     LEDChannels[pixel][2] = myColors[myBaseColor + 2];
     WriteLEDArray();
     delay(ipDelay);
  }
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void poppixels(int count) {
/* Just tell me how many pixels I get to pop, and I'll randomly choose
  which pixel, and which color to make it. */
Serial.print("poppixels, count = ");
Serial.println(count);
/*for (int pixel = lpixel; pixel < hpixel + 1; pixel++)  
  {
    int myBaseColor = randomSpread();
    LEDChannels[pixel][0] = myColors[myBaseColor];
    LEDChannels[pixel][1] = myColors[myBaseColor + 1];
    LEDChannels[pixel][2] = myColors[myBaseColor + 2];
  }
WriteLEDArray();
*/
for (int counter = 0; counter < count; counter++)
  {
     int myBaseColor = randomSpread();
     int pixel = random(lpixel, hpixel+1);
     LEDChannels[pixel][0] = myColors[myBaseColor];
     LEDChannels[pixel][1] = myColors[myBaseColor + 1];
     LEDChannels[pixel][2] = myColors[myBaseColor + 2];
     WriteLEDArray();
     delay(ipDelay);
  }
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void popcolor(int myLpixel, int myHpixel) {
  // Write a random color to all pixels, then write the array
int myBaseColor = randomSpread();
Serial.print("popcolor\t");
Serial.println(myBaseColor);
for (int pixel = myLpixel; pixel < myHpixel + 1; pixel++)  
  {
    LEDChannels[pixel][0] = myColors[myBaseColor];
    LEDChannels[pixel][1] = myColors[myBaseColor + 1];
    LEDChannels[pixel][2] = myColors[myBaseColor + 2];
  }
WriteLEDArray();
delay(nameDelay);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void popcolorblack(int myLpixel, int myHpixel) {
  // Write a random color to all pixels, then write the array
int myBaseColor = randomSpread();
Serial.print("popcolor\t");
Serial.print(myBaseColor);
for (int pixel = myLpixel; pixel < myHpixel + 1; pixel++)  
  {
    LEDChannels[pixel][0] = myColors[myBaseColor];
    LEDChannels[pixel][1] = myColors[myBaseColor + 1];
    LEDChannels[pixel][2] = myColors[myBaseColor + 2];
  }
WriteLEDArray();
delay(nameDelay);
for (int pixel = myLpixel; pixel < myHpixel + 1; pixel++)  
  {
    LEDChannels[pixel][0] = 0;
    LEDChannels[pixel][1] = 0;
    LEDChannels[pixel][2] = 0;
  }
WriteLEDArray();
delay(nameDelay);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void writeblack(int myLpixel, int myHpixel) {
  // Just write one pixel at a time to black...
Serial.println("writeblack");
for (int pixel = myLpixel; pixel < myHpixel + 1; pixel++)  
  {
    LEDChannels[pixel][0] = 0;
    LEDChannels[pixel][1] = 0;
    LEDChannels[pixel][2] = 0;
    WriteLEDArray();
    delay(ipDelay);
  }
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void writecolor(int myBaseColor, int myLpixel, int myHpixel) {
/* We take in a range of LEDs, likely the fulll string, and a base color.
   We then write "white" to the first pixel, then the brightest color to
   the pixel behind it, and then the second-brightest color to the pixel 
   behind that, until all pixels are at the medium-bright, solid color.
   (The sequence goes higher than the range, to make sure that the last
   trailing pixel get to the dimmer color...).
   At the end of the function, all elements in the range should be set
   to the all-solid color set by the base color.
   There is no name delay, presuming that this function is for writing
   a single letter (a subset of the string).*/
Serial.print("writecolor\t");
Serial.println(myBaseColor);
for (int active = myLpixel; active < (myHpixel + 3); active++)
  {
    if (active > (myLpixel - 1)  && active < (myHpixel+ 1))
    {
      LEDChannels[active][0] = 1023;
      LEDChannels[active][1] = 1023;
      LEDChannels[active][2] = 1023;
    }

    if (active > myLpixel  && active < (myHpixel+ 2))
    {
      LEDChannels[active-1][0] = myColors[myBaseColor + 3];
      LEDChannels[active-1][1] = myColors[myBaseColor + 4];
      LEDChannels[active-1][2] = myColors[myBaseColor + 5];
    }

    if (active > (myLpixel + 1)  && active < (myHpixel+ 3))
    {
      LEDChannels[active-2][0] = myColors[myBaseColor + 6];
      LEDChannels[active-2][1] = myColors[myBaseColor + 7];
      LEDChannels[active-2][2] = myColors[myBaseColor + 8];
    }
   WriteLEDArray();
   delay(ipDelay);
  } 
  delay(icDelay);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void writefade(int myBaseColor, int myLpixel, int myHpixel) {
/* We take in a range of LEDs, likely the full string, and a base color.
   We then write "white" to the first pixel, then the brightest color to
   the pixel behind it, and then the second-brightest color to the pixel 
   behind that, until all pixels are at the medium-bright, solid color.
   (The sequence goes higher than the range, to make sure that the last
   trailing pixel get to the dimmer color...) Then, we go back to the 
   beginning of the sequence, and dim them all to black using another 
   2-step trailing pixel method.
   At the end of the function, all elements in the range should be off.*/
Serial.print("writefade\t");
Serial.println(myBaseColor);
for (int active = myLpixel; active < (myHpixel + 3); active++)
  {
    if (active > (myLpixel - 1)  && active < (myHpixel+ 1))
    {
      LEDChannels[active][0] = 1023;
      LEDChannels[active][1] = 1023;
      LEDChannels[active][2] = 1023;
    }

    if (active > myLpixel  && active < (myHpixel+ 2))
    {
      LEDChannels[active-1][0] = myColors[myBaseColor + 3];
      LEDChannels[active-1][1] = myColors[myBaseColor + 4];
      LEDChannels[active-1][2] = myColors[myBaseColor + 5];
    }

    if (active > (myLpixel + 1)  && active < (myHpixel+ 3))
    {
      LEDChannels[active-2][0] = myColors[myBaseColor + 6];
      LEDChannels[active-2][1] = myColors[myBaseColor + 7];
      LEDChannels[active-2][2] = myColors[myBaseColor + 8];
    }
   WriteLEDArray();
   delay(ipDelay);
  } 
  delay(icDelay);

  for (int active = myLpixel; active < (myHpixel + 3); active++)
  {
    if (active > (myLpixel - 1)  && active < (myHpixel+ 1))
    {
      LEDChannels[active][0] = myColors[myBaseColor + 9];
      LEDChannels[active][1] = myColors[myBaseColor + 10];
      LEDChannels[active][2] = myColors[myBaseColor + 11];
    }

    if (active > myLpixel  && active < (myHpixel+ 2))
    {
      LEDChannels[active-1][0] = myColors[myBaseColor + 12];
      LEDChannels[active-1][1] = myColors[myBaseColor + 13];
      LEDChannels[active-1][2] = myColors[myBaseColor + 14];
    }

    if (active > (myLpixel + 1)  && active < (myHpixel+ 3))
    {
      LEDChannels[active-2][0] = 0;
      LEDChannels[active-2][1] = 0;
      LEDChannels[active-2][2] = 0;
    }
   WriteLEDArray();
   delay(ipDelay);
  } 
  delay(icDelay);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void randomletters(int myCount, int myDelay) {
/* take each letter in sequence, pick a random color, send it to the
   writecolor function, until all letters have been written as many 
   times as requested.*/
Serial.print("randomletters, count = ");
Serial.println(myCount);
for (int counter = 0; counter < myCount; counter++)
{
  Serial.print("randomletters pass ");
  Serial.print(counter);
  Serial.print(" of ");
  Serial.println(myCount);
  myBase = randomSmall();
  writecolor (myBase, letter1l, letter1h);
  delay(myDelay);
  myBase = randomSmall();
  writecolor (myBase, letter2l, letter2h);
  delay(myDelay);
  myBase = randomSmall();
  writecolor (myBase, letter3l, letter3h);
  delay(myDelay);
  myBase = randomSmall();
  writecolor (myBase, letter4l, letter4h);
  delay(myDelay);
  myBase = randomSmall();
  writecolor (myBase, letter5l, letter5h);
  delay(nameDelay);
  }
Serial.println("randomletters is done.");
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 The following code is from the original ShiftBrite demo code, by
Garret Mace, at http://docs.macetech.com/doku.php/shiftbrite_2.0
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void SB_SendPacket() {
 
    if (SB_CommandMode == B01) {
     SB_RedCommand = 120;
     SB_GreenCommand = 100;
     SB_BlueCommand = 100;
    }
 
    SPDR = SB_CommandMode << 6 | SB_BlueCommand>>4;
    while(!(SPSR & (1<<SPIF)));
    SPDR = SB_BlueCommand<<4 | SB_RedCommand>>6;
    while(!(SPSR & (1<<SPIF)));
    SPDR = SB_RedCommand << 2 | SB_GreenCommand>>8;
    while(!(SPSR & (1<<SPIF)));
    SPDR = SB_GreenCommand;
    while(!(SPSR & (1<<SPIF)));
}
 
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void WriteLEDArray() {
 
    SB_CommandMode = B00; // Write to PWM control registers
    for (int h = 0;h<NumLEDs;h++) {
    SB_RedCommand = LEDChannels[h][0];
	  SB_GreenCommand = LEDChannels[h][1];
	  SB_BlueCommand = LEDChannels[h][2];
	  SB_SendPacket();
    }
 
    delayMicroseconds(15);
    digitalWrite(latchpin,HIGH); // latch data into registers
    delayMicroseconds(15);
    digitalWrite(latchpin,LOW);
 
    SB_CommandMode = B01; // Write to current control registers
    for (int z = 0; z < NumLEDs; z++) SB_SendPacket();
    delayMicroseconds(15);
    digitalWrite(latchpin,HIGH); // latch data into registers
    delayMicroseconds(15);
    digitalWrite(latchpin,LOW);
}

// * * * * * * * * * * < End of File > * * * * * * * * * * * * * * *
