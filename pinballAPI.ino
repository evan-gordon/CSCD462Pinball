#include <BallyLib.h>

#define CR_ROW   0
#define CR_COL   5
#define COIN_ROW 1
#define COIN_COL 0

Bally bally;

int LMH[3] = {2,8,4} ; // {8,4,12} ; 
int numPlayers = 0, credits = 0;

//return score of element ball hit?
int waitPlayers()
{
 int numPlayers = 0, credits = 0; 
  int button = 0;
  while(numPlayers < 4/*wait for game start condition*/)
    button = bally.waitForTestCreditCoun(/*CR_ROW, CR_COL, COIN_ROW, COIN_COL*/);
    if(button == CREDIT){
      if(credits == 4){//are more than 4 credits allowed?
        credits++;
        setDisplay(5, 0, credits);//this last argument might need to be converted to BCD
      }
      button = 0;
    }
    if(button == COIN){
      if(credits > 0 && numPlayers < 4){
        credits--;
        numPlayers++;
        button = 0;
        //eject ball
      }
    }
  }
}

