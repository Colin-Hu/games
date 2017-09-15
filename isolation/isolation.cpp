#include <iostream>
#include <stdlib.h>
#include <utility>
#include <vector>
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

void aiiterative(gamestate &currgame)
{
   mytree decisiontree;
   decisiontree.initialize();
   decisiontree.createtree(currgame, decisiontree,6);
   decisiontree.evaluatetree(currgame, decisiontree);
   float opteval = decisiontree.propagateminimax(currgame, decisiontree);
   int mymove = decisiontree.selectmove(decisiontree, opteval);
   vector<pair<int,int> > availmoves;
   availmoves = currgame.currentmoves(currgame.getturn());
   pair<int,int> chosenmove = availmoves[mymove];
   currgame.applymove(chosenmove.first,chosenmove.second);
   cout << "My move " << mymove << " " << opteval << "\n";
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
//         entermanualmove(game1);
         airand(game1);
      }
      else
      {
         //airand(game1);
         aiiterative(game1);
      }
      printboardstate(game1);
//      cout << "Possible moves: " << game1.currentmoves().size() << "\n";
   }
   cout << "Game Over: Player " << game1.getturn() << " loses\n";
}
