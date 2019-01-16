// http://www.mauroalfieri.it/elettronica/rotary-encoder-attiny85-neopixel.html#more-10769

#include <Adafruit_NeoPixel.h>                 //Include the neopixel adafruit library for the control 
                                               //of the WS2812B leds;

#ifdef __AVR__                                 //Include the management library avr / power.h for 
#include <avr/power.h>                         //the management of AVR chips;
#endif

#define encA 3                                 //Define the pins for reading the encoder and 
#define encB 4                                 //controlling the LEDs;
#define swtc 1
#define PIN  2

#define NUMPIXELS      24                      //Define the number of leds you have available to 
                                               //end the parametric sketch;

volatile byte currentValue = 0;                //Define the variables that you will use to count 
volatile int  lastEncoded  = 0;                //the position of the encoder and the brightness of the led, 
volatile byte mybrightness = 128;              //set to 128 at the start;

int pos = abs(currentValue % 31);              //Benji Added Code
int lightR = 0; int lightG = 0; int lightB = 0;//Benji Added Code


Adafruit_NeoPixel pixels=Adafruit_NeoPixel(NUMPIXELS,PIN,NEO_GRB+NEO_KHZ800);
                                               //Create the pixel instance of the Adafruit_NeoPixel class;

void setup() {
    pinMode(encA, INPUT);                      //Define how to use the pins;
    pinMode(encB, INPUT);
    pinMode(swtc, INPUT);
    digitalWrite(encA, HIGH);                  //set to 1 (HIGH) the value on the pins to which 
    digitalWrite(encB, HIGH);                  //the encoder is connected;
    
    GIMSK = 0b00100000;                        //Enable pin change interrupts;
    PCMSK = 0b00011010;                        //Enable pin PB3 and PB4 and PB1;
    sei();                                     //Turn on interrupts;
 
    pixels.setBrightness(mybrightness);        //Sets the brightness of the LEDs to 128 ( mybrightness );
    pixels.begin();                            //Initialises the communication with the 24-LED 
                                               //neopixel ring;
}
  
void loop() {
  
  switch(pos){

    case 1: lightR = 255; lightG = 0; lightB = 0; break;       //All Red
    case 2: lightR = 255; lightG = 0; lightB = 51; break;
    case 3: lightR = 255; lightG = 0; lightB = 102; break;    
    case 4: lightR = 255; lightG = 0; lightB = 153; break;
    case 5: lightR = 255; lightG = 0; lightB = 204; break;
    case 6: lightR = 255; lightG = 0; lightB = 255; break;    
    case 7: lightR = 204; lightG = 0; lightB = 255; break;
    case 8: lightR = 153; lightG = 0; lightB = 255; break;    
    case 9: lightR = 102; lightG = 0; lightB = 255; break;
    case 10: lightR = 51; lightG = 0; lightB = 255; break;
    case 11: lightR = 0; lightG = 0; lightB = 255; break;      //All Blue    
    case 12: lightR = 0; lightG = 51; lightB = 255; break;
    case 13: lightR = 0; lightG = 102; lightB = 255; break;
    case 14: lightR = 0; lightG = 153; lightB = 255; break;    
    case 15: lightR = 0; lightG = 204; lightB = 255; break;
    case 16: lightR = 0; lightG = 255; lightB = 255; break;    
    case 17: lightR = 0; lightG = 255; lightB = 204; break;
    case 18: lightR = 0; lightG = 255; lightB = 153; break;
    case 19: lightR = 0; lightG = 255; lightB = 102; break;
    case 20: lightR = 0; lightG = 255; lightB = 51; break;    
    case 21: lightR = 0; lightG = 255; lightB = 0; break;      //All Green 
    case 22: lightR = 51; lightG = 255; lightB = 0; break;
    case 23: lightR = 102; lightG = 255; lightB = 0; break;
    case 24: lightR = 153; lightG = 255; lightB = 0; break;    
    case 25: lightR = 204; lightG = 255; lightB = 0; break;
    case 26: lightR = 255; lightG = 255; lightB = 0; break;    
    case 27: lightR = 255; lightG = 204; lightB = 0; break;
    case 28: lightR = 255; lightG = 153; lightB = 0; break;
    case 29: lightR = 255; lightG = 102; lightB = 0; break;
    case 30: lightR = 255; lightG = 51; lightB = 0; break;    
    case 31: lightR = 122; lightG = 215; lightB = 82; break;   //Kawasaki Candy Lime Green
    default: lightR = 0; lightG = 0; lightB = 0;
  }
for(int i=0;i<NUMPIXELS;i++) { 
       pixels.setPixelColor(i, lightR,lightG,lightB, mybrightness); 
    }
    pixels.show();

    if (digitalRead(swtc) == LOW ) {           //If the encoder button is pressed, its value 
       currentValue = setCurrBrightness();     //becomes LOW, and calls setCurrBrightness;
    }
    delay(20);
}
 
byte setCurrBrightness() {                     //Define the setCurrBrightness function  that 
                                               //returns, at the end of its activities, the value that the 
                                               //encoder has when it is called;
                                               
  byte oldValue = currentValue;                //Save in the oldValue variable  the value you will 
                                               //need at the end of  setCurrBrightness  to return it at the end;
                                               
  currentValue = mybrightness;                 //Set  currentValue as the current brightness, 
                                               //in order to apply the encoder rotation starting from the 
                                               //previously set value;
   
  while (digitalRead(swtc) == LOW) {           //Start a valid while loop until the switch is released;
    mybrightness = currentValue;               //With each cycle of the while it associates to  
                                               //mybrightness  the current value  currentValue;
    pixels.setBrightness(currentValue);        //Sets the brightness of the leds;
    pixels.show();                             //Apply the calculated values ​​to the LEDs;
  }
  return oldValue;                             //At the end of the while loop, after completing 
                                               //the brightness settings, return the value saved in oldValue 
                                               //to the calling function;
}
 
ISR(PCINT0_vect) {
  byte MSB=digitalRead(encA); //MSB=most significant bit
  byte LSB=digitalRead(encB); //LSB=least significant bit
   
  int encoded = (MSB << 1) |LSB;
  int sum  = (lastEncoded << 2) | encoded;
  
  if(sum==0b1101||sum==0b0100||sum==0b0010||sum==0b1011) currentValue++;
  if(sum==0b1110||sum==0b0111||sum==0b0001||sum==0b1000) currentValue--;
   
  lastEncoded = encoded; //store this value for next time
}
