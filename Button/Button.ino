#include <SPI.h>
#include <TFT_eSPI.h>
#include "Button.h"
TFT_eSPI tft = TFT_eSPI();


Button button(19);

void setup() {
  Serial.begin(115200);               // Set up serial port
  pinMode(19, INPUT_PULLUP);
  tft.init();
  tft.setRotation(2);
  tft.setTextSize(1);
  tft.fillScreen(TFT_BLACK);
}

void loop() {
  // put your main code here, to run repeatedly:
  tft.setCursor(0, 15,1);
  //Printing a lot of question marks for println
  int button_state = button.update();
//  Serial.println(button_state);
  Serial.println(digitalRead(19));
}
