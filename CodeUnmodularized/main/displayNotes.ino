////#include "Note.h"
//
//void display_notes() {
//  Serial.println(timesIndex);
//  Serial.println(play_timer);
//  if (times[timesIndex] < (millis() - play_timer)) {
//    Note note = Note(tft, notes[timesIndex], times[timesIndex]);
//    allNotes[timesIndex] = note;
//    Serial.println(notes[timesIndex]);
//    if (notes[timesIndex] == 0 || notes[timesIndex] == 3 || notes[timesIndex] == 4 || notes[timesIndex] == 6) {
//      tft.fillEllipse(34, 20, 7, 5, TFT_GREEN);
//    }
//    if (notes[timesIndex] == 1 || notes[timesIndex] == 3 || notes[timesIndex] == 5 || notes[timesIndex] == 6) {
//      tft.fillEllipse(63, 20, 7, 5, TFT_GREEN);
//    }
//    if (notes[timesIndex] == 2 || notes[timesIndex] == 4 || notes[timesIndex] == 5 || notes[timesIndex] == 6) {
//      tft.fillEllipse(93, 20, 7, 5, TFT_GREEN);
//    }
//    timesIndex += 1;
//  }
//}
//
//void buttonOnePlayFSM(uint8_t button1) {
//}
//void buttonTwoPlayFSM(uint8_t button2) {
//}
//void buttonThreePlayFSM(uint8_t button3) {
//}
