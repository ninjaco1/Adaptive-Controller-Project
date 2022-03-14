//FEATURE SWITCH
#define MOUSEBLOCK              TRUE;
#define JOYSTICKBUTTONBLOCK     TRUE;
#define KEYBOARDBLOCK           TRUE;
#define OLEDBLOCK               FALSE;

#ifdef JOYSTICKBUTTONBLOCK
#include "keyboard_serial_header.hpp"
#include <Wire.h>
#include "SparkFun_Qwiic_Joystick_Arduino_Library.h" //https://www.sparkfun.com/products/15168
#include <TFT.h>
#define OFF 0
#define ON 1
#define RIGHT 2
#define LEFT 1
#endif

#ifdef MOUSEBLOCK
#include "Assistive_Mouse.h"
#endif

// declare joystick objects
#ifdef JOYSTICKBUTTONBLOCK
JOYSTICK Lstick;
JOYSTICK Rstick;
#endif

// declare keyboard object
#ifdef KEYBOARDBLOCK
Key k;
#endif

// declare mouse 
#ifdef MOUSEBLOCK
Assistive_Mouse mouse;
#endif

#ifdef JOYSTICKBUTTONBLOCK
uint8_t Laddress = 0x20; // I2C address for left joystick (default)
uint8_t Raddress = 0x21; // I2C address for right joystick (reprogrammed)

// variables to hold joystick input
uint16_t LinX = 512; // ADC "coordinate" value from the ATtiny85
uint16_t LinY = 512; // ADC "coordinate" value from the ATtiny85
uint16_t RinX = 512; // ADC "coordinate" value from the ATtiny85
uint16_t RinY = 512; // ADC "coordinate" value from the ATtiny85
uint8_t Lkey = 00;   // input to convey to keyboard
uint8_t Rkey = 00;   // input to convey to keyboard

unsigned long timepass = 0; // variable to track time, start at 0

// button status variables, default OFF
uint8_t Lclick = OFF;
uint8_t Rclick = OFF;
uint8_t CAPS = OFF;  // CAPS state will be toggled...
uint8_t Shift = OFF; //... while other buttons work directly as switches
uint8_t Ctrl = OFF;
uint8_t Alt = OFF;
#endif

//******************************************************************************
//                            chk_buttons
// Checks the state of the button number passed to it. It shifts in ones till
// the button is pushed. Function returns a 1 only once per debounced button
// push so a debounce and toggle function can be implemented at the same time.
// Adapted to check all buttons from Ganssel's "Guide to Debouncing"
// Expects active low pushbuttons on PINA port.  Debounce time is determined by
// external loop delay times 12. Looks for rising edge on button so held actions
// have no unexpected effect.
//******************************************************************************
uint8_t chk_buttons(uint8_t pin)
{
    static uint16_t state = 0;                          // holds state of debounce reading
    state = (state << 1) | (digitalRead(pin)) | 0xE000; // mask on PINA button
    if (state == 0xEFFF)                                // compares to rising edge value (so holding doesn't stack)
        return 1;

    // if not true for 12 times, return false
    return 0;
}

//******************************************************************************
//                            chk_click
// Checks the state of the click passed to it. It shifts in ones till
// the button is pushed. Function returns a 1 only once per debounced button
// push so a debounce and toggle function can be implemented at the same time.
// Adapted to check all buttons from Ganssel's "Guide to Debouncing"
// Expects active low pushbuttons on PINA port.  Debounce time is determined by
// external loop delay times 12. Looks for rising edge on button so held actions
// have no unexpected effect.
//******************************************************************************
uint8_t chk_click(uint8_t which_click)
{
    static uint8_t state1 = 0; // holds state of right debounce reading
    static uint8_t state2 = 0; // holds state of left debounce
    if (which_click == RIGHT)
    {                                                          // right click
        state1 = (state1 << 1) | (!Rstick.getButton()) | 0xE0; // mask
        if (state1 == 0xEF)                                    // compares to rising edge value (so holding doesn't stack)
            return 1;

        // if not true for 4 times, return false
        return 0;
    }

    if (which_click == LEFT)
    {                                                          // left click
        state2 = (state2 << 1) | (!Lstick.getButton()) | 0xE0; // mask
        if (state2 == 0xEF)                                    // compares to rising edge value (so holding doesn't stack)
            return 1;

        // if not true for 4 times, return false
        return 0;
    }
}

