#include "./keyboard_serial_header.hpp"

// #define AUTO
#define BUTTON_CONTROL

const int buttonPin1 = 2; // button for
const int buttonPin2 = 3;
Key k; // create an key object which has all the functions in it

// variables will change:
int buttonState1 = 0; // variable for reading the pushbutton status
int buttonState2 = 0; // variable for reading the pushbutton status

// joysticks count
uint8_t joystick1 = 0;
uint8_t joystick2 = 0;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  // setting up the buttons so that it can be read
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
}

void loop()
{
  // put your main code here, to run repeatedly:
  #ifdef AUTO
  for (int joystick1 = 10; joystick1 < 18; joystick1++)
  {
    for (int joystick2 = 20; joystick2 < 28; joystick2++)
    {
      // printing out the status of each thing
      Serial.print("joystick 1: ");
      Serial.print(joystick1, DEC);
      Serial.print("\t");

      Serial.print(", joystick 2: ");
      Serial.print(joystick2, DEC);
      Serial.print("\t");

      Serial.print(", e1: ");
      Serial.print(k.get_e1(), DEC);
      Serial.print("\t");

      Serial.print(", e2: ");
      Serial.print(k.get_e2(), DEC);
      Serial.println("\t");

      // checking the joystick values
      k.keyboard_serial(joystick1, joystick2);

      delay(30); // making sure each switch is working
    }
  }

  // testing with shift
  k.toggle_shift();

  for (int joystick1 = 10; joystick1 < 18; joystick1++)
  {
    for (int joystick2 = 20; joystick2 < 28; joystick2++)
    {
      // printing out the status of each thing
      Serial.print("joystick 1: ");
      Serial.print(joystick1, DEC);
      Serial.print("\t");

      Serial.print(", joystick 2: ");
      Serial.print(joystick2, DEC);
      Serial.print("\t");

      Serial.print(", e1: ");
      Serial.print(k.get_e1(), DEC);
      Serial.print("\t");

      Serial.print(", e2: ");
      Serial.print(k.get_e2(), DEC);
      Serial.println("\t");

      // checking the joystick values
      k.keyboard_serial(joystick1, joystick2);

      delay(30); // making sure each switch is working
    }
  }

  // turning off shift
  k.toggle_shift();

  #endif

  #ifdef BUTTON_CONTROL
  //  for marcos to see if they work
  buttonState1 = digitalRead(buttonPin1);
  buttonState2 = digitalRead(buttonPin2);

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState1 == HIGH) {
    // open new tab:
    // ctrl + t
    k.keyboard_serial(17,25); // ctrl
    k.keyboard_serial(11,24); // t
    k.toggle_ctrl();
    delay(1000);
  } 

  if (buttonState2 == HIGH){
    // close window
    // ctrl + w
    k.keyboard_serial(17,25); // ctrl
    k.keyboard_serial(12,22); // w
    k.toggle_ctrl();
    delay(1000);
  }

  #endif


}
