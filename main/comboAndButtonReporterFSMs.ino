//Will make more lenient by checking for valid press interval in addition to just pressed

void combinationReporterFSM(uint8_t button1, uint8_t button2, uint8_t button3){
//void combinationReporterFSM(Button buttonObj1, Button buttonObj2, Button buttonObj3) {
   //Trying this out here
//   Serial.println("Button 1");
//   Serial.println(buttonObj1.update());
//   Serial.println(buttonObj2.update());
//   Serial.println(buttonObj3.update());
  int buttonOneResult = buttonOneReporterFSM(button1);
//  int buttonOneResult = buttonOneReporterFSM(buttonObj1);
  int buttonTwoResult = buttonTwoReporterFSM(button2);
//  int buttonTwoResult = buttonTwoReporterFSM(buttonObj2);
  int buttonThreeResult = buttonThreeReporterFSM(button3);
//  int buttonThreeResult = buttonThreeReporterFSM(buttonObj3);
  if (buttonOneResult == 1 && buttonTwoResult == 0 && buttonThreeResult == 0) {
    int combo = 0;
    char currentTimeAndNote[40];
    int noteStartTime = (millis()-songStartTime-FALL_OFFSET); 
    sprintf(currentTimeAndNote, "%d:%d,", noteStartTime, combo);
    strcat(notesString, currentTimeAndNote);
    Serial.println(notesString);
  } else if (buttonOneResult == 0 && buttonTwoResult == 1 && buttonThreeResult == 0) {
    int combo = 1;
    char currentTimeAndNote[40];
    int noteStartTime = (millis()-songStartTime-FALL_OFFSET); 
    sprintf(currentTimeAndNote, "%d:%d,", noteStartTime, combo);
    strcat(notesString, currentTimeAndNote);
    Serial.println(notesString);
  } else if (buttonOneResult == 0 && buttonTwoResult == 0 && buttonThreeResult == 1) {
    int combo = 2;
    char currentTimeAndNote[40];
    int noteStartTime = (millis()-songStartTime-FALL_OFFSET); 
    sprintf(currentTimeAndNote, "%d:%d,", noteStartTime, combo);
    strcat(notesString, currentTimeAndNote);
    Serial.println(notesString);
  } else if (buttonOneResult == 1 && buttonTwoResult == 1 && buttonThreeResult == 0) {
    int combo = 3;
    char currentTimeAndNote[40];
    int noteStartTime = (millis()-songStartTime-FALL_OFFSET); 
    sprintf(currentTimeAndNote, "%d:%d,", noteStartTime, combo);
    strcat(notesString, currentTimeAndNote);
    Serial.println(notesString);
  } else if (buttonOneResult == 1 && buttonTwoResult == 0 && buttonThreeResult == 1) {
    int combo = 4;
    char currentTimeAndNote[40];
    int noteStartTime = (millis()-songStartTime-FALL_OFFSET); 
    sprintf(currentTimeAndNote, "%d:%d,", noteStartTime, combo);
    strcat(notesString, currentTimeAndNote);
    Serial.println(notesString);
  } else if (buttonOneResult == 0 && buttonTwoResult == 1 && buttonThreeResult == 1) {
    int combo = 5;
    char currentTimeAndNote[40];
    int noteStartTime = (millis()-songStartTime-FALL_OFFSET); 
    sprintf(currentTimeAndNote, "%d:%d,", noteStartTime, combo);
    strcat(notesString, currentTimeAndNote);
    Serial.println(notesString);
  } else if (buttonOneResult == 1 && buttonTwoResult == 1 && buttonThreeResult == 1) {
    int combo = 6;
    char currentTimeAndNote[40];
    int noteStartTime = (millis()-songStartTime-FALL_OFFSET); 
    sprintf(currentTimeAndNote, "%d:%d,", noteStartTime, combo);
    strcat(notesString, currentTimeAndNote);
    Serial.println(notesString);
  }
}
int buttonOneReporterFSM(uint8_t button1) {
//int buttonOneReporterFSM(Button buttonObj1) {
  //your code here
  //***CHANGING TO ACCOUNT FOR VALID PRESS INTERVAL***
  //NOTE: no holding logic yet
  switch (buttonOneState) {
    case IDLE:
      Serial.println(buttonObj1.update());
      if (button1 == 0) {
//      if (buttonObj1.update() == 1 ||
//          buttonObj1.update() == 2) {
        buttonOneState = DOWN;
        tft.fillEllipse(34, 30, 7, 5, TFT_GREEN);
        Serial.println("VALID PRESS");
        buttonRecordTimer = millis();
        return 1;
      }
      return 0;
      break;
    case DOWN:
      if (button1) {
//      if (buttonObj1.update() == 3) {
        Serial.println("HOLD STATE");
        tft.fillEllipse(34, 30, 7, 5, TFT_CYAN);
        
        buttonOneState = IDLE;
      } 
      //Can get rid of this section by using button
      else {
        if (millis()-buttonRecordTimer>=buttonRecordLength){
          tft.fillEllipse(34, 30, 7, 5, TFT_CYAN);
        }
      }
      return 0;
      break;
  }
}

int buttonTwoReporterFSM(uint8_t button2) {
//int buttonTwoReporterFSM(Button buttonObj2) {
  //your code here
  switch (buttonTwoState) {
    case IDLE:
//      if (button2 == 0) {
      if (buttonObj2.update() == 1 ||
          buttonObj2.update() == 2) {
        buttonTwoState = DOWN;
        tft.fillEllipse(63, 30, 7, 5, TFT_GREEN);
        buttonRecordTimer = millis();
        return 1;
      }
      return 0;
      break;
    case DOWN:
      if (button2) {
//      if (buttonObj2.update() == 0) {
        tft.fillEllipse(63, 30, 7, 5, TFT_CYAN);
        buttonTwoState = IDLE;
      } else {
        if (millis()-buttonRecordTimer>=buttonRecordLength){
          tft.fillEllipse(63, 30, 7, 5, TFT_CYAN);
        }
      }
      return 0;
      break;
  }
}

int buttonThreeReporterFSM(uint8_t button3) {
//int buttonThreeReporterFSM(Button buttonObj3) {
  //your code here
  switch (buttonThreeState) {
    case IDLE:
      if (button3 == 0) {
//      if (buttonObj3.update() == 1 ||
//          buttonObj3.update() ==2) {
        buttonThreeState = DOWN;
        tft.fillEllipse(93, 30, 7, 5, TFT_GREEN);
        buttonRecordTimer = millis();
        return 1;
      }
      return 0;
      break;
    case DOWN:
      if (button3) {
//      if (buttonObj3.update() == 0) {
        tft.fillEllipse(93, 30, 7, 5, TFT_CYAN);
        buttonThreeState = IDLE;
      } 
      //Can get rid of this section by using Button class
      else {
        if (millis()-buttonRecordTimer>=buttonRecordLength){
          tft.fillEllipse(93, 30, 7, 5, TFT_CYAN);
        }
      }
      return 0;
      break;
  }
}
