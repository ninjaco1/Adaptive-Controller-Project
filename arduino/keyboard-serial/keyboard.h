#include <Keyboard.h> // Arduino keyboard library

class Key {
    private:
        uint8_t e1; // what was selected on left joystick
        uint8_t e2; // what was selected on right joystick
        
        // modes
        uint8_t shift; // to see if shift is on or off
        uint8_t caps_lock; // to see if caps lock is on or off

        // [joy1][joy2]
        char keymap[8][8] = {
            //                        tab
            {'k', 'l', 'm', 'n', 'o', 0xb3, NULL, NULL},
            //                        alt
            {'p', 'q', 'r', 's', 't', 0x82, '[', ']'},
            {'u', 'v', 'w', 'x', 'y', ' ','\\', NULL},
            //                        delete
            {',', '.', '\'', '/', 'z', 0xd4, NULL, NULL},
            //                        enter/return
            {'6', '7', '8', '9', '0', 0xb0, '-', '='},
            //                        home
            {'1', '2', '3', '4', '5', 0xd2, NULL, NULL},
            //                        backspace
            {'a', 'b', 'c', 'd', 'e', 0xb2, NULL, NULL},
            //                        ctrl
            {'f', 'g', 'h', 'i', 'j', 0x84, NULL, NULL}
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
            {'F', 'G', 'H', 'I', 'J', 0xc1, NULL, NULL}
        };


    public:
        void keyboard_serial(uint8_t enc1, uint8_t enc2);
        void decode_format(uint8_t enc1, uint8_t enc2);
        void serial_out();


};


void Key::keyboard_serial(uint8_t enc1, uint8_t enc2){
    decode_format(enc1, enc2); // decode enc1 and enc2
    serial_out();
}

void Key::decode_format(uint8_t enc1, uint8_t enc2){
    // enc1 should start with a 1
    e1 = enc1 % 10;
    // enc2 should start with a 2
    e2 = enc2 % 10;
}

void Key::serial_out(){


}
