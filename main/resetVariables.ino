void resetVariables(){
  memset(globalUserAndScore, 0, sizeof(globalUserAndScore));
  score = 0;
  scoreIndex = 0;
  allNotesIndex = 0;
  timesIndex = 0;
  myDFPlayer.reset();
  currentCursorIndex = 0;
  counter = 0;
  currentUserIndex = 0;
  userCounter = 0;
}
