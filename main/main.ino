#include<string.h>
#include <WiFi.h> //Connect to WiFi Network
#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip
#include <SPI.h>
#include <mpu9255_esp32.h>
#include "Button.h"
#include "NameGetter.h"
#include "Note.h"
TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h

//19, 5, 16 in the actual board
const int input_pin_1 = 5;
const int input_pin_2 = 16;
const int input_pin_3 = 19; //this pin is maybe buggy?
int freq = 2000;
int PWNchannel = 0;
int resolution = 8;
//Doesn't work: 6, 17
#include <Arduino.h>
#include <DFRobotDFPlayerMini.h>
HardwareSerial mySoftwareSerial(2);
DFRobotDFPlayerMini myDFPlayer;
MPU9255 imu; 
void printDetail(uint8_t type, int value);

//For some reason others' codes are not working :((
//
//char network[] = "MIT";  //SSID 
//char password[] = ""; //Password
char network[] = "6s08";  //SSID 
char password[] = "iesc6s08"; //Password
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
uint8_t songIndex;
uint8_t userIndex;
char songAuthor[100];
char songMap[20][100] = {"The Chaser", "What Makes You Beautiful", "Nice Intro", "Old Town Road", "Pinball Song", "Baby Shark Dudududududu", "Bopit!", "Talk", "Careless Whisper"};
char authorMap[20][100] = {"INFINITE", "One Direction", "Hotel Elevators", "Billy Ray Cyrus" ,"Your Local Arcade", "6.08 Meme Squad", "some TV producer I guess", "Khalid", "George Michael"};
char descriptionMap[20][100] = {"Super angsty song with synths", "Classic masterpiece", "Bland, but I guess if you're into that..", "not really sure what's going on here tbh but it kind of sounds like country music", "one two three four five, six seven eight nine ten, eleven twelve", "I love this song!!!!!", "This is a commercial that is only a song by a very specific stretch of the imagination.", "Cool tracks, nice beat", "Get your fill of sax today!!!"};
int retrievedSongIndices[20]; //currentCursorIncrementsOneHere. Then uses the index here to index into songMap and authorMap
char retrievedNames[20][100];

int numSongs = 9; //change to reflect actual number of songs
int currentCursorIndex;
int counter = 0; //number of retrieved songIndices in songs database
int currentUserIndex;
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
#define RECORDCONFIRMATION 12
#define USERSELECT 13

const int NUM_NOTES = 1000;
int times[NUM_NOTES] = {-1}; 
int old_times[NUM_NOTES] = {-1};
int notes[NUM_NOTES] = {-1};
const int FALL_OFFSET = 1040;
char recordedString[OUT_BUFFER_SIZE]; //potentially lower
char globalUserAndScore[OUT_BUFFER_SIZE]; //potentially lower
int timesIndex = 0;

int score = 0;
int scoreIndex = 0;
int userCounter = 0;

Note allNotes[NUM_NOTES];


//used to get x,y values from IMU accelerometer!
void get_angle(float* x, float* y) {
  imu.readAccelData(imu.accelCount);
  *x = imu.accelCount[0] * imu.aRes;
  //Changed this to negative
  *y = - imu.accelCount[1] * imu.aRes;
}

NameGetter ng;
Button buttonObj1 = Button(input_pin_1);
Button buttonObj2 = Button(input_pin_2);
Button buttonObj3 = Button(input_pin_3);

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
    display_name_header();
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
  Serial.begin(115200);
  ledcSetup(PWNchannel, freq, resolution);
  ledcAttachPin(14, PWNchannel);
}

void parseNotesToArrays(char *recordedString, int *times/*Name of start time array*/, int* notes/*Name of note permutation array*/){
  // Global string of all notes: “time:permutation,time:permutation,time:permutation,...”
  char* pch = strtok(recordedString, ":");
  int index = 0;
  while (pch!=NULL){
    //pch currently points to "time"
    int noteStartTime = atoi(pch);
    if (noteStartTime > 0) {
      times[index] = noteStartTime;
      pch = strtok(NULL, ",");
      int noteCombo = atoi(pch);
//      Serial.println(noteCombo);
      if (noteCombo < 3) {
        notes[index] = atoi(pch);
        index+=1;
//        Serial.println("<3");
      }
      if (noteCombo == 3 || noteCombo == 4 || noteCombo == 6) {
//        Serial.println("note 0");
        notes[index] = 0;
        times[index] = noteStartTime;
        index+=1;
      }
      if (noteCombo == 3 || noteCombo == 5 || noteCombo == 6) {
//        Serial.println("note 1");
        notes[index] = 1;
        times[index] = noteStartTime;
        index+=1;
      }
      if (noteCombo == 4 || noteCombo == 5 || noteCombo == 6) {
//        Serial.println("note 2");
        notes[index] = 2;
        times[index] = noteStartTime;
        index+=1;
      }
    }
    else {
      pch = strtok(NULL, ",");
    }    
//    notes[index] = atoi(pch);
//    index+=1;
    pch = strtok(NULL, ":");
  }
}

