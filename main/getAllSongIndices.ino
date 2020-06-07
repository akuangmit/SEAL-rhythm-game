void getAllSongIndices(){
  sprintf(request_buffer,"GET http://608dev.net/sandbox/sc/akuang/final_project/songs_db.py/?song_index=-1 HTTP/1.1\r\n");
  strcat(request_buffer,"Host: 608dev.net\r\n"); //add more to the end
  strcat(request_buffer,"\r\n"); //add blank line!

  do_http_request("608dev.net", request_buffer, response_buffer, OUT_BUFFER_SIZE, RESPONSE_TIMEOUT,true);
  // Serial.println(recordedString); //print to serial monitor
}

void getAllUsersForSpecificSongIndex(uint8_t song_index){
  sprintf(request_buffer,"GET http://608dev.net/sandbox/sc/akuang/final_project/songs_db.py/?song_index=%d HTTP/1.1\r\n", song_index);
  strcat(request_buffer,"Host: 608dev.net\r\n"); //add more to the end
  strcat(request_buffer,"\r\n"); //add blank line!

  do_http_request("608dev.net", request_buffer, response_buffer, OUT_BUFFER_SIZE, RESPONSE_TIMEOUT,true);
}
