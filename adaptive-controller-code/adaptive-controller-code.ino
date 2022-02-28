// all the header files
#include "./keyboard_serial_header.hpp"
#include "Assitive_Mouse.hpp"

// including libraries to handle serial comms and work with Sparkfun joystick module
#include <Wire.h>
#include "SparkFun_Qwiic_Joystick_Arduino_Library.h" //https://www.sparkfun.com/products/15168
#include <TFT.h>

//OLED
// Screen dimensions
#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 128 // Change this to 96 for 1.27" OLED.

// You can use any (4 or) 5 pins 
#define SCLK_PIN 15
#define MOSI_PIN 16
#define DC_PIN   8
#define CS_PIN   10
#define RST_PIN  7

// Color definitions
#define BLACK           0x0000
#define BLUE            0x001F
#define RED             0xF800
#define GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0  
#define WHITE           0xFFFF
#define PURPLE          0x7810
#define ORANGE          0xFC00


// OLED
// Screen dimensions
#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 128 // Change this to 96 for 1.27" OLED.

// You can use any (4 or) 5 pins 
#define SCLK_PIN 15
#define MOSI_PIN 16
#define DC_PIN   8
#define CS_PIN   10
#define RST_PIN  7

// Color definitions
#define BLACK           0x0000
#define BLUE            0x001F
#define RED             0xF800
#define GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0  
#define WHITE           0xFFFF
#define PURPLE          0x7810
#define ORANGE          0xFC00
Adafruit_SSD1351 tft = Adafruit_SSD1351(SCREEN_WIDTH, SCREEN_HEIGHT, CS_PIN, DC_PIN, MOSI_PIN, SCLK_PIN, RST_PIN);  

float p = 3.1415926;

// mouse serial

// If HID in use
#if defined(_USING_HID)

// HID Report Descriptor - VERY IMPORTANT - TAKEN FROM MOUSE.CPP
static const uint8_t _hidReportDescriptor[] PROGMEM = {

    //  Mouse
      0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)  // 54
      0x09, 0x02,                    // USAGE (Mouse)
      0xa1, 0x01,                    // COLLECTION (Application)
      0x09, 0x01,                    //   USAGE (Pointer)
      0xa1, 0x00,                    //   COLLECTION (Physical)
      0x85, 0x01,                    //     REPORT_ID (1)
      0x05, 0x09,                    //     USAGE_PAGE (Button)
      0x19, 0x01,                    //     USAGE_MINIMUM (Button 1)
      0x29, 0x03,                    //     USAGE_MAXIMUM (Button 3)
      0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
      0x25, 0x01,                    //     LOGICAL_MAXIMUM (1)
      0x95, 0x03,                    //     REPORT_COUNT (3)
      0x75, 0x01,                    //     REPORT_SIZE (1)
      0x81, 0x02,                    //     INPUT (Data,Var,Abs)
      0x95, 0x01,                    //     REPORT_COUNT (1)
      0x75, 0x05,                    //     REPORT_SIZE (5)
      0x81, 0x03,                    //     INPUT (Cnst,Var,Abs)
      0x05, 0x01,                    //     USAGE_PAGE (Generic Desktop)
      0x09, 0x30,                    //     USAGE (X)
      0x09, 0x31,                    //     USAGE (Y)
      0x09, 0x38,                    //     USAGE (Wheel)
      0x15, 0x81,                    //     LOGICAL_MINIMUM (-127)
      0x25, 0x7f,                    //     LOGICAL_MAXIMUM (127)
      0x75, 0x08,                    //     REPORT_SIZE (8)
      0x95, 0x03,                    //     REPORT_COUNT (3)
      0x81, 0x06,                    //     INPUT (Data,Var,Rel)
      0xc0,                          //   END_COLLECTION
      0xc0,                          // END_COLLECTION
};


// TESTING
#define OLED
#define KEYBOARDSERIAL
#define MOUSESERIAL
#define BUTTONJOYSTICK

// defines for joysticks or buttons
#define OFF 0
#define ON 1

// declare joystick objects
JOYSTICK Lstick;
JOYSTICK Rstick;

// declare keyboard object
Key k;

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

// set up the pins

// variables that will change

// functions

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

// OLED function
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
    Serial.begin(9600); // start comms at 9600 baud

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

    // pinMode for buttons
    pinMode(4, INPUT); // change values later
    pinMode(5, INPUT); // change values later
    pinMode(6, INPUT); // change values later
    pinMode(9, INPUT); // change values later

    #ifdef OLED
    // OLED
    tft.begin();
    tft.setRotation(1);

    uint8_t mode_selector = 8; // joystick encoder value

    switch (mode_selector)
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

    #endif
}

void loop()
{
    // put your main code here, to run repeatly:

    //*********************** joystick code******************************
    // gather coord data from joystick (0-1023)
    // left
    LinX = Lstick.getHorizontal();
    LinY = Lstick.getVertical();
    Lclick = !(Lstick.getButton());

    // right
    RinX = Rstick.getHorizontal();
    RinY = Rstick.getVertical();
    Rclick = !(Rstick.getButton());

    // Parse joystick data for keyboard

    // LEFT STICK: if X is to the left
    if (LinX < 470)
    { // about 60 (~20%) away from center to constitute change
        // three cases for Y
        if (LinY < 470)
        {
            Lkey = 15;
        }
        else if (LinY > 575)
        {
            Lkey = 17;
        }
        else
        {
            Lkey = 16;
        }
    }

    // if X is to the right
    else if (LinX > 575)
    {
        // three cases for Y
        if (LinY < 470)
        {
            Lkey = 13;
        }
        else if (LinY > 575)
        {
            Lkey = 11;
        }
        else
        {
            Lkey = 12;
        }
    }

    // if X is in the middle
    else
    {
        // three cases for Y
        if (LinY < 470)
        {
            Lkey = 14;
        }
        else if (LinY > 575)
        {
            Lkey = 10;
        }
        else
        {
            Lkey = 00;
        }
    }

    // RIGHT STICK: if X is to the left
    if (RinX < 470)
    { // about 60 (~20%) away from center to constitute change
        // three cases for Y
        if (RinY < 470)
        {
            Rkey = 25;
        }
        else if (RinY > 575)
        {
            Rkey = 27;
        }
        else
        {
            Rkey = 26;
        }
    }
    // if X is to the right
    else if (RinX > 575)
    {
        // three cases for Y
        if (RinY < 470)
        {
            Rkey = 23;
        }
        else if (RinY > 575)
        {
            Rkey = 21;
        }
        else
        {
            Rkey = 22;
        }
    }
    // if X is in the middle
    else
    {
        // three cases for Y
        if (RinY < 470)
        {
            Rkey = 24;
        }
        else if (RinY > 575)
        {
            Rkey = 20;
        }
        else
        {
            Rkey = 00;
        }
    }

    // Check pushbuttons, account for debouncing
    // button 1-4 in order
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

    // Print button, joystick, and keyboard output maximum of 100ms (10Hz). After, reset button status (besides CAPS)
    if (millis() >= (timepass + 100))
    {
        timepass = millis(); // set timer to next increment
        // Reset all non CAPS buttons
        Lclick = 0;
        Rclick = 0;
        Shift = 0;
        Ctrl = 0;
        Alt = 0;
    }
    //********************joystick end*************************

    //****************** Mouse Serial **************************

    //****************** Mouse End *****************************

    //****************** OLED Display **************************
    mode_selector = Lkey % 10; // joystick encoder value
    //****************** OLED End ******************************

    //****************** Keyboard Serial ***********************
    k.keyboard_serial(Lkey, Rkey);

    //****************** Keyboard End **************************
}