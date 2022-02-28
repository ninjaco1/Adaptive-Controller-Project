#include "Assistive_Mouse.h"

Assistive_Mouse mouse;
int prev = 0;


void setup() {
  // put your setup code here, to run once:
  delay(5000);
  mouse.start();
}

void loop() {
  /*
  // Move left continously
  for (int i = 0; i < 40; i++) {
    mouse.move(490, 512, 0);
    delay(50);
  }

  delay(1000);

  // Move right continuously
  for (int i = 0; i < 40; i++) {
    mouse.move(530, 512, 0);
    delay(50);    
  }

  delay(1000);

  // Move up continously
  for (int i = 0; i < 40; i++) {
    mouse.move(512, 490, 0);
    delay(50);    
  }

  delay(1000);

  // Move down continously
  for (int i = 0; i < 40; i++) {
    mouse.move(512, 530, 0);
    delay(50);    
  }

  delay(1000);

  // Move left and down continously
  for (int i = 0; i < 40; i++) {
    mouse.move(490, 530, 0);
    delay(50);    
  }

  delay(1000);

  // Move right and down continously
  for (int i = 0; i < 40; i++) {
    mouse.move(530, 530, 0);
    delay(50);
  }

  delay(1000);

  // Move left and up continously
  for (int i = 0; i < 40; i++) {
    mouse.move(490, 490, 0);
    delay(50);
  }

  delay(1000);

  // Move right and up continously
  for (int i = 0; i < 40; i++) {
    mouse.move(530, 490, 0);
    delay(50);
  }

  delay(1000); */

  mouse.press(1);

  for (int i = 0; i < 60; i++) {
    mouse.move(530, 512 - ((i-20) * (i-40))/16, 0);
    delay(50);
  }

  mouse.release(1);
  
  delay(5000);

}
