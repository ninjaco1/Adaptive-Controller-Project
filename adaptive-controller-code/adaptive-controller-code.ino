// FEATURE SWITCH
#define MOUSEBLOCK TRUE;
#define JOYSTICKBUTTONBLOCK TRUE;
#define KEYBOARDBLOCK TRUE;
#define OLEDBLOCK TRUE;

#ifdef OLEDBLOCK
// You can use any (4 or) 5 pins
#define SCLK_PIN 15
#define MOSI_PIN 16
#define DC_PIN 8
#define CS_PIN 10
#define RST_PIN 7

// one of these 3 includes conflicts with the joysticks
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1351.h>
#include <SPI.h>

// Color definitions
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF
#define PURPLE 0x7810
#define ORANGE 0xFC00

// Screen dimensions
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 128

Adafruit_SSD1351 tft = Adafruit_SSD1351(SCREEN_WIDTH, SCREEN_HEIGHT, CS_PIN, DC_PIN, MOSI_PIN, SCLK_PIN, RST_PIN);
bool setMouseSpeedFlag = false;
#endif

#ifdef JOYSTICKBUTTONBLOCK
#include "keyboard_serial_header.hpp"
#include <Wire.h>
#include "SparkFun_Qwiic_Joystick_Arduino_Library.h" //https://www.sparkfun.com/products/15168
//#include <TFT.h>
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

//******************************************************************************
//                            mode_selector
//
//******************************************************************************
void mode_selector(uint8_t selector)
{
    switch (selector)
    {
    case 0:
        visual_aid_display('K', 'O', ' ', 'M', ' ', ' ', 'L', 'N');
        break;
    case 1:
        visual_aid_display('P', 'T', '[', 'R', ']', ' ', 'Q', 'S');
        break;
    case 2:
        visual_aid_display('U', 'Y', '\\', 'W', ' ', ' ', 'V', 'X');
        break;
    case 3:
        visual_aid_display(',', 'Z', ' ', '\'', ' ', ' ', '.', '/');
        break;
    case 4:
        visual_aid_display('6', '0', '-', '8', '=', ' ', '7', '9');
        break;
    case 5:
        visual_aid_display('1', '5', ' ', '3', ' ', ' ', '2', '4');
        break;
    case 6:
        visual_aid_display('A', 'E', ' ', 'C', ' ', ' ', 'B', 'D');
        break;
    case 7:
        visual_aid_display('F', 'J', ' ', 'H', ' ', ' ', 'G', 'I');
        break;
    case 8:
        visual_aid_display('A', 'B', 'C', 'D', 'E', 'F', 'G', 'H');
        break;
    default:
        tft.fillScreen(BLACK);
    }
}

//******************************************************************************
//                            visual_aid_display
//
//******************************************************************************
void visual_aid_display(char one, char two, char three, char four, char five, char six, char seven, char eight)
{
    tft.fillScreen(BLACK);
    tft.fillCircle(tft.width() / 2, tft.height() / 2, tft.height() / 2 - 1, WHITE);
    tft.setTextSize(3);

    // Top
    tft.setCursor(tft.width() / 2 - 6, 5);
    tft.setTextColor(RED);
    tft.print(one);

    // Bottom
    tft.setCursor(tft.width() / 2 - 6, 105);
    tft.setTextColor(YELLOW);
    tft.print(two);

    // Left
    tft.setCursor(5, tft.height() / 2 - 10);
    tft.setTextColor(PURPLE);
    tft.print(three);

    // Right
    tft.setCursor(108, tft.height() / 2 - 10);
    tft.setTextColor(ORANGE);
    tft.print(four);

    // Top Left
    tft.setCursor(25, 20);
    tft.setTextColor(GREEN);
    tft.print(five);

    // Bottom Left
    tft.setCursor(25, 90);
    tft.setTextColor(BLUE);
    tft.print(six);

    // Top Right
    tft.setCursor(90, 20);
    tft.setTextColor(MAGENTA);
    tft.print(seven);

    // Bottom Right
    tft.setCursor(90, 90);
    tft.setTextColor(CYAN);
    tft.print(eight);
}

