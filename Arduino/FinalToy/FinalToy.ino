//Sal Sorice
//ECE387 Final toy prototype project 
//Code created using LPD8806 library from Adafruit, and using example code 
//provided with that library.
//February 8, 2013

#include "LPD8806.h"
#include "SPI.h"
//Pin to use for the ping
int pingPin=11;

// variables
unsigned int duration;
unsigned int inches;
int delayTime;
int brightness;

//Switch pins for controlling the mode of the lights
int switchOne = 5;
int switchTwo = 6;

//Length of YOUR strand in meters
//int sLeng = 2;

//Number of RBG LEDs per 1 meter strand
//int LEDs_perStrand = 32;

//Number of LEDs
//int nLEDs = sLeng*LEDs_perStrand;

int nLEDs=58;

//Output pins for clock and data
int dataPin = 2;    //White wire
int clockPin = 3;   //Green wire

//Create an led strip object, param 1 is # of LEDs, parm 2 is data pin,
//param 3 is clock pin
LPD8806 strip = LPD8806(nLEDs, dataPin, clockPin);

// You can optionally use hardware SPI for faster writes, just leave out
// the data and clock pin parameters.  But this does limit use to very
// specific pins on the Arduino.  For "classic" Arduinos (Uno, Duemilanove,
// etc.), data = pin 11, clock = pin 13.  For Arduino Mega, data = pin 51,
// clock = pin 52.  For 32u4 Breakout Board+ and Teensy, data = pin B2,
// clock = pin B1.  For Leonardo, this can ONLY be done on the ICSP pins.
//LPD8806 strip = LPD8806(nLEDs);

void setup() {
  //Serial monitor
  Serial.begin(9600);
  //Seed the random function
  randomSeed(analogRead(0));
  //Sets up the switch pins as inputs, and 
  //enables the built in pull down resistors for the switches
  pinMode(switchOne,INPUT);
  pinMode(switchTwo,INPUT);
  digitalWrite(switchOne,HIGH);
  digitalWrite(switchTwo,HIGH);
  
  // Start up the LED strip
  strip.begin();

  // Update the strip, to start they are all 'off'
  strip.show();
}

void loop(){
  //Program has three modes to determine what should be displayed
  //Which mode are we in?
  //Mode 1
  //Test cylce
  
  Serial.println(digitalRead(switchOne));
  Serial.println(digitalRead(switchTwo));
  if(digitalRead(switchOne) == LOW && digitalRead(switchTwo)==LOW){
//    Serial.println("Mode 1");
    testCycle();
    
  }
  //Mode two
  //Random cycle
  else if (digitalRead(switchOne) == HIGH && digitalRead(switchTwo)==LOW){
//    Serial.println("Mode Two");
    randomCycle(random(1,5));
    
  }
  //Mode 3
  //Ping Cycle
  else if(digitalRead(switchOne) == LOW && digitalRead(switchTwo)==HIGH){
//    Serial.println("Mode 3");
    pingCycle();
  }
  //Flashing lights!
  else {
//    Serial.println("Mode four");
    flashCycle();
  } 
} 

void flashCycle(){
  int brightness=127;
  colorFlash(strip.Color(brightness, brightness, brightness)); // White
  clearStrip();
  colorFlash(strip.Color(brightness,   0,   0)); // Red
  clearStrip();
  colorFlash(strip.Color(brightness, brightness,   0)); // Yellow
  clearStrip();
  colorFlash(strip.Color(  0, brightness,   0)); // Green
  clearStrip();
  colorFlash(strip.Color(  0, brightness, brightness)); // Cyan
  clearStrip();
  colorFlash(strip.Color(  0,   0, brightness)); // Blue
  clearStrip();
  colorFlash(strip.Color(brightness,   0, brightness)); // Violet
  clearStrip();	
  for(int j =0; j<10; j++){
  colorFlash(strip.Color(brightness, brightness, brightness)); // White
  colorFlash(strip.Color(brightness,   0,   0)); // Red
  colorFlash(strip.Color(brightness, brightness,   0)); // Yellow
  colorFlash(strip.Color(  0, brightness,   0)); // Green
  colorFlash(strip.Color(  0, brightness, brightness)); // Cyan
  colorFlash(strip.Color(  0,   0, brightness)); // Blue
  colorFlash(strip.Color(brightness,   0, brightness)); // Violet
  }
   // Color sparkles
  dither(strip.Color(0,127,127), 0);       // cyan
  dither(strip.Color(0,0,0), 0);           // black
  dither(strip.Color(127,0,127), 0);       // magenta
  dither(strip.Color(0,0,0), 0);           // black
  dither(strip.Color(127,127,0), 0);       // yellow
  dither(strip.Color(0,0,0), 0);           // black
  
  
  
   // Back-and-forth lights
  scanner(127, 127, 127, 5);        
  scanner(127,   0,   0, 5);        
  scanner(127, 127,   0, 5);      
  scanner(  0, 127,   0, 5);     
  scanner(  0, 127, 127, 5);    
  scanner(  0,   0, 127, 5);   
  scanner(127,   0, 127, 5);        
  
   // Wavy ripple effects
  wave(strip.Color(127,0,0), 4, 0);        // candy cane
  wave(strip.Color(0,0,100), 2, 0);        // icy
}


