//class NameGetter {
//    char alphabet[50] = " ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
//    char message[400] = {0};
//    int char_index;
//    int state;
//    unsigned long scrolling_timer;
//    const int scrolling_threshold = 150;
//    const float angle_threshold = 0.3;
//    
//    public:
//
//    NameGetter() {
//      state = 1;
//      memset(message, 0, sizeof(message));
//      char_index = 0;
//      scrolling_timer = millis();
//    }
//
//    void update(float angle, int button, char* output) {
//      Serial.println("update");
//      Serial.println(state);
//      Serial.println("output");
//      Serial.println(output);
//      if (state == 1) {
//        if(button == 1) { //button 1 pressed
//          char letter[2] = "";
//          letter[0] = alphabet[char_index];
//          letter[1] = '\0';
//          Serial.println("letter");
//          Serial.println(letter);
//          strcat(message, letter);
////          Serial.println("query_string");
////          Serial.println(query_string);
//          memset(output, 0, sizeof(output)); //reset!!! before strcpy
////          strcpy(output, query_string); //?? is this correct??
//          strcpy(output, message);        
//          char_index = 0;
//        } else if (button == 2) { //button 2 pressed
//          state = 2;
//          memset(output, 0, sizeof(output));
//        } else { 
//          if (millis()-scrolling_timer >= scrolling_threshold) {
//            if (angle < -angle_threshold) { //maybe left?
//            char_index = (char_index +36)%37; //total 37 chars in 'alphabet'
//            } else if (angle > angle_threshold) { //maybe right?
//              char_index = (char_index + 1)%37;
//            }
//            scrolling_timer = millis();
//          }
//          
//          char letter[2] = "";
//          letter[0] = alphabet[char_index];
//          letter[1] = '\0';
//          char temp[50] = "";
////          strcpy(temp, query_string);
//          strcat(temp, letter);
//          Serial.println("temp");
//          Serial.println(temp);
//          memset(output, 0, sizeof(output)); //reset!!! before strcpy
//          strcpy(output, temp);
//        }
//      } else if (state == 2) {
//        state = 0;
//        strcpy(output, message);
//      } 
//    }
//};
