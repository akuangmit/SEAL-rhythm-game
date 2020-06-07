#ifndef Note_h
#define Note_h
#include "Arduino.h"
#include <TFT_eSPI.h>
class Note
{
  private:
  int NOTE_COMBO;
  int NOTE_TIME;
  float x_pos;
  float y_pos;
  float fall_speed;
  uint16_t color;
  TFT_eSPI* local_tft;
  public:
    Note();
    Note(TFT_eSPI* tft_to_use, int note_combo, int note_time);
    void moveNote(TFT_eSPI* tft_to_use);
    float getX();
    int getNoteCombo();
    int getNoteTime();
    float getY();
    float getFallSpeed();
    int getColor();
    void setColor();
};
#endif
