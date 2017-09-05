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
         if (board[position] == 0)
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
            return 0;
         }
      }
};

void gamestate::reset()
{
   for (int irow=0; irow<5; irow++)
   {
      for (int icol=0; icol<5; icol++)
      {
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

int main()
{
   cout << "hello world\n";
   gamestate game1;
   game1.reset();
   printboardstate(game1);
   game1.applymove(2,2);
   printboardstate(game1);
   game1.applymove(2,3);
   printboardstate(game1);
   game1.applymove(3,3);
   printboardstate(game1);
}
