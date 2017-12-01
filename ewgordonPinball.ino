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
        while(true)
        {
          readPlayfield();
          if(bally.getSwitch(0,7))
          {
            Serial.println("turn over");
            bally.playSound(12);
            ballLive = false;
            break;
          }
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

//TODO bonus for doing rollover lane sets
void readPlayfield()
{
  static unsigned char targetRow = 0;
  long int addScore = 0;
  //Serial.println("reading playfield");
  //read each playfield switch
  //for each switch hit, take appropriate action (add player,
  //fire solenoid, add points, play chime, arm bonus, etc.)
  
  if(bally.getRedge(4, 2) || bally.getRedge(4, 3))
  {
    Serial.println("ball ring");
    bally.playSound(12);
    addScore += 20;
  }
  else if(bally.getRedge(3, 0) || bally.getRedge(3, 1))
  {
    Serial.println("left/right feed lane");
    bally.playSound(12);
    addScore += 10;
  }
  else if(bally.getRedge(3, 2) && targetRow == 11111111)
  {
    bool allDown = true;
    Serial.println("drop target rebound hit, checking bits");
    for(int i = 0; i < 8; ++i)
    {
      if(bitRead(targetRow, i) == 0){allDown = false;}
    }
    if(allDown)
    {
      Serial.println("bits valid");
      bally.playSound(12);
      bally.fireSolenoid(3, true, true);
      bally.fireSolenoid(7, true, true); 
      targetRow = 0;
      addScore += 100;
    }
  }
  static int aHit = false;
  static int bHit = false;
  static int topA = false;
  static int topB = false;
  if(bally.getRedge(3, 3))
  {
    Serial.println("lane B");
    bally.playSound(12);
    bHit = true;
    addScore += 20;
  }
  else if(bally.getRedge(3, 4))
  {
    Serial.println("lane A");
    bally.playSound(12);
    aHit = true;
    addScore += 20;
  }
  else if(bally.getRedge(3, 5))
  {
    Serial.println("top B");
    bally.playSound(12);
    topB = true;
    addScore += 20;
  }
  else if(bally.getRedge(3, 6))
  {
    Serial.println("top A");
    bally.playSound(12);
    topA = true;
    addScore += 20;
  }
  else if(bally.getRedge(3, 7))
  {
    Serial.println("Top Center Kick");
    bally.playSound(13);
    bally.fireSolenoid(0, false);
    addScore += 300;
  }
  else if(bally.getRedge(4, 0) || bally.getRedge(4, 1))
  {
    Serial.println("Out lane");
    bally.playSound(12);
    addScore += 10;
  }
  else if(bally.getRedge(4, 2))
  {
    Serial.println("Right Slingshot");
    bally.playSound(12);
    bally.fireSolenoid(11, false);
    addScore += 10;
  }
  else if(bally.getRedge(4, 3))
  {
    Serial.println("Left Slingshot");
    bally.playSound(12);
    bally.fireSolenoid(13, false);
    addScore += 10;
  }
  else if(bally.getRedge(4, 4))
  {
    Serial.println("Bottom Right Pop Bumper");
    bally.playSound(12);
    bally.fireSolenoid(5, false);
    addScore += 10;
  }
  else if(bally.getRedge(4, 5))
  {
    Serial.println("Bottom Left Pop Bumper");
    bally.playSound(12);
    bally.fireSolenoid(1, false);
    addScore += 10;
  }
  else if(bally.getRedge(4, 6))
  {
    Serial.println("Top Right Pop Bumper");
    bally.playSound(12);
    bally.fireSolenoid(9, false);
    addScore += 10;
  }
  else if(bally.getRedge(4, 7))
  {
    Serial.println("Top Left Pop Bumper");
    bally.playSound(12);
    bally.fireSolenoid(1, false);
    addScore += 10;
  }//*/
  
  unsigned char currTarget = bally.getRedgeRow(2);
  if(currTarget > 0)
  {
    Serial.println("target hit");
    bally.playSound(12);
    targetRow = targetRow | currTarget;    
    addScore += 10;
  }
  //Apply Bonuses
  if(aHit && bHit)
  {
    bHit = false;
    aHit = false;
    addScore += 200;
  }
  if(topA && topB)
  {
    topA = false;
    topB = false;
    addScore += 200;
  }
  if(addScore > 0)
  {
    ballLive = true;
    bally.setLamp(12, 0, true);;//ball in play
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
  for(int i = 0; i < 4; i++){turnOffDisplay(i);}//set scores back to 0
  activePlayer = -1;
  numPlayers =-1;
}

//==================================================finalized functions==========
void initScreen()
{
  bally.fireSolenoid(3, true, true);//targets reset
  bally.fireSolenoid(7, true, true); //""
  bally.setLamp(12, 2, true);//game over
  bally.setLamp(12, 0, false);//ball in play
  bally.setDisplay(4, 0, 0);//turn/ball num
  bally.setDisplay(4, 1, -1);
  bally.setDisplay(4, 2, -1);
  bally.setDisplay(4, 3, 0);//credits
  bally.setDisplay(4, 4, -1);
  bally.setDisplay(4, 5, -1);
  //for(int i = 0; i < 4; i++){updateScore(i, scores[i]);}
  for(int i = 0; i < 4; ++i){turnOffDisplay(i);}
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
        credits--;
        bally.playSound(6);
        bally.setDisplay(4, 3, credits);
        numPlayers++;
        Serial.print("Add Player ");
        Serial.println(numPlayers);
        lightLamp(NUM_PLAYERS_ROW, numPlayers);
        if(numPlayers == 0)
        {
          switchPlayer(0);//set active player 0
          bally.fireSolenoid(6, false);//eject ball
          bally.setContSolenoid(2, false);
          delay(20);
        }
        else{updateScore(numPlayers, scores[numPlayers]);}
      }
    }
    if(numPlayers >= 0){readPlayfield();}//cant start game until there is a player
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
  int temp[7] = {0};
  for(int i = 0; i < N_DIGITS; ++i)
    {
      temp[i] = removeSmallestDigit(scoreCpy);
      /*if(!bitFound)
      {
        if(temp != 0){bitFound = true;}
        else{temp = -1;}
      }//*/
      //bally.setDisplay(dispNumber, i, temp);
    } 
  bool bitFound = false;
  int counter = N_DIGITS - 1;
  while(true)//turn off leading digits
  {
      if(!bitFound && temp[counter] == 0){temp[counter] = -1;}
      else{bitFound = true;}
      counter--;
      bally.setDisplay(dispNumber, counter, temp[counter]);
      if(counter < 0){break;}
  }
}

void turnOffDisplay(int dispNumber)
{
  for(int i = 0; i < N_DIGITS; ++i)
  {
    bally.setDisplay(dispNumber, i, 11);
  }
}

int removeSmallestDigit(long int& score)//takes score splits off leading bit from original score
{
  long int result = (long int)(score % 10);
  score = score / 10;
  return (int)result;
}
