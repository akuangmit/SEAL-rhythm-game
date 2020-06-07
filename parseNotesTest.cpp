#include <iostream>
using namespace std;

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

int main(){
    int times[5];
    int notes[5];
    // int times[4] = {1000,2000,4000,8000}; //recorded times
    // int notes[4] = {1, 5, 6, 0}; //recorded combos
    char recordedString[50] = "1000:1, 2000:5, 4000:6, 8000:0,";
    parseNotesToArrays(recordedString, times, notes);
    for (int i = 0; i<4; i++){
        cout << "in for loop" << endl;
        cout << "Time" << endl;
        cout << times[i] << endl;
        cout << "Note" << endl;
        cout << notes[i] << endl;
    }
}