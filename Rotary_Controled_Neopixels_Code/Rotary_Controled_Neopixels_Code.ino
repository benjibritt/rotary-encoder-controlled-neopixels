// https://learn.adafruit.com/adafruit-neopixel-uberguide/neopixel-shields?view=all#basic-connections
// https://www.youtube.com/watch?v=HQuLZHsGZdI
// https://www.youtube.com/watch?v=J9cDEef0IbQ&t=129s
// https://hifiduino.files.wordpress.com/2010/10/rotenc.jpg
#include <Adafruit_NeoPixel.h>  // Include the NeoPixel Library
#ifdef __AVR__
  #include <avr/power.h>
#endif
#define PIN 6
Adafruit_NeoPixel strip = Adafruit_NeoPixel(130, 6, NEO_GRB + NEO_KHZ800);
// Create a NeoPixel Strip (130 Pixels, Connected to Pin 6)
int lightR = 0; int lightG = 0; int lightB = 0; 
int flicker;

#define BUTTON_PIN   4    // Digital IO pin connected to the button.  This will be
                          // driven with a pull-up resistor so the switch should
                          // pull the pin to ground momentarily.  On a high -> low
                          // transition the button press logic will execute.

// the number of the pushbutton pin
const int buttonPin = 4;

// Used for generating interrupts using CLK signal
const int PinA = 2; 

// Used for reading DT signal
const int PinB = 3;  

// Keep track of last rotary value
int lastCount = 0;

// Updated by the ISR (Interrupt Service Routine)
volatile int virtualPosition = 365;

int pos = 0;


bool oldState = HIGH;
int showType = 0;

// ------------------------------------------------------------------
// INTERRUPT     INTERRUPT     INTERRUPT     INTERRUPT     INTERRUPT 
// ------------------------------------------------------------------
void isr ()  {
  static unsigned long lastInterruptTime = 0;
  unsigned long interruptTime = millis();

  // If interrupts come faster than 5ms, assume it's a bounce and ignore
  if (interruptTime - lastInterruptTime > 20) {
    if (digitalRead(PinB) == LOW)
    {
      virtualPosition-- ; // Could be -5 or -10
    }
    else {
      virtualPosition++ ; // Could be +5 or +10
    }

    // Keep track of when we were here last (no more than every 5ms)
    lastInterruptTime = interruptTime;
  }
}

// ------------------------------------------------------------------
// SETUP    SETUP    SETUP    SETUP    SETUP    SETUP    SETUP    
// ------------------------------------------------------------------
void setup() {  
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Just whilst we debug, view output on serial monitor
  Serial.begin(9600);

  // Rotary pulses are INPUTs
  pinMode(PinA, INPUT);
  pinMode(PinB, INPUT);

  // Attach the routine to service the interrupts
  attachInterrupt(digitalPinToInterrupt(PinA), isr, LOW);

  // Ready to go!
  Serial.println("Start");
  strip.begin();
    strip.setBrightness(120);
}


boolean checkPos(){
  // If the current rotary switch position has changed then update everything
  if (virtualPosition != lastCount) {
    
    // Write out to serial monitor the value and direction
    // Serial.println(virtualPosition);

    // Keep track of this new value
    lastCount = virtualPosition ;
  
    pos = abs(virtualPosition % 36);

    Serial.println(pos);
    return true;
  }
  return false;
}


// ------------------------------------------------------------------
// MAIN LOOP     MAIN LOOP     MAIN LOOP     MAIN LOOP     MAIN LOOP
// ------------------------------------------------------------------


