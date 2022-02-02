/*
 * AssistiveMouse.cpp
 *
 * Author: Jordyn Marshall
 * Date: 1/31/22
 *
 * Based heavily off of the Mouse Arduino library,
 * Copyright (c) 2015, Arduino LCC, under terms of
 * GNU Lesser General Public License
 *
 * This library is to be used with the assistive control
 * for the Adaptive Controllers Senior Design project team.
 * This library would allow for the Arduino to take control
 * of the mouse on the computer it is connected to through
 * USB, using HID protocols.
 *
 */

// Include header file
#include "Assistive_Mouse.h"

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

//================================================================================
//================================================================================
//	Assistive Mouse

// Class Constructor
Assistive_Mouse::Assistive_Mouse(void)
{
    // Sets up the report descriptor so that the Arduino can behave as a mouse.
    static HIDSubDescriptor node(_hidReportDescriptor, sizeof(_hidReportDescriptor));
    HID().AppendDescriptor(&node);

    // Sets all class variables to their defaults
    mouse_use = false;
    buttons = 0;
    s = 1;
}

// Function to get the currently set sensitivity value of the mouse.
float Assistive_Mouse::get_sensitivity(void) {
    return s;
}

// Function to set the sensivitity value of the mouse.
void Assistive_Mouse::set_sensitivity(float new_s) {
    s = new_s;
}

// Starts the mouse by setting the proper sensitivity and letting the class know
// that mouse functions are now valid.
void Assistive_Mouse::start(float new_s) {
    s = new_s;
    mouse_use = true;
}

// Stops the mouse by setting all values to their defaults and sending a report
// to the computer indicating no movement
void Assistive_Mouse::stop(void) {
    buttons = 0;
    s = 1;
    send_data(0, 0, 0);
    mouse_use = false;
}

// Presses the specified button
void Assistive_Mouse::press(uint8_t b) {
    // If mouse in use
    if (mouse_use) {
        // OR the buttons
        buttons = buttons | b;
        // Send button data only
        send_data(0, 0, 0);
    }
}

// Releases the specified button
void Assistive_Mouse::release(uint8_t b) {
    // If mouse in use AND button pressed
    if (mouse_use && (b & buttons)) {
        // XOR the buttons
        buttons = buttons ^ b;
        // Send button data only
        send_data(0, 0, 0);
    }
}

// Mouse click function, which calls both press and release
void Assistive_Mouse::click(uint8_t b) {
    press(b);
    release(b);
}

void Assistive_Mouse::move(int x, int y, int wheel) {
    // If mouse can be used
    if (mouse_use) {
        // Calculate proper x and y values from given joystick values and sensitivity
        int new_x = ((x - 512) / 4) * s;
        int new_y = ((y - 512) / 4) * s;
        int new_w = wheel * s;
        
        // Ensures no overflow for the x value (signed 8-bit number)
        if (new_x > 127) {
            new_x = 127;
        }
        else if (new_x < -128) {
            new_x = -128;
        }

        // Ensures no overflow for the y value (signed 8-bit number)
        if (new_y > 127) {
            new_y = 127;
        }
        else if (new_y < -128) {
            new_y = -128;
        }

        // Ensures no overflow for the wheel value (signed 8-bit number)
        if (new_w > 127) {
            new_w = 127;
        }
        else if (new_w < -128) {
            new_w = -128;
        }

        // Sends this data to the computer
        send_data(new_x, new_y, new_w);
    }
}

// Private function to send data to the computer
void Assistive_Mouse::send_data(signed char x, signed char y, signed char wheel) {
    // Create array of 4 8-bit numbers 
    uint8_t m[4];
    // Index 0 - button data, as a binary number indicating what buttons are pressed
    m[0] = buttons;
    // Index 1 - the new relative x position of the cursor
    m[1] = x;
    // Index 2 - the new relative y position of the cursor
    m[2] = y;
    // Index 3 - the new relative wheel value
    m[3] = 0;
    // Sends this report to the computer
    HID().SendReport(1, m, 4);
}

#endif
