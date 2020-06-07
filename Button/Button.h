#ifndef Button_h
#define Button_h
#include "Arduino.h"

class Button {
  //Not sure if defined states can be cycled through in implementation (in scope?)
  #define UNPRESSED 0
  #define CHECK_FROM_UNPRESSED 1
  #define CURR_PRESSED 2
  #define CHECK_FROM_PRESSED 3
  #define HOLD 4
  #define CHECK_FROM_HOLD 5
  private:
    int state;
    uint32_t t_of_button_change; 
    uint32_t t_of_valid_press;   
    uint32_t debounce_time = 40;
    uint32_t hold_time = 60;
    uint8_t pin;
  public: 
    Button(int p);
//    bool validPress();
    /*
     * Gets an int that represents a valid press: 0 = invalid, 1 = valid
     */
    int update();
};
#endif
