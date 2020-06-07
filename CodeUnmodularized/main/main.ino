#include<string.h>
#include <WiFi.h> //Connect to WiFi Network
#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip
#include <SPI.h>
#include <mpu9255_esp32.h>
#include "Button.h"
TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h

//19, 5, 16 in the actual board
const int input_pin_1 = 5;
const int input_pin_2 = 16;
const int input_pin_3 = 19; //this pin is maybe buggy?
//Doesn't work: 6, 17
#include <Arduino.h>
#include <DFRobotDFPlayerMini.h>
HardwareSerial mySoftwareSerial(2);
DFRobotDFPlayerMini myDFPlayer;
MPU9255 imu; 
void printDetail(uint8_t type, int value);

char network[] = "MIT";  //SSID 
char password[] = ""; //Password
//char network[] = "6s08";  //SSID 
//char password[] = "iesc6s08"; //Password
//Some constants and some resources:
const int RESPONSE_TIMEOUT = 6000; //ms to wait for response from host

const uint16_t IN_BUFFER_SIZE = 5000; //size of buffer to hold HTTP request
const uint16_t OUT_BUFFER_SIZE = 5000; //size of buffer to hold HTTP response
char request_buffer[IN_BUFFER_SIZE]; //char array buffer to hold HTTP request
char response_buffer[OUT_BUFFER_SIZE]; //char array buffer to hold HTTP response

char old_name_str[OUT_BUFFER_SIZE]; //char array buffer to hold HTTP request
char name_str[OUT_BUFFER_SIZE]; //char array buffer to hold HTTP request

const int DT = 40; //milliseconds
uint32_t primary_timer; //main loop timer
uint32_t sample_timer; //timer for button sampling
uint32_t play_timer; //timer for playing game
uint8_t buttonOneState;
uint8_t buttonTwoState;
uint8_t buttonThreeState;
#define IDLE 0
#define DOWN 1
float buttonRecordLength = 300;
float buttonRecordTimer;
//uint32_t homeToRecordDelay = 3000;
uint32_t homeToRecordDelay = 1000;
uint32_t homeToRecordTimer;
uint32_t selectionToPlayTimer;
uint32_t selectionToPlayDelay = 1000;
uint32_t DFPPlayerResetTime = 10000;

char notesString[OUT_BUFFER_SIZE];
int songStartTime; 
int sample_period = 62; //how often to check button states
int screen_state; 
int allNotesIndex;
#define HOME 0
#define RECORD_SELECT 1
#define RECORD 2
#define PLAY 3
#define SCORES 4
#define PLAY_SELECT 5
#define NAME 6
#define SELECTION 7
#define SENDNOTELIST 8
#define SENDANDGETSCORE 9
#define RECORD_TRANSITION 10
#define PLAY_TRANSITION 11

int times[200] = {-1}; 
int notes[200] = {-1};
const int NUM_NOTES = 200;
const int FALL_OFFSET = 1040;
char recordedString[OUT_BUFFER_SIZE]; //potentially lower
int timesIndex = 0;

int score = 0;
int scoreIndex = 0;

class Note {
    int NOTE_COMBO;
    int NOTE_TIME;
    float x_pos;
    float y_pos;
    float fall_speed;
    TFT_eSPI* local_tft;
  public: 
    Note(){
      x_pos = -40;
      y_pos = 0;
    }
    Note(TFT_eSPI* tft_to_use, int note_combo, int note_time) {
      NOTE_COMBO = note_combo;
      NOTE_TIME = note_time;
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
      tft.fillEllipse(x_pos, y_pos, 7, 5, TFT_BLACK);
      if (x_pos >=15){
        tft.fillEllipse(x_pos, y_pos, 7, 5, TFT_GREEN);
      }
    }
    void moveNote() {
      tft.fillEllipse(x_pos, y_pos, 7, 5, TFT_BLACK);
      y_pos += 5;
      if (x_pos>=15){
        tft.fillEllipse(x_pos, y_pos, 7, 5, TFT_GREEN);
      }
    }
    float getX() {
      return x_pos;
    }
    int getNoteCombo(){
      return NOTE_COMBO;
    }
    int getNoteTime(){
      return NOTE_TIME;
    }
    float getY(){
      return y_pos;
    }
    float getFallSpeed(){
      return fall_speed;
    }
};

