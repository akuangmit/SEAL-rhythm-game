void postNoteString(char* notesString){
  Serial.println(notesString);
  char body[5000]; //for body;
//  char bodyString[1100];
  sprintf(body, "name=");
  strcat(body, "SomeSong");
  strcat(body, "&");
  strcat(body, "song_index=");
  strcat(body, "0");
  strcat(body, "&");
  strcat(body, "note_list=");
  strcat(body, notesString);
  strcat(body, "&");
  strcat(body, "note_length=");
  strcat(body, "200");
  strcat(body, "&");
  strcat(body, "song_length=");
  strcat(body, "10000");
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

void getNoteString(char* songName){
  sprintf(request_buffer,"GET http://608dev.net/sandbox/sc/akuang/final_project/songs_db.py/?name=%s HTTP/1.1\r\n", songName);
  strcat(request_buffer,"Host: 608dev.net\r\n"); //add more to the end
  strcat(request_buffer,"\r\n"); //add blank line!

  do_http_request("608dev.net", request_buffer, response_buffer, OUT_BUFFER_SIZE, RESPONSE_TIMEOUT,true);
  // Serial.println(recordedString); //print to serial monitor
}
