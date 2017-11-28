#include <BallyLib.h>

#define CR_ROW   0
#define CR_COL   5
#define COIN_ROW 1
#define COIN_COL 0

Bally bally;

int LMH[3] = {2,8,4} ; // {8,4,12} ; 
int numPlayers = 0, credits = 0;

bool creditReleased()
{
  static bool prev = false;
  bool released = false;
  bool curr = getRedge(CR_ROW, CR_COL);
  if(prev || curr)//logic for rising edge
  {
    released = true;
  }
  prev = curr;
  return released; 
}

bool addPlayerReleased()
{
  static bool prev = false;
  bool released = false;
  bool curr = getRedge(COIN_ROW, COIN_COL);
  if(prev || curr)
  {
    released = true;
  }
  prev = curr;
  return released; 
}

//return score of element ball hit?
int waitPlayers()
{
 int numPlayers = 0, credits = 0; 
  while(numPlayers < 4/*wait for game start condition*/)
    if(creditReleased() == CREDIT){
      if(credits == 4){//are more than 4 credits allowed?
        credits++;
        setDisplay(5, 0, credits);//this last argument might need to be converted to BCD
      }
    }
    if(addPlayerReleased() == COIN){
      if(credits > 0 && numPlayers < 4){
        credits--;
        numPlayers++;
        Serial.println(numPlayers);
        //eject ball
      }
    }
  }
}


void detectBallAction(int& activePlayer, long int scores[])//will need to change player number if ball falls in hole
{
  //scan all switches in machine for ball action,
}



