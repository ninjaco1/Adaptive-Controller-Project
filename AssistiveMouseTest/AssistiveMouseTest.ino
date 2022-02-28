/*
 * Test_Mouse_Class
 * 
 * Author: Jordyn Marshall
 * Date: 1/31/22
 * 
 * This code will be sent to the Arduino and is used to test
 * whether the given mouse class is working properly or not.
 * 
 * Used for Verification
 */

 #include "Assistive_Mouse.h"

Assistive_Mouse mouse;

void setup() {
  Serial.begin(9600);
  delay(1000);
  Serial.println("Serial Communication with computer established for testing.");

  delay(1000);
  Serial.println("Instance of Assistive Mouse class created.");
  delay(5000);
  
  mouse.start();
  delay(1000);
  Serial.println("Begin mouse use.");
  delay(1000);

  /***********************************************************/

  Serial.println("\nBeginning Mouse Click Tests");
 
  Serial.println("Testing Left Mouse Click.");
  delay(1000);
  mouse.click(1);
  delay(1000);

  Serial.println("Testing Right Mouse Click.");
  delay(1000);
  mouse.click(2);
  delay(1000);

  Serial.println("Testing Middle Mouse Click.");
  delay(1000);
  mouse.click(4);
  delay(1000);

  /***********************************************************/

  Serial.println("\nBeginning Mouse Movement Tests");

  movement_test();
  movement_test_smooth();

  /***********************************************************/

  Serial.println("\nBeginning Sensitivity Tests");

  mouse.set_sensitivity(0.25);
  Serial.println("\nSensitivity is set to 0.25. Beginning movement test.");
  delay(1000);
  movement_test();
  movement_test_smooth();

  mouse.set_sensitivity(0.5);
  Serial.println("\nSensitivity is set to 0.5. Beginning movement test.");
  delay(1000);
  movement_test();
  movement_test_smooth();

  mouse.set_sensitivity(2);
  Serial.println("\nSensitivity is set to 2. Beginning movement test.");
  delay(1000);
  movement_test();  
  movement_test_smooth();

  mouse.set_sensitivity(4);
  Serial.println("\nSensitivity is set to 4. Beginning movement test.");
  delay(1000);
  movement_test();
  movement_test_smooth();

  /***********************************************************/

  Serial.println("\nSetting sensitivity back to 1. Beginning Movement Range Tests");
  mouse.set_sensitivity(1);
  delay(1000);

  Serial.println("Moving Mouse w/ Coordinates (0, 0) (Max left and up)");
  delay(1000);
  mouse.move(0, 0, 512);
  delay(1000);

  Serial.println("Moving Mouse w/ Coordinates (128, 128) (Far left and up)");
  delay(1000);
  mouse.move(128, 128, 512);
  delay(1000);
  
  Serial.println("Moving Mouse w/ Coordinates (256, 256) (More left and up)");
  delay(1000);
  mouse.move(256, 256, 512);
  delay(1000);

  Serial.println("Moving Mouse w/ Coordinates (384, 384) (Slight left and up)");
  delay(1000);
  mouse.move(384, 384, 512);  
  delay(1000);

  Serial.println("Moving Mouse w/ Coordinates (512, 512) (No movement)");
  delay(1000);
  mouse.move(512, 512, 512); 
  delay(1000);

  Serial.println("Moving Mouse w/ Coordinates (640, 640) (Slight right and down)");
  delay(1000);
  mouse.move(640, 640, 512);
  delay(1000);

  Serial.println("Moving Mouse w/ Coordinates (768, 768) (More right and down)");
  delay(1000);
  mouse.move(768, 768, 512); 
  delay(1000);

  Serial.println("Moving Mouse w/ Coordinates (896, 896) (Far right and down)");
  delay(1000);
  mouse.move(896, 896, 512);
  delay(1000);   

  Serial.println("Moving Mouse w/ Coordinates (1024, 1024) (Max right and down)");
  delay(1000);
  mouse.move(1024, 1024, 512);
  delay(1000);

  /***********************************************************/

  Serial.println("\nTests complete. Moving to Blink program.");
  Serial.println("Well uh blink doesn't work because the pin isn't right but its not a problem.");

  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);  
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
}

void movement_test() {
  Serial.println("Moving mouse left.");
  delay(1000);
  mouse.move(412, 512, 512);
  delay(1000);

  Serial.println("Moving mouse right.");
  delay(1000);
  mouse.move(612, 512, 512);
  delay(1000);

  Serial.println("Moving mouse up.");
  delay(1000);
  mouse.move(512, 412, 512);
  delay(1000);

  Serial.println("Moving mouse down.");
  delay(1000);
  mouse.move(512, 612, 512);
  delay(1000);

  Serial.println("Moving mouse left and up.");
  delay(1000);
  mouse.move(412, 412, 512);
  delay(1000);

  Serial.println("Moving mouse left and down.");
  delay(1000);
  mouse.move(412, 612, 512);
  delay(1000);

  Serial.println("Moving mouse right and up.");
  delay(1000);
  mouse.move(612, 412, 512);
  delay(1000);
  
  Serial.println("Moving mouse right and down.");
  delay(1000);
  mouse.move(612, 612, 512);
  delay(1000);  
}

void movement_test_smooth() {
  // Move left continously
  Serial.println("Moving left continously.");
  for (int i = 0; i < 40; i++) {
    mouse.move(490, 512, 512);
    delay(50);
  }

  delay(1000);

  // Move right continously
  Serial.println("Moving right continously.");
  for (int i = 0; i < 40; i++) {
    mouse.move(530, 512, 512);
    delay(50);    
  }

  delay(1000);

  // Move up continously
  Serial.println("Moving up continously.");
  for (int i = 0; i < 40; i++) {
    mouse.move(512, 490, 512);
    delay(50);    
  }

  delay(1000);

  // Move down continously
  Serial.println("Moving down continously.");
  for (int i = 0; i < 40; i++) {
    mouse.move(512, 530, 512);
    delay(50);    
  }

  delay(1000);

  // Move left and down continously
  Serial.println("Moving left and down continously.");
  for (int i = 0; i < 40; i++) {
    mouse.move(490, 530, 512);
    delay(50);    
  }

  delay(1000);

  // Move right and down continously
  Serial.println("Moving right and down continously.");
  for (int i = 0; i < 40; i++) {
    mouse.move(530, 530, 512);
    delay(50);
  }

  delay(1000);

  // Move left and up continously
  Serial.println("Moving left and up continously.");
  for (int i = 0; i < 40; i++) {
    mouse.move(490, 490, 512);
    delay(50);
  }

  delay(1000);

  // Move right and up continously
  Serial.println("Moving right and up continously.");
  for (int i = 0; i < 40; i++) {
    mouse.move(530, 490, 512);
    delay(50);
  }
}