void loop(){
  //REPLACE THIS SECTION WITH BUTTONS
  uint8_t button1 = digitalRead(input_pin_1);
  uint8_t button2 = digitalRead(input_pin_2);
  uint8_t button3 = digitalRead(input_pin_3);
//  Serial.println(button3);
  //Serial.println(screen_state);
//  ledcWriteTone(PWNchannel, 2000);
//  
//  for (int dutyCycle = 0; dutyCycle <= 255; dutyCycle=dutyCycle+10){
//  
//    Serial.println(dutyCycle);
//  
//    ledcWrite(PWNchannel, dutyCycle);
//    delay(1000);
//  }
//  
//  ledcWrite(PWNPWNchannel, 125);
//  
//  for (int freq = 255; freq < 10000; freq = freq + 250){
//  
//     Serial.println(freq);
//  
//     ledcWriteTone(PWNchannel, freq);
//     delay(1000);
//  }
  switch (screen_state){
    case NAME : {
      float x, y;
      get_angle(&x, &y);
      int bv = 0;
      if (buttonObj1.update() == 1) {
         bv = 1;
      }
//      if (button1 == 0) { bv = 1;}
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
      
//      if (button3 == 0) { //down
      if (buttonObj3.update() == 1) {  
        screen_state = HOME;
        display_home();
        resetVariables();
      }
      
      break;
    } case HOME: {
      // resetInitialConditions();
      if (buttonObj1.update() == 1) {
//      if (button1 == 0) { //down
        screen_state = RECORD_SELECT;
        getAllSongIndices();
        populateRetrievedIndices(&counter);
        currentCursorIndex = 0;
        displayRecordSelect(currentCursorIndex, numSongs);
//      } else if (button2 == 0) {
      } else if (buttonObj2.update() == 1) {
        getAllSongIndices();
        populateRetrievedIndices(&counter);
        currentCursorIndex = 0;
        displaySong(currentCursorIndex, counter);
        screen_state = SELECTION;
//        display_selection();
      }
      break;
    } 
    case RECORD_SELECT: {
      if (buttonObj1.update() == 1) {
        if (currentCursorIndex>0){
          currentCursorIndex-=1;
        } else if (currentCursorIndex ==0) {
          currentCursorIndex = numSongs-1; //last element
        }
        displayRecordSelect(currentCursorIndex, numSongs);
      } 
      if (buttonObj2.update() == 1){
        if (currentCursorIndex<numSongs-1){ //maxlength
          currentCursorIndex+=1;
        } else if (currentCursorIndex == numSongs-1) {
          currentCursorIndex = 0; //first
        }
        displayRecordSelect(currentCursorIndex, numSongs);
      }
      
      if (buttonObj3.update() == 1) {
//      if (button3==0){
        //songIndex = retrievedSongIndices[currentCursorIndex];
        songIndex = currentCursorIndex;
        Serial.print("currentIndex: ");
        Serial.println(currentCursorIndex);
        myDFPlayer.play(currentCursorIndex+1);// guessed (is this how we do it maybe)
        //myDFPlayer.play(1);
        screen_state = RECORD_TRANSITION;
        homeToRecordTimer = millis();
        Serial.println("In Home");
        Serial.println(homeToRecordTimer);
        Serial.println(millis());
      } 
      break;
    }
    case RECORD_TRANSITION: { //later: add a record select
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
        //Change funct call w/ button obj arguments
//        Serial.println("Entering combination reporter FSM");
        combinationReporterFSM(button1, button2, button3);
        sample_timer = millis();
      }
      if (myDFPlayer.available() && (millis()-songStartTime)>DFPPlayerResetTime) {
        if (myDFPlayer.readType()==DFPlayerPlayFinished) {
          myDFPlayer.read();
          screen_state = SENDNOTELIST;
          myDFPlayer.reset();
          myDFPlayer.volume(16);
        }
     } 
      break;
    } case SELECTION: {
//      Serial.print("counter: ");
//      Serial.println(counter);
      songIndex = 0; //logic to set songIndex
      if (buttonObj1.update() == 1) {
//      if (button3==0){
        if (currentCursorIndex>0){
          currentCursorIndex-=1;
        } else if (currentCursorIndex ==0) {
          currentCursorIndex = counter-1; //last element
        }
        Serial.print("songIndex: ");
        Serial.println(retrievedSongIndices[currentCursorIndex]);
        displaySong(currentCursorIndex, counter);
      } 
      if (buttonObj2.update() == 1){
        if (currentCursorIndex<counter-1){ //maxlength
          currentCursorIndex+=1;
        } else if (currentCursorIndex == counter -1) { //last elem
          currentCursorIndex = 0; //first
        }
        Serial.print("songIndex: ");
        Serial.println(retrievedSongIndices[currentCursorIndex]);
        displaySong(currentCursorIndex, counter);
      }
      
//      if (button3 == 0) {
      if (buttonObj3.update()==1){
        screen_state = USERSELECT;
        songIndex = retrievedSongIndices[currentCursorIndex];
        Serial.print("songIndex: ");
        Serial.println(songIndex);
        getAllUsersForSpecificSongIndex(songIndex);
        populateRetrievedUsers(&userCounter);
        currentUserIndex = 0;
        getGlobalHighScore(songIndex, retrievedNames[currentUserIndex]);
        displayUser(currentUserIndex, userCounter, response_buffer);
      }
//      if (buttonObj3.update() == 1) {
//        // resetInitialConditions();
//        strcpy(songAuthor, "B"); //B
////        songAuthor = "G";
//        getNoteString(songAuthor, songIndex); //can change user to Andy to see
//        strcpy(recordedString, response_buffer);
//        Serial.println(recordedString);
//        parseNotesToArrays(recordedString, times, notes);
//        myDFPlayer.play(1);
//        screen_state = PLAY_TRANSITION;
//        selectionToPlayTimer = millis();
//      }
      break;
    }
    case USERSELECT: {
      
      if (buttonObj1.update() == 1) {
//      if (button3==0){
        if (currentUserIndex>0){
          currentUserIndex-=1;
        } else if (currentUserIndex ==0) {
          currentUserIndex = userCounter-1; //last element
        } 
        getGlobalHighScore(songIndex, retrievedNames[currentUserIndex]);
        displayUser(currentUserIndex, userCounter, response_buffer);
      } 
      if (buttonObj2.update() == 1){
        if (currentUserIndex<userCounter-1){ //maxlength
          currentUserIndex+=1;
        } else if (currentUserIndex == userCounter -1) { //last elem
          currentUserIndex = 0; //first
        }
        getGlobalHighScore(songIndex, retrievedNames[currentUserIndex]);
        displayUser(currentUserIndex, userCounter, response_buffer);
      }
      if (buttonObj3.update() == 1) {
        // resetInitialConditions();
        strcpy(songAuthor, retrievedNames[currentUserIndex]); //populated song author
//        Serial.println("Song Author");
//        Serial.println(songAuthor);
//        Serial.println("SongIndex");
//        Serial.println(songIndex);
//        songAuthor = "G";
        getNoteString(songAuthor, songIndex); //can change user to Andy to see
        strcpy(recordedString, response_buffer);
        Serial.println(recordedString);
        parseNotesToArrays(recordedString, times, notes);
        //strcpy(old_times, times);
        memcpy(old_times, times, sizeof(times));
//        myDFPlayer.play(1);
        Serial.print("songIndex: ");
        Serial.println(songIndex);
        myDFPlayer.play(songIndex+1); //probably?
        screen_state = PLAY_TRANSITION;
        selectionToPlayTimer = millis();
      }
      break;
    }
    
    case PLAY_TRANSITION: {
      
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
          screen_state = SENDANDGETSCORE;
          ledcWriteTone(PWNchannel, -1);
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
          //Change dis later too
          scoreReporterFSM(button1, button2, button3);
          sample_timer = millis();
        }
       }
      

      break;
    } case SENDNOTELIST: {
      postNoteString(notesString, songIndex);
      postScore("overwrite", songIndex, 0, name_str);
      memset(notesString, '\0', sizeof(notesString));
      screen_state = RECORDCONFIRMATION; //Add later: transition to new state whether successfully updated db
      display_record_confirmation(songIndex, name_str);
      break;
    } case RECORDCONFIRMATION: {
      if (buttonObj3.update() == 1) {
//      if (button3==0){
        screen_state = HOME;
        display_home();
        resetVariables();
      }
      break;
    }
    case SENDANDGETSCORE: {
      //posting score
      //retrieve high score
      postScore(name_str, songIndex, score, songAuthor);
      getGlobalHighScore(songIndex, songAuthor);
      strcpy(globalUserAndScore, response_buffer);
      display_scores(globalUserAndScore);
      screen_state = SCORES;
      break;
    } case SCORES: {
//      if (button1 == 0) {
      if (buttonObj1.update() == 1) {
        myDFPlayer.play(1);
        screen_state = PLAY_TRANSITION;
        selectionToPlayTimer = millis();
        allNotesIndex = 0;
        timesIndex = 0;
        scoreIndex = 0;
        score = 0;
        //parseNotesToArrays(recordedString, times, notes);
        //memcpy(old_times, times, sizeof(old_times));
        getNoteString(songAuthor, songIndex); //can change user to Andy to see
        strcpy(recordedString, response_buffer);
        Serial.println(recordedString);
        parseNotesToArrays(recordedString, times, notes);
        memset(allNotes, -1, sizeof(allNotes));
//      } else if (button3 == 0) {
      } else if (buttonObj3.update() == 1) {
        screen_state = HOME;
        display_home();
        selectionToPlayTimer = millis(); // rippu name but maybe keep it the same...

        //Reset to initial conditions
        resetVariables();
      }
      break;
    }
  }
  
  while (millis() - primary_timer < DT); //wait for primary timer to increment
  primary_timer = millis();
}
