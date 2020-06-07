#include "Arduino.h"
#include "Note.h"
#include <TFT_eSPI.h>

Note::Note() {
  x_pos = -40;
  y_pos = 0;
//  Serial.println("TFT_GREEN");
//  Serial.println(TFT_GREEN);
//  color = TFT_GREEN;
}
Note::Note(TFT_eSPI* tft_to_use, int note_combo, int note_time) {
  NOTE_COMBO = note_combo;
//  Serial.println("NOTE_COMBO");
//  Serial.println(NOTE_COMBO);
  NOTE_TIME = note_time;
  color = TFT_GREEN;
//  Serial.print("note time: ");
//  Serial.println(note_time);
//  Serial.print("color: ");
//  Serial.println(color);
  if (note_combo == 0) {
    x_pos = 34;
  }
  if (note_combo == 1) {
    x_pos = 63;
  }
  if (note_combo == 2) {
    x_pos = 93;
  }
  y_pos = 20;
  local_tft = tft_to_use;
  local_tft->fillEllipse(x_pos, y_pos, 7, 5, TFT_BLACK);
  if (x_pos >=15){
//    Serial.println("color");
//    Serial.println(color);
//    Serial.println("TFT_GREEN");
//    Serial.println(TFT_GREEN);
    local_tft->fillEllipse(x_pos, y_pos, 7, 5, color);
//    local_tft->fillEllipse(x_pos, y_pos, 7, 5, TFT_GREEN);
    //Serial.println("Note displayed");
  }
}

void Note::moveNote(TFT_eSPI* tft_to_use) {
  tft_to_use->fillEllipse(x_pos, y_pos, 7, 5, TFT_BLACK);
  y_pos += 5;
  if (x_pos>=15){
    tft_to_use->fillEllipse(x_pos, y_pos, 7, 5, color);
//    tft_to_use->fillEllipse(x_pos, y_pos, 7, 5, TFT_GREEN);
  }
}

float Note::getX() {
  return x_pos;
}

int Note::getNoteCombo(){
  return NOTE_COMBO;
}

int Note::getNoteTime(){
  return NOTE_TIME;
}

float Note::getY(){
  return y_pos;
}

float Note::getFallSpeed(){
  return fall_speed;
}

int Note::getColor(){
  return color;
}

void Note::setColor(){
  color = TFT_BLACK;
}