Note allNotes[NUM_NOTES];
void parseNotesToArrays(char *recordedString, int *times/*Name of start time array*/, int* notes/*Name of note permutation array*/){
  // Global string of all notes: “time:permutation,time:permutation,time:permutation,...”
  char* pch = strtok(recordedString, ":");
  int index = 0;
  while (pch!=NULL){
    //pch currently points to "time"
    int noteStartTime = atoi(pch);
    times[index] = noteStartTime;
    pch = strtok(NULL, ",");
    notes[index] = atoi(pch);
    index+=1;
    pch = strtok(NULL, ":");
  }
}

//used to get x,y values from IMU accelerometer!
void get_angle(float* x, float* y) {
  imu.readAccelData(imu.accelCount);
  *x = imu.accelCount[0] * imu.aRes;
  //Changed this to negative
  *y = - imu.accelCount[1] * imu.aRes;
}
class NameGetter {
    char alphabet[50] = " ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    char message[400] = {0};
    char intermediate[400] = {0};
    int char_index;
    int state;
    unsigned long scrolling_timer;
    const int scrolling_threshold = 150;
    const float angle_threshold = 0.3;
    
    public:

    NameGetter() {
      state = 1;
      memset(message, 0, sizeof(message));
      memset(intermediate, 0, sizeof(intermediate));
      char_index = 0;
      scrolling_timer = millis();
    }

    void update(float angle, int button, char* output) {
//      Serial.print("state: ");
//      Serial.println(state);   
//      Serial.print("output: ");
//      Serial.println(output);
      if (state == 1) {
        if(button == 1) { //button 1 pressed
          char letter[2] = "";
          letter[0] = alphabet[char_index];
          letter[1] = '\0';
//          Serial.println("letter");
//          Serial.println(letter);
          strcat(message, letter);
          
          memset(output, 0, sizeof(output)); //reset!!! before strcpy
//          strcpy(output, query_string); //?? is this correct??
          strcpy(output, message);      
//          Serial.print("output: ");
//          Serial.println(output);  
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
};

NameGetter ng;
//Button button1 = Button(input_pin_1);
//Button button2 = Button(input_pin_2);
//Button button3 = Button(input_pin_3);

void setup(){
    mySoftwareSerial.begin(9600, SERIAL_8N1, 32, 33);
    Serial.begin(115200); //initialize serial!
    tft.init(); //initialize the screen
    tft.setRotation(2); //set rotation for our layout

    WiFi.begin(network,password); //attempt to connect to wifi
    uint8_t count = 0; //count used for Wifi check times
    Serial.print("Attempting to connect to ");
    Serial.println(network);
    while (WiFi.status() != WL_CONNECTED && count<12) {
      delay(500);
      Serial.print(".");
      count++;
    }
    delay(2000);
    if (WiFi.isConnected()) { //if we connected then print our IP, Mac, and SSID we're on
      Serial.println("CONNECTED!");
      Serial.println(WiFi.localIP().toString() + " (" + WiFi.macAddress() + ") (" + WiFi.SSID() + ")");
      delay(500);
    } else { //if we failed to connect just Try again.
      Serial.println("Failed to Connect :/  Going to restart");
      Serial.println(WiFi.status());
      ESP.restart(); // restart the ESP (proper way)
    }
    if (imu.setupIMU(1)) {
      Serial.println("IMU Connected!");
    } else {
      Serial.println("IMU Not Connected :/");
      Serial.println("Restarting");
      ESP.restart(); // restart the ESP (proper way)
    }
    pinMode(input_pin_1,INPUT_PULLUP); //sets IO pin 15 as an input which defaults to a 3.3V signal when unconnected and 0V when the switch is pushed
    pinMode(input_pin_2,INPUT_PULLUP);
    pinMode(input_pin_3,INPUT_PULLUP);
    display_name();
    screen_state = NAME;
    timesIndex = 0;
    if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
      
      Serial.println(myDFPlayer.readType(),HEX);
      Serial.println(F("Unable to begin:"));
      Serial.println(F("1.Please recheck the connection!"));
      Serial.println(F("2.Please insert the SD card!"));
      while(true);
    }
    Serial.println(F("DFPlayer Mini online."));
    myDFPlayer.setTimeOut(500);
    myDFPlayer.volume(16);
    myDFPlayer.volumeUp(); //Volume Up
    myDFPlayer.volumeDown(); //Volume Down

  myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);
  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);
  int delayms=100;
  allNotesIndex = 0;
}

