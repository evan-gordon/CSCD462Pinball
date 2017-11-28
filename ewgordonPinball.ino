#include <BallyLib.h>
//audio:
/*
# Trigger mode (0..3)
  #  0:Level triggered. Repeats. Responds to code change immediately.
  #  1:Level triggered (sustained).  Repeats. Always plays to end of file.
  #  2:Edge triggered.  Plays once. Ignores code changes during playback.
  #  3:Edge triggered (retriggerable).  Plays once.  Responds to code change.
*/
void setup() {
  Serial.begin(9600);
  //displays, set any lamps that should always be on
}

void loop() {
  bool gameStarted = false;
  numPlayers = 0;
  long int scores[4] = {0};
  int scoreFound = waitPlayers();//accept players until game starts
  scores[0] = scoreFound;
  //init S/W state: scores, player number, ball number, drop target 
  //counters, any other game and/or ball state variables
  //init H/W, such as game over lamp
  int button = bally.waitForTestCreditCoun(CR_ROW, CR_COL, COIN_ROW, COIN_COL);
  if(button == CREDIT){
    gameOver = false;
    //turn off game over, indicate #players=1, set player1 score to zero
    //loop for each player and ball (3 balls per player per game)
      //zero the switch memory so don’t retain sticky hits from before
      //init any S/W and H/W state that should reset on each ball
      //light current player up and display the ball number
      //fire the outhole solenoid to eject a ball
      //loop, reading each playfield switch
        //for each switch hit, take appropriate action (add player,
        //fire solenoid, add points, play chime, arm bonus, etc.)
      //until the outlane switch is read
      //advance current player and/or ball number
    //until each player has played 3 balls
    //check for high score (optional)
    //perform random score match (optional), fire knocker on match
  }

}
