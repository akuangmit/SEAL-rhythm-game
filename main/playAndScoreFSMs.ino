#include <typeinfo>
#include "Note.h"
void scoreReporterFSM(uint8_t button1, uint8_t button2, uint8_t button3){
//void scoreReporterFSM(Button button1, Button button2, Button button3) {
  int score_buffer = 60; //1000 before
  int offset = 920;
  int buttonOneResult = buttonOnePlayFSM(button1);
  int buttonTwoResult = buttonTwoPlayFSM(button2);
  int buttonThreeResult = buttonThreePlayFSM(button3);
  int currentNote = notes[scoreIndex];
  int currentNoteTime = times[scoreIndex] + offset;
  int tempIndex = scoreIndex;
  boolean indexStop = false;
  boolean wasPressed = false;
//  Serial.print("Buttons: ");
//  Serial.print(buttonOneResult);
//  Serial.print(buttonTwoResult);
//  Serial.println(buttonThreeResult);
  //boolean wasPressed = (buttonOneResult == 0 or buttonTwoResult == 0 or buttonThreeResult == 0); //initialized to false if no button pressed
//  ledcWriteTone(PWNchannel, -1);
  ledcWrite(PWNchannel, 0);
  tft.setCursor(15,150,1);
  tft.println("                                                  ");
  if (buttonOneResult == 1 or buttonTwoResult == 1 or buttonThreeResult == 1) {
    wasPressed = true;
  }
//  Serial.print("wasPressed: ");
//  Serial.println(wasPressed);
//  
  int old_score = score;
    
  while ((int)times[tempIndex] + offset < (int)((millis() - play_timer) + 3*score_buffer)) { //while note has fallen past top line
    if ((int)times[tempIndex] >= 0) {
      indexStop = true;
    }
    else if (!indexStop) { //when negative time and have not found positive time yet
      scoreIndex++;
    }
    if (indexStop && (int)times[tempIndex] >= 0) {
        if ((int)times[tempIndex] + offset < (int)((millis() - play_timer) - 3*score_buffer)) { //if note has fallen past bottom line
            scoreIndex++;
        }
        else if ((notes[tempIndex] == 0 && buttonOneResult == 1 && buttonTwoResult == 0 && buttonThreeResult == 0) 
          || (notes[tempIndex] == 1 && buttonOneResult == 0 && buttonTwoResult == 1 && buttonThreeResult == 0)
          || (notes[tempIndex] == 2 && buttonOneResult == 0 && buttonTwoResult == 0 && buttonThreeResult == 1)
          || (notes[tempIndex] == 3 && buttonOneResult == 1 && buttonTwoResult == 1 && buttonThreeResult == 0)
          || (notes[tempIndex] == 4 && buttonOneResult == 1 && buttonTwoResult == 0 && buttonThreeResult == 1)
          || (notes[tempIndex] == 5 && buttonOneResult == 0 && buttonTwoResult == 1 && buttonThreeResult == 1)
          || (notes[tempIndex] == 6 && buttonOneResult == 1 && buttonTwoResult == 1 && buttonThreeResult == 1)) {
          if ((int)times[tempIndex] + offset > (int)((millis() - play_timer) - score_buffer) && (int)times[tempIndex] + offset < (int)((millis() - play_timer) + score_buffer)) {
              score += 3;
              tft.setCursor(80,150,1);
              tft.println("Perfect");
            }
            else if ((int)times[tempIndex] + offset > (int)((millis() - play_timer) - 2*score_buffer) && (int)times[tempIndex] + offset < (int)((millis() - play_timer) + 2*score_buffer)) {
              score += 2;
              tft.setCursor(80,150,1);
              tft.println("Great");
            }
            else {
              score += 1;
              tft.setCursor(80,150,1);
              tft.println("OK");
            }
            allNotes[tempIndex].setColor();
            times[tempIndex] = -1;
            break;
        }
    }
    tempIndex++;
//    Serial.println(score);
  }
  if (score == old_score and wasPressed) {
    Serial.println("Incorrect Press! -1");
    ledcWriteTone(PWNchannel, 2000);
    ledcWrite(PWNchannel, 125);
    score -= 1; 
  }
  tft.setCursor(15,150,1);
  tft.print("Score: ");
  tft.println(score);
}

int buttonOnePlayFSM(uint8_t button1) {
//int buttonOnePlayFSM(Button button1) {
  switch (buttonOneState) {
    case IDLE:
      if (button1 == 0) {
//      if (button1.update() == 1) {
        buttonOneState = DOWN;
        tft.fillEllipse(34, 130, 7, 5, TFT_GREEN);
        buttonRecordTimer = millis();
        return 1;
      }
      return 0;
      break;
    case DOWN:
      if (button1) {
//      if (button1.update() == 0) {
        tft.fillEllipse(34, 130, 7, 5, TFT_CYAN);
        buttonOneState = IDLE;
      } 
      //Can get rid of this section by using button
      else {
        if (millis()-buttonRecordTimer>=buttonRecordLength){
          tft.fillEllipse(34, 130, 7, 5, TFT_CYAN);
        }
      }
      return 0;
      break;
  }
}
int buttonTwoPlayFSM(uint8_t button2) {
//int buttonTwoPlayFSM(Button button2) {
  switch (buttonTwoState) {
    case IDLE:
      if (button2 == 0) {
//      if (button2.update() == 1) {
        buttonTwoState = DOWN;
        tft.fillEllipse(63, 130, 7, 5, TFT_GREEN);
        buttonRecordTimer = millis();
        return 1;
      }
      return 0;
      break;
    case DOWN:
      if (button2) {
//      if (button2.update() == 0) {
        tft.fillEllipse(63, 130, 7, 5, TFT_CYAN);
        buttonTwoState = IDLE;
      } 
      //Can get rid of this section by using button
      else {
        if (millis()-buttonRecordTimer>=buttonRecordLength){
          tft.fillEllipse(63, 130, 7, 5, TFT_CYAN);
        }
      }
      return 0;
      break;
  }
}

int buttonThreePlayFSM(uint8_t button3) {
//int buttonThreePlayFSM(Button button3) {
  switch (buttonThreeState) {
    case IDLE:
      if (button3 == 0) {
//      if (button3.update() == 1) {
        buttonThreeState = DOWN;
        tft.fillEllipse(93, 130, 7, 5, TFT_GREEN);
        buttonRecordTimer = millis();
        return 1;
      }
      return 0;
      break;
    case DOWN:
      if (button3) {
//      if (button3.update() == 0) {
        tft.fillEllipse(93, 130, 7, 5, TFT_CYAN);
        buttonThreeState = IDLE;
      } 
      //Can get rid of this section by using button
      else {
        if (millis()-buttonRecordTimer>=buttonRecordLength){
          tft.fillEllipse(93, 130, 7, 5, TFT_CYAN);
        }
      }
      return 0;
      break;
  }
} 
