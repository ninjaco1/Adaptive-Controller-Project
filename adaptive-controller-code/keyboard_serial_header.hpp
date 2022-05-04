#ifndef KEYBOARD_SERIAL_HEADER_HPP
#define KEYBOARD_SERIAL_HEADER_HPP
#include <Keyboard.h> // Arduino/micro keyboard library


class Key {
    private:
        uint8_t e1; // what was selected on left joystick
        uint8_t e2; // what was selected on right joystick
        char buffer[2]; // if shift or ctrl were pressed
        uint8_t buffer_count; // the items in the buffer
        
        // modes
        uint8_t shift;      // to see if shift is on or off
        uint8_t caps_lock;  // to see if caps lock is on or off
        uint8_t ctrl;       // to see if the ctrl is on or off
        uint8_t alt;        // to see if the alt is on or off
        
        // https://www.arduino.cc/reference/en/language/functions/usb/keyboard/keyboardmodifiers/
        // [joy1][joy2]
        char keymap[8][8] = {
            //                        tab
            {'k', 'l', 'm', 'n', 'o', KEY_TAB, KEY_LEFT_SHIFT, NULL},
            //                        alt
            {'p', 'q', 'r', 's', 't', KEY_LEFT_ALT, '[', ']'},
            {'u', 'v', 'w', 'x', 'y', ' ','\\', NULL},
            //                        delete
            {',', '.', '\'', '/', 'z', KEY_DELETE, NULL, NULL},
            //                        enter/return
            {'6', '7', '8', '9', '0', KEY_RETURN, '-', '='},
            //                        home
            {'1', '2', '3', '4', '5', KEY_HOME, NULL, NULL},
            //                        backspace
            {'a', 'b', 'c', 'd', 'e', KEY_BACKSPACE, NULL, NULL},
            //                        ctrl
            {'f', 'g', 'h', 'i', 'j', KEY_LEFT_CTRL, KEY_CAPS_LOCK, NULL}
        };

        // keymap with shift 
        char keymap_shift[8][8] = {
            //                        up_arrow       
            {'K', 'L', 'M', 'N', 'O', 0xda, NULL, NULL},
            //                        esc
            {'P', 'Q', 'R', 'S', 'T', 0xb1, '{', '}'},
            //                        right_arrow
            {'U', 'V', 'W', 'X', 'Y', 0xd7, NULL, NULL},
            //                        insert
            {'<', '>', '"', '?', 'Z', 0xd1, NULL, NULL},
            //                        down_arrow
            {'^', '&', '*', '(', ')', 0xd9, '_', '+'},
            //                        end
            {'!', '@', '#', '$', '%', 0xd5, NULL, NULL},
            //                        left_arrow
            {'A', 'B', 'C', 'D', 'E', 0xd8, NULL, NULL},
            //                        caps_lock
            {'F', 'G', 'H', 'I', 'J', KEY_CAPS_LOCK, NULL, NULL}
        };

        char keymap_caps[8][8] = {
            //                        tab
            {'K', 'L', 'M', 'N', 'O', KEY_TAB, KEY_LEFT_SHIFT, NULL},
            //                        alt
            {'P', 'Q', 'R', 'S', 'T', KEY_LEFT_ALT, '[', ']'},
            {'U', 'V', 'W', 'X', 'Y', ' ','\\', NULL},
            //                        delete
            {',', '.', '\'', '/', 'Z', KEY_DELETE, NULL, NULL},
            //                        enter/return
            {'6', '7', '8', '9', '0', KEY_RETURN, '-', '='},
            //                        home
            {'1', '2', '3', '4', '5', KEY_HOME, NULL, NULL},
            //                        backspace
            {'A', 'B', 'C', 'D', 'E', KEY_BACKSPACE, NULL, NULL},
            //                        ctrl
            {'F', 'G', 'H', 'I', 'J', KEY_LEFT_CTRL, KEY_CAPS_LOCK, NULL}
        };


