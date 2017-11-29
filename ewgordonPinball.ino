#include <BallyLib.h>


#define CR_ROW   0
#define CR_COL   5
#define COIN_ROW 1
#define COIN_COL 0
#define PLAYER_UP_ROW 14
#define NUM_PLAYERS_ROW 13
Bally bally;

int activePlayer = 0;
int LMH[3] = {2,8,4} ; // {8,4,12} ; 

void setup() {
  Serial.begin(9600);
  //displays, set any lamps that should always be on
}

void loop() {
  bool gameStarted = false;
  int numPlayers = 0, credits = 0;
  long int scores[4] = {0};
  //int scoreFound = waitPlayers(numPlayers, credits);//accept players until game starts
  //scores[0] = scoreFound;
  //init S/W state: scores, player number, ball number, drop target 
  //counters, any other game and/or ball state variables
  //init H/W, such as game over lamp
  int r = waitPlayers(numPlayers, credits);
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


bool creditReleased()
{
  static bool values[3] = {false};
  for(int i = 0; i < 3; i++)
  {
    values[0] = values[1];
    values[1] = values[2];
    values[2] = bally.getCabSwitch(1, 0);
    if(!values[0] && values[1] && values[2])
     {
      return true;
     }
  }
  return false; 
}

bool addPlayerReleased()
{
  static bool values[3] = {false};
  for(int i = 0; i < 3; i++)
  {
    values[0] = values[1];
    values[1] = values[2];
    values[2] = bally.getCabSwitch(0, 5);
    if(!values[0] && values[1] && values[2])
     {
      return true;
     }
  }
  return false; 
}

//return score of element ball hit?
int waitPlayers(int& numPlayers, int& credits)
{
  while(numPlayers < 4/*wait for game start condition*/){
    bool result = creditReleased();
    if(result){
      if(credits < 4){//are more than 4 credits allowed?
        credits++;
        bally.setDisplay(5, 0, (char)credits);//this last argument might need to be converted to BCD
      }
    }
    result = addPlayerReleased();
    if(result){
      if(credits > 0 && numPlayers < 4){
        switchPlayer(0);//set active player 0
        lightLamp(NUM_PLAYERS_ROW, numPlayers);
        credits--;
        numPlayers++;
        bally.fireSolenoid(6, true);//eject ball
      }
    }
  }
}

void lightLamp(int row, int col)
{
  for(int i = 0; i < 4; ++i)
  {
    int a = false;
    if(i == col){a = true;}
    bally.setLamp( row, i, a);
  }
}

void switchPlayer(const int& playerIndex)
{
  //switch activePlayer logic
  if(0 <= playerIndex && playerIndex <= 3)
  {
    activePlayer = playerIndex;
    Serial.println("TODO switch active player score");
    //bally.setDisplay( , );//set score display
    lightLamp(PLAYER_UP_ROW, activePlayer);//set player number display
  }
}

void detectBallAction(int& activePlayer, long int scores[])//will need to change player number if ball falls in hole
{
  //scan all switches in machine for ball action,
}

void setScore(int dispNumber, long int score)
{
  long int scoreCpy = score;
  for(int i = 0; i < N_DIGITS; ++i)
    {
      int temp = removeSmallestDigit(score);
      if(temp == 0)
      {
        temp = -1;
      }
      Serial.println("TODO set display code here");
      //bally.setDisplay( , );
    } 
  //make leading zeros -1?
}

int removeSmallestDigit(long int& score)//takes score splits off leading bit from original score
{
  int result = (int)(score % 10);
  score = score / 10;
  return result;
}




