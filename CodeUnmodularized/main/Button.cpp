#include "Arduino.h"
#include "Button.h"

Button::Button(int p) {
  state = UNPRESSED;
  pin = p;
}

//flag 0 if unpressed, 1 if just pressed, 2 if within valid press interval, 3 if held
int Button::update() {
  int flag = 0;
  uint8_t button_state = digitalRead(pin);
  switch(state) {
    case UNPRESSED:
      if (button_state ==  0) {
        state = CHECK_FROM_UNPRESSED;
        t_of_button_change = millis();
      }
      break;
    case CHECK_FROM_UNPRESSED:
      if (button_state == 1) {
        state = UNPRESSED;
      }
      else if (button_state == 0 
              && millis() - t_of_button_change >= debounce_time) {
                state = CURR_PRESSED;
                flag = 1;
                t_of_valid_press = millis();
              }
      break;
    case CURR_PRESSED:
      flag = 2;
      if (button_state == 1) {
        state = CHECK_FROM_PRESSED;
        t_of_button_change = millis();
      }
      else if (button_state == 0 
              && millis() - t_of_valid_press >= hold_time) {
                 state = HOLD;
              }
      break;
    case CHECK_FROM_PRESSED:
      //If the button is pressed
      if (button_state == 0) {
        state = CURR_PRESSED;
        flag = 2;
        t_of_valid_press = millis();
      }
      //If the button is Unpressed
      else if (button_state == 1
        && millis() - t_of_button_change >= debounce_time) {
          state = UNPRESSED; 
          flag = 0;
        }
      break;
    case HOLD:
      flag = 3;
      if (button_state == 1) {
        state = CHECK_FROM_HOLD;
        t_of_button_change = millis();
      }
      break;
    case CHECK_FROM_HOLD:
      //If the button is pressed
      if (button_state == 0) {
        state = HOLD;
        flag = 3;
      }
      //If the button is Unpressed
      else if (button_state == 1
        && millis() - t_of_button_change >= debounce_time) {
          state = UNPRESSED; 
          flag = 0;
        }
      break;
  }
  return flag;
}