    public:
        Key();
        void keyboard_serial(uint8_t enc1, uint8_t enc2);
        void decode_format(uint8_t enc1, uint8_t enc2);
        void serial_out();
        void toggle_shift();
        void toggle_caps_lock();
        void toggle_alt();
        void toggle_ctrl();


        // getters
        uint8_t get_shift();
        uint8_t get_caps();
        uint8_t get_ctrl();
        uint8_t get_e1();
        uint8_t get_e2();
        void reset_keys();
        


};

Key::Key(){
    Keyboard.begin();
    buffer_count = 0;
}

/*****************************************************
 *                  keyboard_serial 
 *  It takes the joystick data as the inputs, then  
 *  parses the data to see what combination of joystick
 *  was made. Then send that character to the screen
 * ***************************************************/
void Key::keyboard_serial(uint8_t enc1, uint8_t enc2){
    // if any of the joysticks are 0 that means that mouse
    // is being used
    if (enc1 == 0 || enc2 == 0)
        return;
    decode_format(enc1, enc2); // decode enc1 and enc2

    // these numbers shouldn't do anything 
    if (e1 == 8 || e1 == 9 || e2 == 8 || e2 == 9)
        return;


    serial_out();
}

/*****************************************************
 *                  decode_format 
 *  Takes the raw joystick data as input then decodes
 *  it.
 *  Examples:
 *  enc1 = 13, enc2 = 24 ----> e1 = 3, e2 = 4 
 * ***************************************************/
void Key::decode_format(uint8_t enc1, uint8_t enc2){
    // enc1 should start with a 1
    e1 = enc1 % 10;
    // enc2 should start with a 2
    e2 = enc2 % 10;
}

/*****************************************************
 *                  serial_out 
 *  Check which modes are on then determine what 
 *  character to send to the serial. 
 * 
 * ***************************************************/
void Key::serial_out(){
    char key_press;
    // check if the thing press was a ctrl
//    if (keymap[e1][e2] == KEY_LEFT_CTRL) {}


    // then everything else after
  
    //key_press = (shift == 0 && caps_lock == 0) ? keymap[e1][e2] : keymap_shift[e1][e2];

    if(shift)
        key_press = keymap_shift[e1][e2];
    else if (caps_lock)
        key_press = keymap_caps[e1][e2];
    else    
        key_press = keymap[e1][e2];
    
    switch (key_press){
        case KEY_LEFT_SHIFT:
        {
            toggle_shift();
        }
        case KEY_RIGHT_SHIFT:
        {
            toggle_shift();
        }
        case KEY_CAPS_LOCK:
        {
            toggle_caps_lock();
        }
        case KEY_LEFT_CTRL:
        {
            toggle_ctrl();
        }
        case KEY_RIGHT_CTRL:
        {
            toggle_ctrl();
        }
        case KEY_LEFT_ALT:
        {
            toggle_alt();
        }
        case KEY_RIGHT_ALT:
        {
            toggle_alt();
        }
        case NULL:
        {
            return; // means that are no letters here
        }

    }
    // checking if any of the keys are toggled
    if (ctrl == 1)
        Keyboard.press(KEY_LEFT_CTRL);

    if (shift == 1)
        Keyboard.press(KEY_LEFT_SHIFT);

    // if (caps_lock == 1) 
    //     Keyboard.press(KEY_CAPS_LOCK);

    if (alt == 1) 
        Keyboard.press(KEY_LEFT_ALT);
    
    Keyboard.press(key_press);
    Keyboard.releaseAll();
}


void Key::toggle_shift(){
    shift ^= 1;
}

void Key::toggle_caps_lock(){
    caps_lock ^= 1;
}

void Key::toggle_ctrl(){
    ctrl ^= 1;
}

void Key::toggle_alt(){
    alt ^= 1;
}

uint8_t Key::get_e1(){
    return e1;
}

uint8_t Key::get_e2(){
    return e2;
}

void Key::reset_keys(){
    shift = 0;
    ctrl = 0;
    alt = 0;
}

uint8_t Key::get_shift(){
    return shift;
}

uint8_t Key::get_caps(){
    return caps_lock;
}

#endif
