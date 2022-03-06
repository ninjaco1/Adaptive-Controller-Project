#include "./keyboard_serial_header.hpp"

#define AUTO
// #define BUTTON_CONTROL

const int buttonPin1 = 2; // button for
const int buttonPin2 = 3;
Key k; // create an key object which has all the functions in it

// variables will change:
int buttonState1 = 0; // variable for reading the pushbutton status
int buttonState2 = 0; // variable for reading the pushbutton status
int lastButtonState1 = LOW;
int lastButtonState2 = LOW;

// for debouncing
unsigned long lastDebounceTime1 = 0; // the last time the output pin was toggled
unsigned long lastDebounceTime2 = 0; // the last time the output pin was toggled
unsigned long debouceDelay = 50; // the debounce time; increase if the output flickers


// joysticks count
uint8_t joystick1 = 0;
uint8_t joystick2 = 0;

// functions
void check_button(){
  // deboucing
  // https://docs.arduino.cc/built-in-examples/digital/Debounce
  int reading1, reading2;
  
  // read the state of the switch into a local variable
  reading1 = digitalRead(buttonPin1);
  reading2 = digitalRead(buttonPin2);

  // check to see if you pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // if the switch changed, due to noise or pressing:
  if(reading1 != lastButtonState1){
    // reset the debouncing timer
    lastDebounceTime1 = millis();
  }

  if(reading2 != lastButtonState2){
    // rest the bouncing timer
    lastDebounceTime2 = millis();
  }

  if((millis() - lastDebounceTime1) > debouceDelay){
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so that it as the actual current state:

    //if the button state has changed:
    if (reading1 != buttonState1){
      buttonState1 = reading1;

      // only toggle thet LEDif the new button sttae is HIGH
      if (buttonState1 == HIGH){
        // do something
        // k.keyboard_serial(17,25); // ctrl
        k.toggle_ctrl();
        k.keyboard_serial(11,24); // t
        k.toggle_ctrl();
       
      }
    }
  }
  if((millis() - lastDebounceTime2) > debouceDelay){
    //if the button state has changed:
    if (reading2 != buttonState2){
      buttonState2 = reading2;

      // only toggle thet LEDif the new button sttae is HIGH
      if (buttonState2 == HIGH){
        // do something
        // k.keyboard_serial(17,25); // ctrl
        k.toggle_ctrl();
        k.keyboard_serial(12,22); // w
        k.toggle_ctrl();        
      }
    }
  }


  lastButtonState1 = reading1;
  lastButtonState2 = reading2;

}



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
  // buttonState1 = digitalRead(buttonPin1);
  // buttonState2 = digitalRead(buttonPin2);

  // // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  // if (buttonState1 == HIGH) {
  //   // open new tab:
  //   // ctrl + t
  //   k.keyboard_serial(17,25); // ctrl
  //   k.keyboard_serial(11,24); // t
  //   k.toggle_ctrl();
  //   delay(1000);
  // } 

  // if (buttonState2 == HIGH){
  //   // close window
  //   // ctrl + w
  //   k.keyboard_serial(17,25); // ctrl
  //   k.keyboard_serial(12,22); // w
  //   k.toggle_ctrl();
  //   delay(1000);
  // }
  //****************************************************
  check_button(); // check if button 1 and two are pressed
  #endif


}
