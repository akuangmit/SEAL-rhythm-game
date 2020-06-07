void postScore(char* user, char* songName, int score){
  char body[5000]; //for body;
  char scoreString[5];
//  char bodyString[1100];
  sprintf(body, "user=");
  strcat(body, user);
  strcat(body, "&");
  strcat(body, "song=");
  strcat(body, songName);
  strcat(body, "&");
  strcat(body, "high_score=");
  sprintf(scoreString, "%d", score);
  strcat(body, scoreString);
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
