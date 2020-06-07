void parseNotesToArrays(char *recordedString, int *times/*Name of start time array*/, int* notes/*Name of note permutation array*/){
  // Global string of all notes: “time:permutation,time:permutation,time:permutation,...”
  char* pch = strtok(recordedString, ":");
  // cout << "first" << endl;
  // cout << pch << endl;
  int index = 0;
  while (pch!=NULL){
    // cout << index << endl;
    //pch currently points to "time"
    int noteStartTime = atoi(pch);
    times[index] = noteStartTime;
    pch = strtok(NULL, ",");
    notes[index] = atoi(pch);
    // cout << "after" << endl;
    index+=1;
    pch = strtok(NULL, ":");
    // cout << pch << endl;

  }
}
