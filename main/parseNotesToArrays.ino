//void parseNotesToArrays(char *recordedString, int *times/*Name of start time array*/, int* notes/*Name of note permutation array*/){
//  // Global string of all notes: “time:permutation,time:permutation,time:permutation,...”
//  char* pch = strtok(recordedString, ":");
//  int index = 0;
//  while (pch!=NULL){
//    //pch currently points to "time"
//    int noteStartTime = atoi(pch);
//    times[index] = noteStartTime;
//    pch = strtok(NULL, ",");
//    notes[index] = atoi(pch);
//    index+=1;
//    pch = strtok(NULL, ":");
//  }
//}