void loop(){
  //REPLACE THIS SECTION WITH BUTTONS
  uint8_t button1 = digitalRead(input_pin_1);
  uint8_t button2 = digitalRead(input_pin_2);
  uint8_t button3 = digitalRead(input_pin_3);
  //Serial.println(screen_state);
  switch (screen_state){
    case NAME : {
      float x, y;
      get_angle(&x, &y);
      int bv = 0;
//      if (button1.update() == 1) {
//         bv = 1;
//      }
      if (button1 == 0) { bv = 1;}
//      Serial.print("angle y: ");
//      Serial.println(-y);
      ng.update(-y, bv, name_str);

      if (strcmp(name_str, old_name_str) != 0) {//only draw if changed!
        display_name();
      }
      memset(old_name_str, 0, sizeof(old_name_str));
      strcat(old_name_str, name_str);
      Serial.print("name_str: ");
      Serial.println(name_str);
//      Serial.println(button1.update());
      
      if (button3 == 0) { //down
//      if (button3.update() == 1) {  
        screen_state = HOME;
        display_home();
      }
      
      break;
    } case HOME: {
      // resetInitialConditions();
//      if (button1.update() == 1) {
      if (button1 == 0) { //down
        myDFPlayer.play(1);
        screen_state = RECORD_TRANSITION;
        homeToRecordTimer = millis();
        Serial.println("In Home");
        Serial.println(homeToRecordTimer);
        Serial.println(millis());
        
      } else if (button2 == 0) {
//        else if (button2.update() == 1) {
        screen_state = SELECTION;
        display_selection();
      }
      break;
    } case RECORD_TRANSITION: {
      Serial.println("In record transition");
      Serial.println(homeToRecordTimer);
      Serial.println(millis());
      if (millis()-homeToRecordTimer>=homeToRecordDelay){
        screen_state = RECORD;
        display_record();
        sample_timer = millis();
      }
      break;
    } case RECORD: {
//      Serial.println("In Record");
//      Serial.println(myDFPlayer.available());
      if (millis() - sample_timer > sample_period) {
        combinationReporterFSM(button1, button2, button3);
        sample_timer = millis();
      }
      if (myDFPlayer.available() && (millis()-songStartTime)>DFPPlayerResetTime) {
        if (myDFPlayer.readType()==DFPlayerPlayFinished) {
          myDFPlayer.read();
          screen_state = SENDNOTELIST;
          myDFPlayer.reset();
        }
     } 
      break;
    } case SELECTION: {
      if (button3 == 0) {
//      if (button3.update() == 1) {
        resetInitialConditions();
        getNoteString("SomeSong"); 
        strcpy(recordedString, response_buffer);
        parseNotesToArrays(recordedString, times, notes);
        myDFPlayer.play(1);
        screen_state = PLAY_TRANSITION;
        selectionToPlayTimer = millis();
      }
      break;
    } case PLAY_TRANSITION: {
      
      if (millis()-selectionToPlayTimer>=selectionToPlayDelay){
        display_play();
        screen_state = PLAY;
        play_timer = millis();
        allNotesIndex = 0;
        timesIndex = 0;
      }
      break;
    } case PLAY: {
//      Serial.println(timesIndex);
//      Serial.println("In Play");
//      Serial.println(myDFPlayer.available());
      if (myDFPlayer.available() && (millis()-play_timer)>DFPPlayerResetTime) {
        if (myDFPlayer.readType()==DFPlayerPlayFinished) {
          myDFPlayer.read();
          screen_state = SCORES;
          display_scores();
          myDFPlayer.reset();
        }
       } else {
         if (millis()-play_timer > 40) {
          display_notes();
          //Leaving these for now because I don't know what these do
//          buttonOnePlayFSM(button1);
//          buttonTwoPlayFSM(button2);
//          buttonThreePlayFSM(button3);
        }
        if (millis() - sample_timer > sample_period) {
          scoreReporterFSM(button1, button2, button3);
          sample_timer = millis();
        }
       }
      

      break;
    } case SENDNOTELIST: {
      postNoteString(notesString);
      screen_state = HOME; //Add later: transition to new state whether successfully updated db
      display_home();
      break;
    } case SENDANDGETSCORE: {
      //posting score
      //retrieve high score
      break;
    } case SCORES: {
      if (button1 == 0) {
//      if (button1.update() == 1) {
        myDFPlayer.play(1);
        screen_state = PLAY_TRANSITION;
        selectionToPlayTimer = millis();
        allNotesIndex = 0;
        timesIndex = 0;
        memset(allNotes, 0, sizeof(allNotes));
      } else if (button3 == 0) {
//      else if (button3.update() == 1) {
        screen_state = HOME;
        display_home();
        selectionToPlayTimer = millis(); // rippu name but maybe keep it the same...
      }
      break;
    }
  }
  
  while (millis() - primary_timer < DT); //wait for primary timer to increment
  primary_timer = millis();
}

