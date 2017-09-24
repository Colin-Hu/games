#include <iostream>
#include <stdlib.h>
#include <utility>
#include <vector>
#include <chrono>
#include "mylib.hpp"
using namespace std;

void printboardstate(gamestate currgame)
{
   for (int irow=0; irow<5; irow++)
   {
      for (int icol=0; icol<5; icol++)
      {
         cout << currgame.boardstate(irow,icol) <<  " ";
//         cout << irow << "," << icol << " ";
      }
      cout << "\n";
   }
   cout << "----------" << " Player " << currgame.getturn() << "\n";
}

void entermanualmove(gamestate &currgame)
{
   int row, col;
   cin >> row >> col;
//   cout << "Attempting to apply move to " << row << " " << col << "\n";
   currgame.applymove(row,col);
}

void airand(gamestate &currgame)
{
   vector<pair<int,int> > availmoves;
   availmoves = currgame.currentmoves(currgame.getturn());
   int nummoves = availmoves.size();
   int mymove = rand() % nummoves;
   pair<int,int> chosenmove = availmoves[mymove];
//   for (int i = 0; i < nummoves; i++)
//   {
//      cout << availmoves[i].first << " " << availmoves[i].second << "\n";
//   }
//   cout << "AI Moves " << mymove << " of " << nummoves << " Row Col " << chosenmove.first << " " << chosenmove.second << "\n";
   currgame.applymove(chosenmove.first,chosenmove.second);
}

void aiiterative(gamestate &currgame, double timepermove)
{
   cout << "Thinking... \n";
   chrono::time_point<chrono::system_clock> start, end, lstart;
   chrono::duration<double> elapsed_seconds, total_time;
   mytree decisiontree;
   float opteval, alpha = 0, beta = 25;
   int mymove;
   int addedlevel = 1;
   int currentlevel = 0;
   double lastleveltime, totaltime;
   start = chrono::system_clock::now();
   decisiontree.initialize();

   while(lastleveltime < timepermove/4 && addedlevel)
   {
      cout << "Calculating move " << currentlevel << "\n";
      lstart = chrono::system_clock::now();
//      addedlevel = decisiontree.createtree(currgame, decisiontree,1);
      addedlevel = decisiontree.addlevel(currgame, decisiontree);
      decisiontree.evaluatetree(currgame, decisiontree);
//      opteval = decisiontree.propagateminimax(currgame, decisiontree);
      opteval = decisiontree.propagatealphabeta(alpha, beta, currgame, decisiontree);
      mymove = decisiontree.selectmove(decisiontree, opteval);
      end = chrono::system_clock::now();
      elapsed_seconds = end-lstart;
      lastleveltime = elapsed_seconds.count();
      total_time = end-start;
      totaltime = total_time.count();
      if (addedlevel)
      {
         currentlevel++;
      }
      if (totaltime > timepermove)
      {
         decisiontree.prunetree(decisiontree);
         break;
      }
   }

   vector<pair<int,int> > availmoves;
   availmoves = currgame.currentmoves(currgame.getturn());
   pair<int,int> chosenmove = availmoves[mymove];
   currgame.applymove(chosenmove.first,chosenmove.second);
   end = chrono::system_clock::now();
   elapsed_seconds = end-start;
   cout << "My move " << mymove << " " << opteval << "\n";
   cout << "End Time " << elapsed_seconds.count() << "\n";
}

int main()
{
//   cout << "hello world\n";
   gamestate game1;
   game1.reset();
   printboardstate(game1);
//   game1.applymove(2,2);
//   printboardstate(game1);
   while (!game1.currentmoves(game1.getturn()).empty())
   {
      if (game1.getturn() == 1)
//      if (true)
      {
         entermanualmove(game1);
//         airand(game1);
      }
      else
      {
         //airand(game1);
         aiiterative(game1,10.0);
      }
      printboardstate(game1);
//      cout << "Possible moves: " << game1.currentmoves().size() << "\n";
   }
   cout << "Game Over: Player " << game1.getturn() << " loses\n";
}
