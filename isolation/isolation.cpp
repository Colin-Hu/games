#include <iostream>
#include <stdlib.h>
#include <utility>
#include <vector>
using namespace std;

class gamestate
{
      int turn, pos1, pos2;
      int board[25];
   public:
      void reset();
      int boardstate(int row, int col)
      {
         return board[row*5+col];
      }
      int getturn()
      {
         return turn;
      }
      int applymove(int row, int col)
      {
         int position = row*5+col;
//         if (board[position] == 0)
         if (islegalmove(row,col,turn))
         {
            if (turn == 1)
            {
               board[position] = 1;
               if (pos1 != -1)
               {
                  board[pos1] = 3;
               }
               pos1 = position;
               turn = 2;
            }
            else
            {
               board[position] = 2;
               if (pos2 != -1)
               {
                  board[pos2] = 3;
               }
               pos2 = position;
               turn = 1;
            }
            return 1;
         }
         else
         {
            cout << "Illegal move\n";
            return 0;
         }
      }
      int islegalmove(int row, int col, int player)
      {
         int currentpos, newpos;
         if (player == 1)
         {
            currentpos = pos1;
         }
         else
         {
            currentpos = pos2;
         }
         newpos = row*5+col;
         // check in bounds
         if (row < 0 || row > 4 || col < 0 || col > 4)
         {
            return 0;
         }
//       cout << "Checking legality old pos: " << currentpos << "\n";
         if (currentpos == -1)
         {
   //         cout << "First move legal in any position\n";
         }
         // if in row, col, or diag
         else if (row == currentpos/5)
         {
            for(int icol = currentpos; icol%5 != col; icol = icol + (col-icol%5)/abs(col-icol%5))
            {
//               cout << "Checking col collision " << icol << " " << col << "\n";
               if (board[icol] != 0 && icol != currentpos)
               {
//               cout << "Ran into existing col square\n";
                  return 0;
               }
            }
   //         cout << "Same row as last position\n";
         }
         else if (col == currentpos%5)
         {
            for(int irow = currentpos; irow/5 != row; irow = irow + 5*(row-irow/5)/abs(row-irow/5))
            {
//               cout << "Checking row collision " << irow << " " << row << "\n";
               if (board[irow] != 0 && irow != currentpos)
               {
//               cout << "Ran into existing row square\n";
                  return 0;
               }
            }
   //         cout << "Same col as last position\n";
         }
         else if (abs(currentpos%5-col) == abs(currentpos/5-row))
         {
//            cout << "On diagonal \n";
            for (int idiag = 1; idiag < abs(currentpos%5-col); idiag++)
            {
               int checkrow = (currentpos/5) + idiag*(row-currentpos/5)/abs(currentpos/5-row);
               int checkcol = (currentpos%5) + idiag*(col-currentpos%5)/abs(currentpos%5-col);
//               cout << "Checking " << checkrow << " " << checkcol << "\n";
               if (board[checkrow*5+checkcol] != 0)
               {
                  return 0;
               }
            }
         }
//         else if ((currentpos-newpos)%6 == 0)
//         {
//         }
         else
         {
   //         cout << "Not a valid move by row/col\n";
            return 0;
         }
         // Check to make sure position is not occupied
         if (board[row*5+col] != 0)
         {
   //         cout << "Spot is occupied\n";
            return 0;
         }
   //      cout << "Fell out must be legal\n";
         return 1;
      }
      vector<pair<int,int> > currentmoves()
      {
         vector<pair<int,int> > availmoves;
         pair <int, int> currentmove;
         for (int irow = 0; irow < 5; irow++)
         {
            for (int icol = 0; icol < 5; icol++)
            {
//               cout << "Checking move to " << irow << " " << icol << "\n";
               currentmove.first = irow;
               currentmove.second = icol;
               if (islegalmove(irow,icol,turn))
               {
//                  cout << "Added " << irow << " " << icol << " to available moves\n";
                  availmoves.push_back(currentmove);
               }
            }
         }
         return availmoves;
      }
};

void gamestate::reset()
{
   for (int irow=0; irow<5; irow++)
   {
      for (int icol=0; icol<5; icol++)
      {
//         board[irow*5+icol] = irow*5+icol;
         board[irow*5+icol] = 0;
      }
   }
   turn=1;
   pos1=-1;
   pos2=-1;
}

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
   availmoves = currgame.currentmoves();
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

int main()
{
//   cout << "hello world\n";
   gamestate game1;
   game1.reset();
   printboardstate(game1);
//   game1.applymove(2,2);
//   printboardstate(game1);
   while (!game1.currentmoves().empty())
   {
      if (game1.getturn() == 1)
      {
         entermanualmove(game1);
      }
      else
      {
      airand(game1);
      }
      printboardstate(game1);
   }
   cout << "Game Over: Player " << game1.getturn() << " loses\n";
}
