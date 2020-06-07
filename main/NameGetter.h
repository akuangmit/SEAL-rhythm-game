//Started doing it, feel free to edit
#ifndef NameGetter_h
#define NameGetter_h
#include "Arduino.h"

class NameGetter {
    private:
      char alphabet[50] = " ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
      char message[400] = {0};
      char intermediate[400] = {0};
      int char_index;
      int state;
      unsigned long scrolling_timer;
      const int scrolling_threshold = 150;
      const float angle_threshold = 0.3;
    
    public:
      NameGetter();
      void update(float angle, int button, char* output);
};
#endif