void randomCycle(int selection){
  Serial.println(selection);
  if (selection == 1){
    testCycle();
  }
  else if(selection == 2){
    pingCycle();
  }
  else if(selection == 3){
    flashCycle();
  }
  else{
     randomCycle(random(1,3)); 
  }
  
}
//-------------------------------------------//
//    Good to go at this point, test and     //
//    Just need to determine max distance    //
//-------------------------------------------//
void pingCycle(){
  pinMode(pingPin, OUTPUT);          // Set pin to OUTPUT
  digitalWrite(pingPin, LOW);        // Ensure pin is low
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);       // Start ranging
  delayMicroseconds(5);              //   with 5 microsecond burst
  digitalWrite(pingPin, LOW);        // End ranging
  pinMode(pingPin, INPUT);           // Set pin to INPUT
  duration = pulseIn(pingPin, HIGH); // Read echo pulse
  inches = duration / 74 / 2;        // Convert to inches
  if(inches>=60){
    inches = 59;
  }
  brightness = 127*((60-inches)/(60.0));
  delayTime = 25*((inches)/(60.0));
//  Serial.println(inches);
//  Serial.println(brightness);
//  Serial.println(delayTime);
  colorWipe(strip.Color(brightness, brightness, brightness), delayTime); // White
  colorWipe(strip.Color(brightness,   0,   0), delayTime); // Red
  colorWipe(strip.Color(brightness, brightness,   0), delayTime); // Yellow
  colorWipe(strip.Color(  0, brightness,   0), delayTime); // Green
  colorWipe(strip.Color(  0, brightness, brightness), delayTime); // Cyan
  colorWipe(strip.Color(  0,   0, brightness), delayTime); // Blue
  colorWipe(strip.Color(brightness,   0, brightness), delayTime); // Violet	
}

void testCycle(){
   // Send a simple pixel chase in...
  colorChase(strip.Color(127, 127, 127), 12); // White
  colorChase(strip.Color(127,   0,   0), 12); // Red
  colorChase(strip.Color(127, 127,   0), 12); // Yellow
  colorChase(strip.Color(  0, 127,   0), 12); // Green
  colorChase(strip.Color(  0, 127, 127), 12); // Cyan
  colorChase(strip.Color(  0,   0, 127), 12); // Blue
  colorChase(strip.Color(127,   0, 127), 12); // Violet

  // Fill the entire strip with...
  colorWipe(strip.Color(127,   0,   0), 12);  // Red
  colorWipe(strip.Color(  0, 127,   0), 12);  // Green
  colorWipe(strip.Color(  0,   0, 127), 12);  // Blue

 

  rainbow(0);
  rainbowCycle(0);  // make it go through the cycle fairly fast
}


// Fill the dots progressively along the strip.
void colorFlash(uint32_t c) {
  int i;

  for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
  }
  strip.show();
  delay(200);
}

void clearStrip(){
  for (int i=0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, 0);
    strip.show();
    delay(5);
  }
}
  

//**********************************************************************************//
//                 Everything beyond this point should not be touched               //
//                 Unless you want to modify how these functions work               //
//**********************************************************************************//  
//Helper functions (borrowed from Adafruit provided sample code)
void rainbow(uint8_t wait) {
  int i, j;
   
  for (j=0; j < 384; j++) {     // 3 cycles of all 384 colors in the wheel
    for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel( (i + j) % 384));
    }  
    strip.show();   // write all the pixels out
    delay(wait);
  }
}

// Slightly different, this one makes the rainbow wheel equally distributed 
// along the chain
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;
  
  for (j=0; j < 384 * 5; j++) {     // 5 cycles of all 384 colors in the wheel
    for (i=0; i < strip.numPixels(); i++) {
      // tricky math! we use each pixel as a fraction of the full 384-color wheel
      // (thats the i / strip.numPixels() part)
      // Then add in j which makes the colors go around per pixel
      // the % 384 is to make the wheel cycle around
      strip.setPixelColor(i, Wheel( ((i * 384 / strip.numPixels()) + j) % 384) );
    }  
    strip.show();   // write all the pixels out
    delay(wait);
  }
}

