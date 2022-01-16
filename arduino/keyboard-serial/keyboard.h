class Keyboard {
    private:
        uint8_t e1; // what was selected on left joystick
        uint8_t e2; // what was selected on right joystick



    public:
        void keyboard_serial(uint8_t enc1, uint8_t enc2);
        void decode_format(uint8_t enc1, uint8_t enc2);


};


void Keyboard::keyboard_serial(uint8_t enc1, uint8_t enc2){
    decode_format(enc1, enc2); // decode enc1 and enc2
}

void Keyboard::decode_format(uint8_t enc1, uint8_t enc2){
    // enc1 should start with a 1
    e1 = enc1 % 10;
    // enc2 should start with a 2
    e2 = enc2 % 10;



}