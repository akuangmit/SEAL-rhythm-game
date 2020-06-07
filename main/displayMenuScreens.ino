void displayRecordSelect(int currentCursorIndex, int numberOfSongIndices){
  tft.fillScreen(TFT_BLACK); //fill background
  tft.setCursor(0,0,1); // set the cursor
  int referenceIndex = currentCursorIndex;
  tft.println("Choose your song to record! Press 1 to go to previous page, press 2 to go to next page, press 3 to confirm");
  tft.println("");
  tft.print("Song: ");
  tft.println(songMap[referenceIndex]);
  tft.print("Artist: ");
  tft.println(authorMap[referenceIndex]);
  tft.println("");
  tft.println(descriptionMap[referenceIndex]);
  tft.println("");
  tft.print("Page ");
  int currentPage = currentCursorIndex+1;
  int numPages = numberOfSongIndices;
  char temp [10];
  sprintf(temp, "%d/%d", currentPage, numPages);
  tft.print(temp);
}






//// The screen of record select
//// The first screen of play select
////button press logic handled outside in record or select respectively
//// int currentCursorIndex = 0;
//  // if button 2, displaySong
//  //    // char[][] allSongs = ......//can create outer most size to be the number of songindices (pass this in)
//   // char[][] allArtists = .....
void displaySong(int currentCursorIndex, int numberOfSongIndices){
   //Before calling this function, we need to do a db query for all the songindices
  tft.fillScreen(TFT_BLACK); //fill background
  tft.setCursor(0,0,1); // set the cursor
  int referenceIndex = retrievedSongIndices[currentCursorIndex];
//  Serial.println("SONG");
//  Serial.println(songMap[referenceIndex]);
//  Serial.println("AUTHOR");
//  Serial.println(authorMap[referenceIndex]);
//  Serial.println("CursorIndex");
//  Serial.println(currentCursorIndex);
//  Serial.println("Ref index");
//  Serial.println(referenceIndex);
  tft.println("Choose your song to play! Press 1 to go to previous page, press 2 to go to next page, press 3 to confirm");
  tft.println("");
  tft.print("Song: ");
  tft.println(songMap[referenceIndex]);
  tft.print("Artist: ");
  tft.println(authorMap[referenceIndex]);
  tft.println("");
  tft.println(descriptionMap[referenceIndex]);
  tft.println("");
  tft.print("Page ");
  int currentPage = currentCursorIndex+1;
  int numPages = numberOfSongIndices;
  char temp [10];
  sprintf(temp, "%d/%d", currentPage, numPages);
  tft.print(temp);
}
//
//// Display all users who recorded a particular song
//void displayUsersWhoRecordedTheSong
//
//
//
////Song Name
////by
////Song Artist
////
////1/5  Press 3
//
//
////
////Recorded by 
////User Who Recorded
////High Score, User Who Got that High score
////1/5 Press 3
////
////Play mode

void displayUser(int currentUserIndex, int numberOfSongIndices, char* response_buffer){
  tft.fillScreen(TFT_BLACK); //fill background
  tft.setCursor(0,0,1); // set the cursor
  tft.println("Select the name of the user who recorded the song! Press 1 to go to previous page, press 2 to go to next page, press 3 to confirm");
  tft.println("");
  tft.print("Recorded by: ");
  tft.println(retrievedNames[currentUserIndex]);
  tft.println("");
  tft.print("Global high score: ");
  tft.println(response_buffer);
  tft.println("");
  tft.print("Page ");
  int currentPage = currentUserIndex+1;
  int numPages = numberOfSongIndices;
  char temp [10];
  sprintf(temp, "%d/%d", currentPage, numPages);
  tft.print(temp);
}