void loop() {

  // Get current button state.
  bool newState = digitalRead(BUTTON_PIN);

  // Check if state changed from high to low (button press).
  if (newState == LOW && oldState == HIGH) {
    // Short delay to debounce button.
    delay(20);
    // Check if button is still low after debounce.
    newState = digitalRead(BUTTON_PIN);
    if (newState == LOW) {
      showType++;
      if (showType > 1)
        showType=0;
    }
  }

  // Set the last button state to the old state.
  oldState = newState;
  
 if (showType == 0) {
  for (int i = 0; i < strip.numPixels(); i++)
  {
    strip.setPixelColor(i, 0, 0, 0);
  }
  // Set the NeoPixel to a certain RGB Value
  strip.show();  // Show the NeoPixel Color
}
if (showType == 1) {
  
  checkPos();
  
  switch(pos){
    case 0: 
    flicker = random(100,200);
    lightR = 250-flicker; lightG = lightR-40-flicker; lightB = 40-flicker; 
    if(lightG<0) lightG=0; if(lightR<0) lightR=0; if(lightB<0) lightB=0;
    delay(random(100,150));
    break;
    case 1:   
    colorWipe(strip.Color(148, 0, 211), 50);
    colorWipe(strip.Color(75, 0, 130), 50);
    colorWipe(strip.Color(0, 0, 255), 50); 
    colorWipe(strip.Color(0, 255, 0), 50);
    colorWipe(strip.Color(255, 255, 0), 50);
    colorWipe(strip.Color(255, 127, 0), 50); 
    colorWipe(strip.Color(255, 0, 0), 50);
    break;
    case 2:
    theaterChase(strip.Color(255, 0, 0), 50);
    theaterChase(strip.Color(255, 255, 0), 50); 
    theaterChase(strip.Color(0, 255, 0), 50);
    theaterChase(strip.Color(0, 255, 255), 50);
    theaterChase(strip.Color(0, 0, 255), 50); 
    break;
    case 3: theaterChaseRainbow(50); break;
    case 4: rainbow(20); break;
    case 5: rainbowCycle(20); break;
    case 6: lightR = 255; lightG = 0; lightB = 0; break;
    case 7: lightR = 255; lightG = 0; lightB = 51; break;
    case 8: lightR = 255; lightG = 0; lightB = 102; break;    
    case 9: lightR = 255; lightG = 0; lightB = 153; break;
    case 10: lightR = 255; lightG = 0; lightB = 204; break;
    case 11: lightR = 255; lightG = 0; lightB = 255; break;    
    case 12: lightR = 204; lightG = 0; lightB = 255; break;
    case 13: lightR = 153; lightG = 0; lightB = 255; break;    
    case 14: lightR = 102; lightG = 0; lightB = 255; break;
    case 15: lightR = 51; lightG = 0; lightB = 255; break;
    case 16: lightR = 0; lightG = 0; lightB = 255; break;    
    case 17: lightR = 0; lightG = 51; lightB = 255; break;
    case 18: lightR = 0; lightG = 102; lightB = 255; break;
    case 19: lightR = 0; lightG = 153; lightB = 255; break;    
    case 20: lightR = 0; lightG = 204; lightB = 255; break;
    case 21: lightR = 0; lightG = 255; lightB = 255; break;    
    case 22: lightR = 0; lightG = 255; lightB = 204; break;
    case 23: lightR = 0; lightG = 255; lightB = 153; break;
    case 24: lightR = 0; lightG = 255; lightB = 102; break;
    case 25: lightR = 0; lightG = 255; lightB = 51; break;    
    case 26: lightR = 0; lightG = 255; lightB = 0; break;    
    case 27: lightR = 51; lightG = 255; lightB = 0; break;
    case 28: lightR = 102; lightG = 255; lightB = 0; break;
    case 29: lightR = 153; lightG = 255; lightB = 0; break;    
    case 30: lightR = 204; lightG = 255; lightB = 0; break;
    case 31: lightR = 255; lightG = 255; lightB = 0; break;    
    case 32: lightR = 255; lightG = 204; lightB = 0; break;
    case 33: lightR = 255; lightG = 153; lightB = 0; break;
    case 34: lightR = 255; lightG = 102; lightB = 0; break;
    case 35: lightR = 255; lightG = 51; lightB = 0; break;    
    case 36: lightR = 255; lightG = 0; lightB = 0; break;
    default: lightR = 0; lightG = 0; lightB = 0;
  }
  
  
  //strip.Color(lightR, lightG, lightB);
  for (int i = 0; i < strip.numPixels(); i++)
  {
    strip.setPixelColor(i, lightR, lightG, lightB);
  }
  // Set the NeoPixel to a certain RGB Value
  strip.show();  // Show the NeoPixel Color

} else {
  for (int i = 0; i < strip.numPixels(); i++)
  {
    strip.setPixelColor(i, 0, 0, 0);
  }
  // Set the NeoPixel to a certain RGB Value
  strip.show();  // Show the NeoPixel Color
}
}




// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    if(checkPos()){break;}
    if(showType == 0){break;}
    delay(wait);

  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
    if(checkPos()){break;}
    if(showType == 0){break;}

  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
    if(checkPos()){break;}    
    if(showType == 0){break;}
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  boolean quit = false;
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();
      if(checkPos()){quit = true; break;}
      delay(wait);
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
      if(checkPos()){break;}
    }        
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  boolean quit = false;
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();
      if(checkPos()){quit = true; break;}

      delay(wait);
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
      if(checkPos()){break;}
    }
      if(checkPos()){break;}
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