void setup()
{
    // put your setup code here, to run once:
    Serial.begin(9600);

    #ifdef JOYSTICKBUTTONBLOCK
    if (Lstick.begin(Wire, Laddress) == false)
    {
        Serial.println("Left Joystick does not appear to be connected. Please check wiring. Freezing...");
        while (1)
            ;
    }

    if (Rstick.begin(Wire, Raddress) == false)
    {
        Serial.println("Right Joystick does not appear to be connected. Please check wiring. Freezing...");
        while (1)
            ;
    }

    mouse.start();

    // pinMode for buttons
    pinMode(4, INPUT); // change values later
    pinMode(5, INPUT); // change values later
    pinMode(6, INPUT); // change values later
    pinMode(9, INPUT); // change values later
    #endif
}

void loop()
{
    #ifdef JOYSTICKBUTTONBLOCK
    // gather coord data from joystick (0-1023)
    LinX = Lstick.getHorizontal();
    LinY = Lstick.getVertical();
    Lclick = chk_click(LEFT); // send 1 for left click, 2 for right (reflect mouse click)

    RinX = Rstick.getHorizontal();
    RinY = Rstick.getVertical();
    Rclick = chk_click(RIGHT); // send RIGHT for right click, LEFT for left click

    // Parse joystick data for keyboard

    // LEFT STICK: if X is to the left
    if (LinX < 470)
    { // about 60 (~20%) away from center to constitute change
        // three cases for Y
        if (LinY < 470)
            Lkey = 15;
        else if (LinY > 575)
            Lkey = 17;
        else
            Lkey = 16;
    }
    // if X is to the right
    else if (LinX > 575)
    {
        // three cases for Y
        if (LinY < 470)
            Lkey = 13;
        else if (LinY > 575)
            Lkey = 11;
        else
            Lkey = 12;
    }
    // if X is in the middle
    else
    {
        // three cases for Y
        if (LinY < 470)
            Lkey = 14;
        else if (LinY > 575)
            Lkey = 10;
        else
            Lkey = 00;
    }

    // RIGHT STICK: if X is to the left
    if (RinX < 470)
    { // about 60 (~20%) away from center to constitute change
        // three cases for Y
        if (RinY < 470)
            Rkey = 25;
        else if (RinY > 575)
            Rkey = 27;
        else
            Rkey = 26;
    }

    // if X is to the right
    else if (RinX > 575)
    {
        // three cases for Y
        if (RinY < 470)
            Rkey = 23;
        else if (RinY > 575)
            Rkey = 21;
        else
            Rkey = 22;
    }

    // if X is in the middle
    else
    {
        // three cases for Y
        if (RinY < 470)
            Rkey = 24;
        else if (RinY > 575)
            Rkey = 20;
        else
            Rkey = 00;
    }

    // Check pushbuttons, account for debouncing
    if (chk_buttons(4))
    {
        CAPS = CAPS ^ 1;
        k.toggle_caps_lock();
    }
    else if (chk_buttons(5))
    {
        Shift = 1;
        k.toggle_shift();
    }
    else if (chk_buttons(6))
    {
        Ctrl = 1;
        k.toggle_ctrl();
    }
    else if (chk_buttons(9))
    {
        Alt = 1;
        k.toggle_alt();
    }
    #endif
    
    // Print button, joystick, and keyboard output maximum of 100ms (10Hz). After, reset button status (besides CAPS)
    if (millis() >= (timepass + 100))
    {
        #ifdef JOYSTICKBUTTONBLOCK && MOUSEBLOCK
        if (Lkey == 0){

            //******************** Mouse Serial Code *****************
            if(Rclick)
                mouse.click(RIGHT);
            if(Lclick)
                mouse.click(LEFT);
            mouse.move(RinX, RinY, 0);
            Rclick = 0;
            //******************** Mouse Serial End ******************

        }
        #endif

        #ifdef JOYSTICKBUTTONBLOCK && KEYBOARDBLOCK
        if (Lkey != 0){

            timepass = millis(); // set timer to next increment

            static uint8_t prevRkeyState = 0;
            static bool resetJoystickState = true;

            if(prevRkeyState != Rkey && resetJoystickState == true)
                k.keyboard_serial(Lkey,Rkey);

            if(Rkey == 0)
                resetJoystickState = true;
            else
                resetJoystickState = false;
            prevRkeyState = Rkey;

            // Reset all non-CAPS buttons
            Lclick = 0;
            Rclick = 0;
            Shift = 0;
            Ctrl = 0;
            Alt = 0;


        }
        #endif
    }

}
