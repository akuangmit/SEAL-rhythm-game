void populateRetrievedIndices(int* counterPtr){
  char *pch;
  pch = strtok(response_buffer, " ");
  int retrievedIndex = 0;
  while (pch!=NULL){
    retrievedSongIndices[retrievedIndex] = atoi(pch);
    pch = strtok(NULL, " ");
    retrievedIndex+=1;
  }
  *counterPtr = retrievedIndex;
  retrievedSongIndices[retrievedIndex] = '\0';
}
//char retrievedNames[20][100];
void populateRetrievedUsers(int* userCounterPtr){
  char *pch;
  pch = strtok(response_buffer, " ");
  int index = 0;
  while (pch!=NULL){  
    strcpy(retrievedNames[index], pch);
    Serial.println(index);
    Serial.println(retrievedNames[index]);
    pch = strtok(NULL, " ");
    index+=1;
  }
  Serial.println("Out of while");
  *userCounterPtr = index;
  Serial.println("Before strcpy");
//  strcpy(retrievedNames[index], '\0');
}
