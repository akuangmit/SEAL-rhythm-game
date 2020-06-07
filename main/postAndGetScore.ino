void postScore(char* user, int songIndex, int score, char* songAuthor){
  char body[5000]; //for body;
  char scoreString[5];
//  char bodyString[1100];
  sprintf(body, "user=");
  strcat(body, user);
  strcat(body, "&");
  strcat(body, "song_index=");
  char songIndexStr[2];
  sprintf(songIndexStr, "%d", songIndex);
  songIndexStr[1] = '\0';
  strcat(body, songIndexStr);
  strcat(body, "&");
  strcat(body, "high_score=");
  char highScoreString[3];
  sprintf(highScoreString, "%d", songIndex);
  songIndexStr[2] = '\0';
  sprintf(highScoreString, "%d", score);
  strcat(body, highScoreString);
  strcat(body, "&");
  strcat(body, "song_author=");
  strcat(body, songAuthor);
  Serial.println(body);
  int body_len = strlen(body); //calculate body length (for header reporting)
  sprintf(request_buffer,"POST http://608dev.net/sandbox/sc/akuang/final_project/scores_db.py HTTP/1.1\r\n");
  strcat(request_buffer,"Host: 608dev.net\r\n");
  strcat(request_buffer,"Content-Type: application/x-www-form-urlencoded\r\n");
  sprintf(request_buffer+strlen(request_buffer),"Content-Length: %d\r\n", body_len); //append string formatted to end of request buffer
  strcat(request_buffer,"\r\n"); //new line from header to body
  strcat(request_buffer,body); //body
  strcat(request_buffer,"\r\n"); //header
  do_http_request("608dev.net", request_buffer, response_buffer, OUT_BUFFER_SIZE, RESPONSE_TIMEOUT,true); //returns high score here
}

void getGlobalHighScore(int song_index, char* song_author){
  sprintf(request_buffer,"GET http://608dev.net/sandbox/sc/akuang/final_project/scores_db.py/?song_index=%d&song_author=%s HTTP/1.1\r\n", song_index, song_author);
  strcat(request_buffer,"Host: 608dev.net\r\n"); //add more to the end
  strcat(request_buffer,"\r\n"); //add blank line!

  do_http_request("608dev.net", request_buffer, response_buffer, OUT_BUFFER_SIZE, RESPONSE_TIMEOUT,true);
  // Serial.println(recordedString); //print to serial monitor
}
