void display_name_header() {
  tft.fillScreen(TFT_BLACK); //fill background
  tft.setCursor(0,0,1); // set the cursor
  tft.println("Welcome to SEAL! To start, please enter your name. Tilt left and right to scroll through letters, press 1 to lock in letter and 3 to enter name.");
  tft.println();
  tft.println(name_str);
}

void display_name() {
  tft.setCursor(0,70,1); // set the cursor
  tft.println(name_str);
}

void display_home() {
  tft.fillScreen(TFT_BLACK); //fill background
  tft.setCursor(0,0,1); // set the cursor
  tft.print(name_str);
  tft.print(", ");
  tft.println("Welcome to SEAL!:) ");
  tft.print("Press 1 to record, 2 to select a track.");
}

void display_record() {
  tft.fillScreen(TFT_BLACK);
  tft.drawRect(15,15,100,130,TFT_BLUE);
  tft.drawLine(15,30,115,30, TFT_PINK);
  tft.fillEllipse(34, 30, 7, 5, TFT_CYAN);
  tft.fillEllipse(63, 30, 7, 5, TFT_CYAN);
  tft.fillEllipse(93, 30, 7, 5, TFT_CYAN);
  buttonOneState = IDLE;
  buttonTwoState = IDLE;
  buttonThreeState = IDLE;
  songStartTime = millis(); //adjust later for actual mp3 start
}

void display_selection() {
  tft.fillScreen(TFT_BLACK); //fill background
  tft.setCursor(0,0,1); // set the cursor
  tft.println("Select a track to play:");
  tft.print("Press 1 to scroll up, 2 to scroll down, and 3 to select.");
}

void displayRecordSelect(){
  tft.fillScreen(TFT_BLACK); //fill background
  tft.setCursor(0,0,1); // set the cursor
  tft.println("Select a track to record:");
  tft.print("Press 1 to scroll up, 2 to scroll down, and 3 to select.");  
}

void display_record_confirmation(int songIndex, char* new_str){
  tft.fillScreen(TFT_BLACK); //fill background
  tft.setCursor(0,0,1); // set the cursor
  tft.println("Recorded!");
  tft.println("");
  tft.print("Song: ");
  tft.println(songMap[songIndex]); 
  tft.println("");
  tft.print("Artist: ");
  tft.println(authorMap[songIndex]);
  tft.println("");
  tft.print("Recorded by user ");
  tft.println(new_str);
  tft.println("");
  tft.print("Press 3 to return to HOME screen");  
  
}

void display_play() {
  tft.fillScreen(TFT_BLACK);
  tft.drawRect(15,15,100,130,TFT_BLUE); //bottom screen at 145
  tft.drawLine(15,130,115,130, TFT_PINK);
  tft.fillEllipse(34, 130, 7, 5, TFT_CYAN);
  tft.fillEllipse(63, 130, 7, 5, TFT_CYAN);
  tft.fillEllipse(93, 130, 7, 5, TFT_CYAN);
  buttonOneState = IDLE;
  buttonTwoState = IDLE;
  buttonThreeState = IDLE;
  songStartTime = millis();
//  for (int i = 0; i < 200; i++) {
//    Serial.println("notes");
//    Serial.println(notes[i]);
//    Serial.println("allNotes");
//    Serial.println(allNotes[i].getNoteCombo());
//  }
//  tft.fillScreen(TFT_BLACK); //fill background
//  tft.setCursor(0,0,1); // set the cursor
//  tft.print("playing game!");
}

void display_scores(char *globalUserAndScore) {
  tft.fillScreen(TFT_BLACK); //fill background
  tft.setCursor(0,0,1); // set the cursor
  tft.print("Your score: ");
  tft.println(score);
  tft.print("High score: ");
  tft.println(globalUserAndScore);
  tft.println("Press 1 to play same song again, 3 to return to home!");
}

void display_notes() {
  // Serial.println(sizeof(allNotes)/sizeof(allNotes[0])); //In case we want of size of something
  for (int i = 0; i < allNotesIndex; i++) {
    allNotes[i].moveNote(&tft);
  }
  tft.drawRect(15,15,100,130,TFT_BLUE); //bottom screen at 145
  tft.drawLine(15,130,115,130, TFT_PINK);
  tft.fillEllipse(34, 130, 7, 5, TFT_CYAN);
  tft.fillEllipse(63, 130, 7, 5, TFT_CYAN);
  tft.fillEllipse(93, 130, 7, 5, TFT_CYAN);
//  Serial.print("time: ");
//  Serial.println(times[timesIndex]);
  
  if ((times[timesIndex]<=0)) {
  }
  else if ((times[timesIndex]>0) && (times[timesIndex] < (millis() - play_timer))) {
//    Serial.println("reinitialize notes");
    Note note(&tft, notes[timesIndex], times[timesIndex]);
    allNotes[allNotesIndex] = note;
    allNotesIndex++;
    timesIndex += 1;
  }
}
