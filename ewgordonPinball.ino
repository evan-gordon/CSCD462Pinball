#include <BallyLib.h>
#define PLAYER_UP_ROW 14
#define NUM_PLAYERS_ROW 13

bool ballLive = false;
long int scores[4] = {0};
int activePlayer = -1;
int LMH[3] = {2,8,4} ; // {8,4,12} ; 
Bally bally;

void setup() {
  Serial.begin(9600);
  //displays, set any lamps that should always be on
  //Serial.println("i");
}

void loop() {
  ballLive = false;
  scores[4] = {0};
  activePlayer = -1;
  int numPlayers = -1, credits = 0, turn = 0;
  initScreen();//init H/W, such as game over lamp
  //init S/W state: scores, player number, ball number, drop target 
  //counters, any other game and/or ball state variables
  //Serial.println("Startup complete");
  waitPlayers(numPlayers, credits);//wait for 4 players || ball in play
  //Serial.println("Stop accepting players");
  bally.setDisplay(4, 0, turn);//set turn/ball
  bally.setLamp(12, 2, false);//game over off
  
      while(turn < 3)//loop for each player and ball (3 balls per player per game)
      {
        bally.fireSolenoid(3, true, true);//targets reset
        bally.fireSolenoid(7, true, true); 
        bally.zeroSwitchMemory();//zero the switch memory so don’t retain sticky hits from before
        //init any S/W and H/W state that should reset on each ball
        //light current player up and display the ball number
        //bally.fireSolenoid(6, false);//fire the outhole solenoid to eject a ball
        bool turnStarted = false;
        while((!ballLive && !turnStarted) ||//turn hasnt started and ball not in play
                (turnStarted && ballLive))//ball in play, turn started
        {
          readPlayfield();
          if(ballLive){turnStarted = true;}
        }
        bally.playSound(20);
        if(activePlayer < numPlayers)//advance current player and/or ball number
        {
          Serial.println("Increment Player");
          activePlayer++;
        }
        else
        {
          Serial.println("New Turn");
          turn++;
          bally.setDisplay(4, 0, turn);
          activePlayer = 0;
        }//until each player has played 3 balls
        switchPlayer(activePlayer);
        bally.fireSolenoid(6, false);//eject ball
      }
      //check for high score (optional)
      //perform random score match (optional), fire knocker on match
      //gameOver(numPlayers);
}

void readPlayfield()
{
  long int addScore = 0;
  Serial.println("reading playfield");
  if(bally.getSwitch(0,7))
  {
    Serial.println("Ball died");
    ballLive = false;
  }
  else if(bally.getRedge(4, 2) || bally.getRedge(4, 3))
  {
    Serial.println("ball ring");
    addScore += 20;
  }
  else if(bally.getRedgeRow(2) > 0)
  {
    Serial.println("target hit");
    static int targetCounter = 0;
    targetCounter++;
    addScore += 10;
    if(targetCounter >= 8)
    {
      bally.fireSolenoid(3, true, true);
      bally.fireSolenoid(7, true, true); 
      targetCounter = 0; 
      addScore += 100;
    }
  }
  //read each playfield switch
  //for each switch hit, take appropriate action (add player,
  //fire solenoid, add points, play chime, arm bonus, etc.)
  if(addScore > 0)
  {
    ballLive = true;
    scores[activePlayer] = scores[activePlayer] + addScore;
    updateScore(activePlayer, scores[activePlayer]);
  }
}

void switchPlayer(const int& playerIndex)//switch activePlayer logic
{
  if(0 <= playerIndex && playerIndex <= 3)
  {
    activePlayer = playerIndex;
    updateScore(activePlayer, scores[activePlayer]);
    lightLamp(PLAYER_UP_ROW, activePlayer);//set player number display
  }
}

void gameOver(int numPlayers)
{
  lightLamp(12, 2);//turn on game over light
  for(int i = 0; i < 4; i++){setScore(i, 0);}//set scores back to 0
  activePlayer = -1;
  numPlayers =-1;
}

//==================================================finalized functions==========
void initScreen()
{
  bally.setLamp(12, 2, true);//game over
  bally.setLamp(12, 0, false);//ball in play
  bally.setDisplay(4, 0, 0);//turn/ball num
  bally.setDisplay(4, 1, -1);
  bally.setDisplay(4, 2, -1);
  bally.setDisplay(4, 3, 0);//credits
  bally.setDisplay(4, 4, -1);
  bally.setDisplay(4, 5, -1);
  for(int i = 0; i < 4; i++){updateScore(i, scores[i]);}

  bally.fireSolenoid(3, true, true);//targets reset
  bally.fireSolenoid(7, true, true); 
  bally.zeroSwitchMemory();
}

void waitPlayers(int& numPlayers, int& credits)
{
  while(numPlayers < 3 && !ballLive){
    bool result = creditReleased();
    if(result){
      if(credits < 4){//are more than 4 credits allowed?
        Serial.println("Add Credit");
        credits++;
        bally.setDisplay(4, 3, credits);
        bally.setDisplay(10, 3, credits);//this last argument might need to be converted to BCD
      }
    }
    result = addPlayerReleased();
    if(result){
      if(credits > 0 && numPlayers < 3){
        Serial.print("Add Player ");
        Serial.println(numPlayers);
        credits--;
        bally.setDisplay(4, 3, credits);
        numPlayers++;
        lightLamp(NUM_PLAYERS_ROW, numPlayers);
        if(numPlayers == 0)
        {
          switchPlayer(0);//set active player 0
          bally.fireSolenoid(6, false);//eject ball
          bally.setContSolenoid(2, false);
          delay(20);
        }
      }
    }
    readPlayfield();
  }
  Serial.println("Game Started");
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

void lightLamp(int row, int col)
{
  Serial.print("Light lamp");
    Serial.println(col);
  for(int i = 0; i < 4; ++i)
  {
    int a = false;
    if(i == col){a = true;}
    bally.setLamp( row, i, a);
  }
}

void updateScore(int dispNumber, int newScore)
{
  Serial.print("setScore ");
  Serial.println(newScore);
  long int scoreCpy = newScore;
  for(int i = 0; i < N_DIGITS; ++i)
    {
      int temp = removeSmallestDigit(scoreCpy);
      if(i != 0 &&temp == 0)
      {
        temp = -1;
      }
      bally.setDisplay(dispNumber, i, temp);
    } 
}

int removeSmallestDigit(long int& score)//takes score splits off leading bit from original score
{
  int result = (int)(score % 10);
  score = score / 10;
  return result;
}