void setup()
{
    // put your setup code here, to run once:
    Serial.begin(9600);

#ifdef OLEDBLOCK
    tft.begin();
    tft.setRotation(2);
    tft.fillScreen(BLACK);
#endif

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

    //LED Diodes
    pinMode(18, OUTPUT);
    pinMode(19, OUTPUT);
    pinMode(20, OUTPUT);
    pinMode(21, OUTPUT);
    
    // flags for the menu
    // bool setMouseSpeedFlag = false;

    tft.setCursor(0, 0);
    tft.setTextColor(WHITE);
    tft.setTextSize(1);
    tft.print("Please Select Mouse  Sensitivity:");

    tft.setCursor(0, 20);
    tft.setTextColor(WHITE);
    tft.print("RED: Low");

    tft.setCursor(0, 30);
    tft.setTextColor(WHITE);
    tft.print("LOW: Normal");

    tft.setCursor(0, 40);
    tft.setTextColor(WHITE);
    tft.print("Blue: Fast");
    // reading dummny
    chk_buttons(4);
    chk_buttons(5);
    chk_buttons(6);
    chk_buttons(9);
        chk_buttons(4);
    chk_buttons(5);
    chk_buttons(6);
    chk_buttons(9);
        chk_buttons(4);
    chk_buttons(5);
    chk_buttons(6);
    chk_buttons(9);
        chk_buttons(4);
    chk_buttons(5);
    chk_buttons(6);
    chk_buttons(9);
    _delay_ms(1000);

    while (1)
    {
        // leave the menu when done

        // let buttons choose sentivity
        if (chk_buttons(4))
        {
            mouse.set_sensitivity(0.5);
            break;
        }

        if (chk_buttons(5))
        {
            mouse.set_sensitivity(1);
            break;
        }
        if (chk_buttons(6))
        {
            mouse.set_sensitivity(2);
            break;
        }
    }

    tft.fillScreen(BLACK);
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
        //tft.fillScreen(RED);
        digitalWrite(18,CAPS);
    }
    else if (chk_buttons(5))
    {
        Shift = 1;
        k.toggle_shift();
        //tft.fillScreen(YELLOW);
        digitalWrite(19,Shift);
    }
    else if (chk_buttons(6))
    {
        Ctrl = 1;
        k.toggle_ctrl();
        //tft.fillScreen(BLUE);
        digitalWrite(20,Ctrl);
    }
    else if (chk_buttons(9))
    {
        Alt = 1;
        k.toggle_alt();
        //tft.fillScreen(GREEN);
        digitalWrite(21,Alt);
    }
#endif

    // Print button, joystick, and keyboard output maximum of 100ms (10Hz). After, reset button status (besides CAPS)
    if (millis() >= (timepass + 100))
    {
        static uint8_t leftJoystickCounter = 0;

#ifdef JOYSTICKBUTTONBLOCK &&MOUSEBLOCK
        if (Lkey == 0)
        {

            //******************** Mouse Serial Code *****************
            if (Rclick)
            {
                mouse.click(RIGHT);
            }
            if (Lclick)
            {
                mouse.click(LEFT);
            }
            mouse.move(RinX, RinY, 0);
            Rclick = 0;
            //******************** Mouse Serial End ******************

            leftJoystickCounter = 0;
        }
#endif

#ifdef JOYSTICKBUTTONBLOCK &&KEYBOARDBLOCK
        if (Lkey != 0)
        {
            static uint8_t prevRkeyState = 0;
            static bool resetJoystickState = true;

            timepass = millis(); // set timer to next increment
            leftJoystickCounter++;

            if (leftJoystickCounter == 10)
            {
                mode_selector(Lkey % 10);
            }

            if (prevRkeyState != Rkey && resetJoystickState == true)
            {
                k.keyboard_serial(Lkey, Rkey);
                Shift = 0;
                Ctrl = 0;
                Alt = 0;
                digitalWrite(19,Shift);
                digitalWrite(20,Ctrl);
                digitalWrite(21,Alt);
            }
  
            if (Rkey == 0)
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
