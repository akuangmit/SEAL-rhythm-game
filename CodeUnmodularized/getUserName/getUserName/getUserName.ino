#include <WiFi.h> //Connect to WiFi Network
#include <SPI.h>
#include <TFT_eSPI.h>
#include <mpu9255_esp32.h>
#include<math.h>
#include<string.h>

TFT_eSPI tft = TFT_eSPI();
const int SCREEN_HEIGHT = 160;
const int SCREEN_WIDTH = 128;
const int BUTTON_PIN = 5;
const int LOOP_PERIOD = 40;

MPU9255 imu; //imu object called, appropriately, imu
const uint16_t OUT_BUFFER_SIZE = 1000; //size of buffer to hold HTTP response
char old_response[OUT_BUFFER_SIZE]; //char array buffer to hold HTTP request
char response[OUT_BUFFER_SIZE]; //char array buffer to hold HTTP request
unsigned long primary_timer;

int old_val;

//used to get x,y values from IMU accelerometer!
void get_angle(float* x, float* y) {
  imu.readAccelData(imu.accelCount);
  *x = imu.accelCount[0] * imu.aRes;
  *y = imu.accelCount[1] * imu.aRes;
}

class NamePicker {
    char alphabet[50] = " ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    char message[400] = {0}; //contains previous query response
    char query_string[50] = {0};
    int char_index;
    int state;
    unsigned long scrolling_timer;
    const int scrolling_threshold = 150;
    const float angle_threshold = 0.3;
  public:

    NamePicker() {
      state = 0;
      char_index = 0;
      scrolling_timer = millis();
    }
    void update(float angle, int button, char* output) {
      Serial.println("update");
      Serial.println("output");
      Serial.println(output);
      
      if(button == 1) { //short press
          char letter[2] = "";
          letter[0] = alphabet[char_index];
          letter[1] = '\0';
          Serial.println("letter");
          Serial.println(letter);
          strcat(query_string, letter);
          Serial.println("query_string");
          Serial.println(query_string);
          memset(output, 0, sizeof(output)); //reset!!! before strcpy
          strcpy(output, query_string); //?? is this correct??
          char_index = 0;
      } else {
          if (millis()-scrolling_timer >= scrolling_threshold) {
            if (angle < -angle_threshold) { //maybe left?
            char_index = (char_index +36)%37; //total 37 chars in 'alphabet'
            } else if (angle > angle_threshold) { //maybe right?
              char_index = (char_index + 1)%37;
            }
            scrolling_timer = millis();
          }
          
          char letter[2] = "";
          letter[0] = alphabet[char_index];
          letter[1] = '\0';
          char temp[50] = "";
          strcpy(temp, query_string);
          strcat(temp, letter);
          Serial.println("temp");
          Serial.println(temp);
          memset(output, 0, sizeof(output)); //reset!!! before strcpy
          strcpy(output, temp);
        }
     
    }
};

NamePicker np; 

void setup() {
  Serial.begin(115200); //for debugging if needed.
  tft.init();
  tft.setRotation(2);
  tft.setTextSize(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN, TFT_BLACK); //set color of font to green foreground, black background
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  primary_timer = millis();
}

void loop() {
  enter_user_name();
  while (millis() - primary_timer < LOOP_PERIOD); //wait for primary timer to increment
  primary_timer = millis();
}

void enter_user_name(){
  float x, y;
  get_angle(&x, &y); //get angle values
  uint8_t bv = digitalRead(BUTTON_PIN); //get button 1 value
  
  np.update(-y, bv, response); 

  
  if (strcmp(response, old_response) != 0) {//only draw if changed!
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(0, 0, 1);
    tft.println(response);
  }
  memset(old_response, 0, sizeof(old_response));
  strcat(old_response, response);
}
