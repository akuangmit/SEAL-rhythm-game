//class Note {
//    int NOTE_COMBO;
//    int NOTE_TIME;
//    float x_pos;
//    float y_pos;
//    float fall_speed;
//    TFT_eSPI* local_tft;
//  public: 
//    Note(TFT_eSPI* tft_to_use, int note_combo, int note_time) {
//      NOTE_COMBO = note_combo;
//      NOTE_TIME = note_time;
//      if (note_combo == 0) {
//        x_pos = 34;
//      }
//      if (note_combo == 1) {
//        x_pos = 63;
//      }
//      if (note_combo == 2) {
//        x_pos = 93;
//      }
//      y_pos = 20;
//      local_tft = tft_to_use;
//    }
//    void moveNote() {
//      local_tft->fillEllipse(x_pos, y_pos, 7, 5, TFT_BLACK);
//      y_pos += 20;
//      local_tft->fillEllipse(x_pos, y_pos, 7, 5, TFT_GREEN);
//    }
//};