void display_name() {
  tft.fillScreen(TFT_BLACK); //fill background
  tft.setCursor(0,0,1); // set the cursor
  tft.println("Enter your name. Tilt to scroll, press 1 to lock in letter and 3 to enter name.");
  tft.println();
  tft.println(name_str);
}

void display_home() {
  tft.fillScreen(TFT_BLACK); //fill background
  tft.setCursor(0,0,1); // set the cursor
  tft.println("Welcome to SEAL!");
  tft.print("Press 1 to record, 2 to select a track.");
}

void display_selection() {
  tft.fillScreen(TFT_BLACK); //fill background
  tft.setCursor(0,0,1); // set the cursor
  tft.println("Select a track:");
  tft.print("Press 1 to scroll up, 2 to scroll down, and 3 to select.");
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
//  tft.fillScreen(TFT_BLACK); //fill background
//  tft.setCursor(0,0,1); // set the cursor
//  tft.print("playing game!");
}

void display_scores() {
  tft.fillScreen(TFT_BLACK); //fill background
  tft.setCursor(0,0,1); // set the cursor
  tft.print("Your score: ");
  tft.println(score);
  tft.print("High score: ");
  tft.println(100);
  tft.println("Press 1 to play same song again, 3 to return to home!");
}

void combinationReporterFSM(uint8_t button1, uint8_t button2, uint8_t button3){
//void combinationReporterFSM(Button button1, Button button2, Button button3) {
  int buttonOneResult = buttonOneReporterFSM(button1);
  int buttonTwoResult = buttonTwoReporterFSM(button2);
  int buttonThreeResult = buttonThreeReporterFSM(button3);
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
//int buttonOneReporterFSM(Button button1) {
  //your code here
  switch (buttonOneState) {
    case IDLE:
      if (button1 == 0) {
//      if (button1.update() == 1) {
        buttonOneState = DOWN;
        tft.fillEllipse(34, 30, 7, 5, TFT_GREEN);
        buttonRecordTimer = millis();
        return 1;
      }
      return 0;
      break;
    case DOWN:
      if (button1) {
//      if (button1.update() == 0) {
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
//int buttonTwoReporterFSM(Button button2) {
  //your code here
  switch (buttonTwoState) {
    case IDLE:
      if (button2 == 0) {
//      if (button2.update() == 1) {
        buttonTwoState = DOWN;
        tft.fillEllipse(63, 30, 7, 5, TFT_GREEN);
        buttonRecordTimer = millis();
        return 1;
      }
      return 0;
      break;
    case DOWN:
      if (button2) {
//      if (button2.update() == 0) {
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
//int buttonThreeReporterFSM(Button button3) {
  //your code here
  switch (buttonThreeState) {
    case IDLE:
      if (button3 == 0) {
//      if (button3.update() == 1) {
        buttonThreeState = DOWN;
        tft.fillEllipse(93, 30, 7, 5, TFT_GREEN);
        buttonRecordTimer = millis();
        return 1;
      }
      return 0;
      break;
    case DOWN:
      if (button3) {
//      if (button3.update() == 0) {
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

void display_notes() {
  // Serial.println(sizeof(allNotes)/sizeof(allNotes[0])); //In case we want of size of something
  for (int i = 0; i < allNotesIndex; i++) {
    allNotes[i].moveNote();
  }
  tft.drawRect(15,15,100,130,TFT_BLUE); //bottom screen at 145
  tft.drawLine(15,130,115,130, TFT_PINK);
  tft.fillEllipse(34, 130, 7, 5, TFT_CYAN);
  tft.fillEllipse(63, 130, 7, 5, TFT_CYAN);
  tft.fillEllipse(93, 130, 7, 5, TFT_CYAN);
  if ((times[timesIndex]>0) && (times[timesIndex] < (millis() - play_timer))) {
    if (notes[timesIndex] < 3) {
      Note note(&tft, notes[timesIndex], times[timesIndex]);
      allNotes[allNotesIndex] = note;
      allNotesIndex++;
    }
    else if (notes[timesIndex] == 3) {
      Note note1(&tft, 0, times[timesIndex]);
      allNotes[allNotesIndex] = note1;
      allNotesIndex++;
      Note note2(&tft, 1, times[timesIndex]);
      allNotes[allNotesIndex] = note2;
      allNotesIndex++;
    }
    else if (notes[timesIndex] == 4) {
      Note note1(&tft, 0, times[timesIndex]);
      allNotes[allNotesIndex] = note1;
      allNotesIndex++;
      Note note2(&tft, 2, times[timesIndex]);
      allNotes[allNotesIndex] = note2;
      allNotesIndex++;
    }
    else if (notes[timesIndex] == 5) {
      Note note1(&tft, 1, times[timesIndex]);
      allNotes[allNotesIndex] = note1;
      allNotesIndex++;
      Note note2(&tft, 2, times[timesIndex]);
      allNotes[allNotesIndex] = note2;
      allNotesIndex++;
    }
    else if (notes[timesIndex] == 6) {
      Note note1(&tft, 0, times[timesIndex]);
      allNotes[allNotesIndex] = note1;
      allNotesIndex++;
      Note note2(&tft, 1, times[timesIndex]);
      allNotes[allNotesIndex] = note2;
      allNotesIndex++;
      Note note3(&tft, 2, times[timesIndex]);
      allNotes[allNotesIndex] = note3;
      allNotesIndex++;
    }
    timesIndex += 1;
  }
}

void scoreReporterFSM(uint8_t button1, uint8_t button2, uint8_t button3){
//void scoreReporterFSM(Button button1, Button button2, Button button3) {
  int score_buffer = 1000;
  int buttonOneResult = buttonOnePlayFSM(button1);
  int buttonTwoResult = buttonTwoPlayFSM(button2);
  int buttonThreeResult = buttonThreePlayFSM(button3);
  int currentNote = notes[scoreIndex];
  int currentNoteTime = times[scoreIndex] + 1040;
  Serial.println("one score reporter loop");
  Serial.println(scoreIndex);
  Serial.println(currentNote);
  Serial.println(currentNoteTime);
  Serial.println((millis() - play_timer) - 500);
  Serial.println((millis() - play_timer) + 500);

  if (buttonOneResult == 1) {
    Serial.println("HERE");
    Serial.println(millis());
    Serial.println(play_timer);
    Serial.println(millis() - play_timer);
  } 
  
  if (buttonTwoResult == 1) {
    Serial.println("HERE");
    Serial.println(millis());
    Serial.println(play_timer);
    Serial.println(millis() - play_timer);
  }
//  Serial.println(buttonOneResult);
//  Serial.println(buttonTwoResult);
//  Serial.println(buttonThreeResult);
  if (currentNote == 0) {
    if (buttonOneResult == 1 && buttonTwoResult == 0 && buttonThreeResult == 0 && currentNoteTime > (millis() - play_timer) - score_buffer && currentNoteTime < (millis() - play_timer) + score_buffer) {
      score += 1;
      scoreIndex += 1;
    }
    else if (currentNoteTime + score_buffer < (millis() - play_timer)) {
      scoreIndex += 1;
    }
  }
  if (currentNote == 1) {
    if (buttonOneResult == 0 && buttonTwoResult == 1 && buttonThreeResult == 0 && currentNoteTime > (millis() - play_timer) - score_buffer && currentNoteTime < (millis() - play_timer) + score_buffer) {
      score += 1;
      scoreIndex += 1;
    }
    else if (currentNoteTime + score_buffer < (millis() - play_timer)) {
      scoreIndex += 1;
    }
  }
  if (currentNote == 2) {
    if (buttonOneResult == 0 && buttonTwoResult == 0 && buttonThreeResult == 1 && currentNoteTime > (millis() - play_timer) - score_buffer && currentNoteTime < (millis() - play_timer) + score_buffer) {
      score += 1;
      scoreIndex += 1;
    }
    else if (currentNoteTime + score_buffer < (millis() - play_timer)) {
      scoreIndex += 1;
    }
  }
  if (currentNote == 3) {
    if (buttonOneResult == 1 && buttonTwoResult == 1 && buttonThreeResult == 0 && currentNoteTime > (millis() - play_timer) - score_buffer && currentNoteTime < (millis() - play_timer) + score_buffer) {
      score += 1;
      scoreIndex += 1;
    }
    else if (currentNoteTime + score_buffer < (millis() - play_timer)) {
      scoreIndex += 1;
    }
  }
  if (currentNote == 4) {
    if (buttonOneResult == 1 && buttonTwoResult == 0 && buttonThreeResult == 1 && currentNoteTime > (millis() - play_timer) - score_buffer && currentNoteTime < (millis() - play_timer) + score_buffer) {
      score += 1;
      scoreIndex += 1;
    }
    else if (currentNoteTime + score_buffer < (millis() - play_timer)) {
      scoreIndex += 1;
    }
  }
  if (currentNote == 5) {
    if (buttonOneResult == 0 && buttonTwoResult == 1 && buttonThreeResult == 1 && currentNoteTime > (millis() - play_timer) - score_buffer && currentNoteTime < (millis() - play_timer) + score_buffer) {
      score += 1;
      scoreIndex += 1;
    }
    else if (currentNoteTime + score_buffer < (millis() - play_timer)) {
      scoreIndex += 1;
    }
  }
  if (currentNote == 6) {
    if (buttonOneResult == 1 && buttonTwoResult == 1 && buttonThreeResult == 1 && currentNoteTime > (millis() - play_timer) - score_buffer && currentNoteTime < (millis() - play_timer) + score_buffer) {
      score += 1;
      scoreIndex += 1;
    }
    else if (currentNoteTime + score_buffer < (millis() - play_timer)) {
      scoreIndex += 1;
    }
  }
  Serial.println(score);
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

void resetInitialConditions(){
  memset(notesString, 0 , sizeof(notesString));
  memset(allNotes, 0, sizeof(allNotes));
  memset(times, 0, sizeof(times));
  memset(notes, 0, sizeof(notes));
  memset(recordedString, 0, sizeof(recordedString));
  allNotesIndex = 0;
  timesIndex = 0;
  scoreIndex = 0;
  score = 0;
}


  
