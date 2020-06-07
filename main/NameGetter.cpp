//started doing it, feel free to edit
#include "Arduino.h"
#include "NameGetter.h"

  NameGetter::NameGetter() {
    state = 1;
    memset(message, 0, sizeof(message));
    memset(intermediate, 0, sizeof(intermediate));
    char_index = 0;
    scrolling_timer = millis();
  }
  void NameGetter::update(float angle, int button, char* output) {
    if (state == 1) {
      if(button == 1) { //button 1 pressed
        char letter[2] = "";
        letter[0] = alphabet[char_index];
        letter[1] = '\0';
        strcat(message, letter);
        
        memset(output, 0, sizeof(output)); //reset!!! before strcpy
        strcpy(output, message);      
        char_index = 0;
      } else if (button == 2) { //button 2 pressed
        state = 2;
//          memset(output, 0, sizeof(output));
      } else { 
        if (millis()-scrolling_timer >= scrolling_threshold) {
          if (angle < -angle_threshold) { //maybe left?
          char_index = (char_index +36)%37; //total 37 chars in 'alphabet'
          } else if (angle > angle_threshold) { //maybe right?
            char_index = (char_index + 1)%37;
          }
          scrolling_timer = millis();
        }
//          Serial.print("char index: ");
//          Serial.println(char_index);
        char letter[2] = "";
        letter[0] = alphabet[char_index];
        letter[1] = '\0';
        char temp[50] = "";
        strcat(temp, letter);
//          Serial.print("temp: ");
//          Serial.println(temp);
        memset(intermediate, 0, sizeof(intermediate));
        strcpy(intermediate, message);
        strcat(intermediate, temp);
//          Serial.print("intermediate: ");
//          Serial.println(intermediate);
        memset(output, 0, sizeof(output)); //reset!!! before strcpy
        strcpy(output, intermediate);
      }
    } else if (state == 2) {
      state = 1;
//        strcpy(output, message);
    } 
  }
