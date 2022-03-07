/*
 * AssistiveMouse.h
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

// Define this library only once
#ifndef ASSISTIVE_MOUSE_h
#define ASSISTIVE_MOUSE_h

// Include HID library
#include "HID.h"

// Warning if HID unable to be used.
#if !defined(_USING_HID)

#warning "Using legacy HID core (non pluggable)"

#else

 //================================================================================
 //================================================================================
 //  Assistive Mouse

// Defines macros to make mouse clicks easier to use
#define MOUSE_LEFT 1
#define MOUSE_RIGHT 2
#define MOUSE_MIDDLE 4
#define MOUSE_ALL (MOUSE_LEFT | MOUSE_RIGHT | MOUSE_MIDDLE)

// Class to keep track of the usage of the mouse
class Assistive_Mouse {
private:
    bool mouse_use;     // Indicates whether mouse usage is valid
    uint8_t buttons;    // Indicates current state of the buttons
    float s;              // Indicates the current sensivity value

public:
    /*
    *   Function: Assistive_Mouse()
    *   Parameters: None
    *   Return Value: None
    *
    *   The constructor for this class, which sets up the HID Report Descriptor and
    *   sets up all default values.
    */
    Assistive_Mouse(void);

    /*
    *   Function: get_sensitivity()
    *   Parameters: None
    *   Return Value: The integer of the current sensitivity value
    *
    *   Returns the currently set sensitivity value for this mouse
    */
    float get_sensitivity(void);

    /*
    *   Function: set_sensitivity()
    *   Parameters: The new sensitivity value
    *   Return Value: None
    *
    *   Changes the currently set sensitivity value to its new value
    */
    void set_sensitivity(float);

    /*
    *   Function: start()
    *   Parameters: The new sensitivity value. Defaults to 1
    *   Return Value: None
    *
    *   Indicates that the mouse is in use with the given sensivitity value
    */
    void start(float new_s = 1);

    /*
    *   Function: stop()
    *   Parameters: None
    *   Return Value: None
    *
    *   Resets all mouse values and indicates that the mouse is no longer in use
    */
    void stop(void);

    /*
    *   Function: press()
    *   Parameters: The button to be pressed. Defaults to the left mouse button.
    *   Return Value: None
    *
    *   Indicates that the given button is now pressed and sends that data to the computer.
    */
    void press(uint8_t b = MOUSE_LEFT);

    /*
    *   Function: release()
    *   Parameters: The button to be released. Defaults to the left mouse button.
    *   Return Value:
    *
    *   Releases the given button, if it is pressed, and sends the data to the computer.
    */
    void release(uint8_t b = MOUSE_LEFT);

    /*
    *   Function: click()
    *   Parameters: The button to be clicked. Defaults to the left mouse button.
    *   Return Value: None
    *
    *   Clicks the given button by calling both the press and release functions
    */
    void click(uint8_t b = MOUSE_LEFT);

    /*
    *   Function: move()
    *   Parameters: The x and y values from the joystick, as well as any values for the mouse wheel
    *   Return Value: None
    *
    *   The public move function for this mouse. Takes the joystick values and translates them so that
    *   they can be properly sent to the computer.
    */
    void move(int x, int y, int wheel = 0);

private:
    /*
    *   Function: send_data()
    *   Parameters: The relative new x, y, and wheel positions of the cursor
    *   Return Value: None
    *
    *   Sends a report to the computer indicating how the mouse should be moved. 
    */
    void send_data(signed char x, signed char y, signed char wheel = 0);
};

#endif
#endif