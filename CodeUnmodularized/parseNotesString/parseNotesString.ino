//Assuming there are global variables accounting for time play started (w/o any offset)
/*
 * Populates passed-in int vectors with note permutations active at a given time
 * @param fillerArray1 int vector holding all start times of currently active notes in-order
 * @param fillerArray2 int vector holding int representation of note permutation of notes currently active in-order
 */
void parseNotesString(const vector<int> &/*Name of start time array*/, const vector<int> &/*Name of note permutation array*/){
  // Global string of all notes: “time:permutation,time:permutation,time:permutation,...”
  char* pch = strtok(notesString, ":");
  while (pch!=NULL){
    //pch currently points to "time"
    boolean isActive = false;
    int noteStartTime = atoi(pch);
    int curr_t = millis() - start_t;
    //Start time within active range
    if (noteStartTime - curr_t >= 0 &&
        noteStartTime - curr_t < /*some_calculated_value (global, not passed in)*/) {
          //Double-checking this to see if correct or other function needed
          /*Name of start time array*/.push_back(noteStartTime);
          isActive = true;
        }
    //Now points to note permutation 
    pch = strtok(NULL, ":");
    //Adds permutation to active-tracking data structure
    if (isActive) {
      /*Name of note permutation array*/.push_back(atoi(pch));
    }
    //Points to next time in the String
    pch = strtok(NULL, ",");
  }
}

/*
 * 
 * 
 * int array version
 * 
 * 
 */

/*
 * Populates passed-in int arrays with note permutations active at a given time
 * @param fillerArray1 int array holding all start times of currently active notes in-order
 * @param fillerArray2 int array holding int representation of note permutation of notes currently active in-order
 */
void parseNotesString(int*/*Name of start time array*/, int*/*Name of note permutation array*/){
  // Global string of all notes: “time:permutation,time:permutation,time:permutation,...”
  char* pch = strtok(notesString, ":");
  int idxCounter = 0;
  while (pch!=NULL){
    //pch currently points to "time"
    boolean isActive = false;
    int noteStartTime = atoi(pch);
    int curr_t = millis() - start_t;
    //Start time within active range
    if (noteStartTime - curr_t >= 0 &&
        noteStartTime - curr_t < /*some_calculated_value (global, not passed in)*/) {
          /*Name of start time array*/[idxCounter] = noteStartTime;
          isActive = true;
        }
    //Now points to note permutation 
    pch = strtok(NULL, ":");
    //Adds permutation to active-tracking data structure
    if (isActive) {
      /*Name of note permutation array*/[idxCounter] = atoi(pch);
    }
    //Increments index counter
    idxCounter++;
    //Points to next time in the String
    pch = strtok(NULL, ",");
  }
}