// Fill the dots progressively along the strip.
void colorWipe(uint32_t c, uint8_t wait) {
  int i;

  for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

// Chase one dot down the full strip.
void colorChase(uint32_t c, uint8_t wait) {
  int i;

  // Start by turning all pixels off:
  for(i=0; i<strip.numPixels(); i++) strip.setPixelColor(i, 0);

  // Then display one pixel at a time:
  for(i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c); // Set new pixel 'on'
    strip.show();              // Refresh LED states
    strip.setPixelColor(i, 0); // Erase pixel, but don't refresh!
    delay(wait);
  }

  strip.show(); // Refresh to turn off last pixel
}

// An "ordered dither" fills every pixel in a sequence that looks
// sparkly and almost random, but actually follows a specific order.
void dither(uint32_t c, uint8_t wait) {

  // Determine highest bit needed to represent pixel index
  int hiBit = 0;
  int n = strip.numPixels() - 1;
  for(int bit=1; bit < 0x8000; bit <<= 1) {
    if(n & bit) hiBit = bit;
  }

  int bit, reverse;
  for(int i=0; i<(hiBit << 1); i++) {
    // Reverse the bits in i to create ordered dither:
    reverse = 0;
    for(bit=1; bit <= hiBit; bit <<= 1) {
      reverse <<= 1;
      if(i & bit) reverse |= 1;
    }
    strip.setPixelColor(reverse, c);
    strip.show();
    delay(wait);
  }
  delay(250); // Hold image for 1/4 sec
}

// "Larson scanner" = Cylon/KITT bouncing light effect
void scanner(uint8_t r, uint8_t g, uint8_t b, uint8_t wait) {
  int i, j, pos, dir;

  pos = 0;
  dir = 1;

  for(i=0; i<((strip.numPixels()-1) * 8); i++) {
    // Draw 5 pixels centered on pos.  setPixelColor() will clip
    // any pixels off the ends of the strip, no worries there.
    // we'll make the colors dimmer at the edges for a nice pulse
    // look
    strip.setPixelColor(pos - 2, strip.Color(r/4, g/4, b/4));
    strip.setPixelColor(pos - 1, strip.Color(r/2, g/2, b/2));
    strip.setPixelColor(pos, strip.Color(r, g, b));
    strip.setPixelColor(pos + 1, strip.Color(r/2, g/2, b/2));
    strip.setPixelColor(pos + 2, strip.Color(r/4, g/4, b/4));

    strip.show();
    delay(wait);
    // If we wanted to be sneaky we could erase just the tail end
    // pixel, but it's much easier just to erase the whole thing
    // and draw a new one next time.
    for(j=-2; j<= 2; j++) 
        strip.setPixelColor(pos+j, strip.Color(0,0,0));
    // Bounce off ends of strip
    pos += dir;
    if(pos < 0) {
      pos = 1;
      dir = -dir;
    } else if(pos >= strip.numPixels()) {
      pos = strip.numPixels() - 2;
      dir = -dir;
    }
  }
}

// Sine wave effect
#define PI 3.14159265
void wave(uint32_t c, int cycles, uint8_t wait) {
  float y;
  byte  r, g, b, r2, g2, b2;

  // Need to decompose color into its r, g, b elements
  g = (c >> 16) & 0x7f;
  r = (c >>  8) & 0x7f;
  b =  c        & 0x7f; 

  for(int x=0; x<(strip.numPixels()*5); x++)
  {
    for(int i=0; i<strip.numPixels(); i++) {
      y = sin(PI * (float)cycles * (float)(x + i) / (float)strip.numPixels());
      if(y >= 0.0) {
        // Peaks of sine wave are white
        y  = 1.0 - y; // Translate Y to 0.0 (top) to 1.0 (center)
        r2 = 127 - (byte)((float)(127 - r) * y);
        g2 = 127 - (byte)((float)(127 - g) * y);
        b2 = 127 - (byte)((float)(127 - b) * y);
      } else {
        // Troughs of sine wave are black
        y += 1.0; // Translate Y to 0.0 (bottom) to 1.0 (center)
        r2 = (byte)((float)r * y);
        g2 = (byte)((float)g * y);
        b2 = (byte)((float)b * y);
      }
      strip.setPixelColor(i, r2, g2, b2);
    }
    strip.show();
    delay(wait);
  }
}



/* Helper functions for the helpers o.O */

//Input a value 0 to 384 to get a color value.
//The colours are a transition r - g -b - back to r

uint32_t Wheel(uint16_t WheelPos)
{
  byte r, g, b;
  switch(WheelPos / 128)
  {
    case 0:
      r = 127 - WheelPos % 128;   //Red down
      g = WheelPos % 128;      // Green up
      b = 0;                  //blue off
      break; 
    case 1:
      g = 127 - WheelPos % 128;  //green down
      b = WheelPos % 128;      //blue up
      r = 0;                  //red off
      break; 
    case 2:
      b = 127 - WheelPos % 128;  //blue down 
      r = WheelPos % 128;      //red up
      g = 0;                  //green off
      break; 
  }
  return(strip.Color(r,g,b));
}
