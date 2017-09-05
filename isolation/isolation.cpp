#include <iostream>
#include <stdlib.h>
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
      };
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
      int currentpos;
      if (player == 1)
      {
         currentpos = pos1;
      }
      else
      {
         currentpos = pos2;
      }
      // check in bounds
      if (row < 0 || row > 4 || col < 0 || col > 4)
      {
         return 0;
      }
//      cout << "Checking legality old pos: " << currentpos << "\n";
      if (currentpos == -1)
      {
//         cout << "First move legal in any position\n";
      }
      // if in row, col, or diag
      else if (row == currentpos/5)
      {
//         cout << "Same row as last position\n";
      }
      else if (col == currentpos%5)
      {
//         cout << "Same col as last position\n";
      }
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
   cout << "----------\n";
}

void entermanualmove(gamestate &currgame)
{
   int row, col;
   cin >> row >> col;
   cout << "Attempting to apply move to " << row << " " << col << "\n";
   currgame.applymove(row,col);
}

int main()
{
   cout << "hello world\n";
   gamestate game1;
   game1.reset();
   printboardstate(game1);
//   game1.applymove(2,2);
//   printboardstate(game1);
   entermanualmove(game1);
   printboardstate(game1);
   entermanualmove(game1);
   printboardstate(game1);
   entermanualmove(game1);
   printboardstate(game1);
   entermanualmove(game1);
   printboardstate(game1);
   entermanualmove(game1);
   printboardstate(game1);
}
