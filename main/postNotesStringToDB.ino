void postNoteString(char* notesString, int songIndex){
  Serial.println("posting");
  Serial.println(name_str);
  Serial.println(notesString);
  Serial.println(songIndex);
  char body[5000]; //for body;
//  char bodyString[1100];
  sprintf(body, "name=");
  strcat(body, name_str);
  strcat(body, "&");
  strcat(body, "song_index=");
  char songIndexStr[2];
  sprintf(songIndexStr, "%d", songIndex);
  songIndexStr[1] = '\0';
  strcat(body, songIndexStr);
  strcat(body, "&");
  strcat(body, "note_list=");
  strcat(body, notesString);
  Serial.println("BODY");
  Serial.println(body);
  int body_len = strlen(body); //calculate body length (for header reporting)
  sprintf(request_buffer,"POST http://608dev.net/sandbox/sc/akuang/final_project/songs_db.py HTTP/1.1\r\n");
  strcat(request_buffer,"Host: 608dev.net\r\n");
  strcat(request_buffer,"Content-Type: application/x-www-form-urlencoded\r\n");
  sprintf(request_buffer+strlen(request_buffer),"Content-Length: %d\r\n", body_len); //append string formatted to end of request buffer
  strcat(request_buffer,"\r\n"); //new line from header to body
  strcat(request_buffer,body); //body
  strcat(request_buffer,"\r\n"); //header
  do_http_request("608dev.net", request_buffer, response_buffer, OUT_BUFFER_SIZE, RESPONSE_TIMEOUT,true);
}

void getNoteString(char* username, int song_index){
  sprintf(request_buffer,"GET http://608dev.net/sandbox/sc/akuang/final_project/songs_db.py/?name=%s&song_index=%d HTTP/1.1\r\n", username, song_index);
  strcat(request_buffer,"Host: 608dev.net\r\n"); //add more to the end
  strcat(request_buffer,"\r\n"); //add blank line!

  do_http_request("608dev.net", request_buffer, response_buffer, OUT_BUFFER_SIZE, RESPONSE_TIMEOUT,true);
  // Serial.println(recordedString); //print to serial monitor
}
