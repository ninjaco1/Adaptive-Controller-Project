#include "Assistive_Mouse.h"
#include <Wire.h>
#include "SparkFun_Qwiic_Joystick_Arduino_Library.h" //https://www.sparkfun.com/products/15168
#include <TFT.h>

#define OFF 0
#define ON 1

Assistive_Mouse mouse;

//declare joystick objects
JOYSTICK Lstick;
JOYSTICK Rstick;

uint8_t Laddress = 0x20;  //I2C address for left joystick (default)
uint8_t Raddress = 0x21;  //I2C address for right joystick (reprogrammed)

//variables to hold joystick input
uint16_t LinX = 512;       //ADC "coordinate" value from the ATtiny85
uint16_t LinY = 512;       //ADC "coordinate" value from the ATtiny85
uint16_t RinX = 512;       //ADC "coordinate" value from the ATtiny85
uint16_t RinY = 512;       //ADC "coordinate" value from the ATtiny85
uint8_t Lkey = 00;        //input to convey to keyboard
uint8_t Rkey = 00;        //input to convey to keyboard

unsigned long timepass = 0;   //variable to track time, start at 0

//button status variables, default OFF
uint8_t Lclick = OFF;
uint8_t Rclick = OFF;
uint8_t CAPS = OFF;       //CAPS state will be toggled...
uint8_t Shift = OFF;      //... while other buttons work directly as switches
uint8_t Ctrl = OFF;
uint8_t Alt = OFF;

//******************************************************************************
//                            chk_buttons                                      
//Checks the state of the button number passed to it. It shifts in ones till   
//the button is pushed. Function returns a 1 only once per debounced button    
//push so a debounce and toggle function can be implemented at the same time.  
//Adapted to check all buttons from Ganssel's "Guide to Debouncing"            
//Expects active low pushbuttons on PINA port.  Debounce time is determined by 
//external loop delay times 12. Looks for rising edge on button so held actions
//have no unexpected effect.
//******************************************************************************
uint8_t chk_buttons(uint8_t pin) {
        static uint16_t state = 0;      //holds state of debounce reading
        state = (state << 1) | (digitalRead(pin)) | 0xE000; //mask on PINA button
        if (state == 0xEFFF)    //compares to rising edge value (so holding doesn't stack)
                return 1;

        //if not true for 12 times, return false        
        return 0;
}

//******************************************************************************
//                            chk_click
//Checks the state of the click passed to it. It shifts in ones till
//the button is pushed. Function returns a 1 only once per debounced button
//push so a debounce and toggle function can be implemented at the same time.
//Adapted to check all buttons from Ganssel's "Guide to Debouncing"
//Expects active low pushbuttons on PINA port.  Debounce time is determined by
//external loop delay times 12. Looks for rising edge on button so held actions
//have no unexpected effect.
//******************************************************************************
uint8_t chk_click(uint8_t which_click) {
  static uint8_t state1 = 0;      //holds state of right debounce reading
  static uint8_t state2 = 0;      //holds state of left debounce
  if (which_click == 1) {  //right click
    state1 = (state1 << 1) | (!Rstick.getButton()) | 0xE0; //mask
    if (state1 == 0xEF)    //compares to rising edge value (so holding doesn't stack)
      return 1;

    //if not true for 4 times, return false
    return 0;
  }

  if (which_click == 2) {  //left click
    state2 = (state2 << 1) | (!Lstick.getButton()) | 0xE0; //mask
    if (state2 == 0xEF)    //compares to rising edge value (so holding doesn't stack)
      return 1;

    //if not true for 4 times, return false
    return 0;
  }
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  mouse.start();

  if(Rstick.begin(Wire, Raddress) == false){
    Serial.println("Right Joystick does not appear to be connected. Please check wiring. Freezing...");
    while(1);
  }
}

void loop() {
  // Poll Joystick
  RinX = Rstick.getHorizontal();
  RinY = Rstick.getVertical();
  Rclick = chk_click(1);

  // Mouse code
  if (Rclick) {
    mouse.click(1);
  }
  mouse.move(RinX, RinY, 0);

  // Reset Values
  Rclick = 0;
}
