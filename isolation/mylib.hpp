#include <iostream>

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

class mytree
{
      pair<int,int> move;
      float eval;
      mytree* parent;
      vector<mytree> children;
      int level;
   public:
      void initialize()
      {
         parent = NULL;
         move.first = -1;
         move.second = -1;
         level = 0;
      }
      void addlevel(gamestate currgame, int depth)
      {
         cout << "Entering addlevel depth " << depth << " level " << this->level <<"\n";
         if (depth > 0)
         {
            if (this->children.size() == 0)
            {
               vector<pair<int,int> > possiblemoves = currgame.currentmoves();
               cout << "Adding to level \n";
               for (int i = 0; i < possiblemoves.size(); i++)
               {
                  mytree newnode;
                  newnode.initialize();
                  newnode.parent = this;
                  newnode.level = this->level + 1;
                  newnode.move.first = possiblemoves[i].first;
                  newnode.move.second = possiblemoves[i].second;
                  cout << "Added move for player " << newnode.move.first << " " << newnode.move.second << " " << currgame.getturn() << " " << newnode.level << "\n";
                  children.push_back(newnode);
               }
            }
            cout << "Total children " << this->children.size() << "\n";
            for (int i = 0; i < this->children.size(); i++)
            {
               gamestate tempgame = currgame;
               cout << "Creating new game state by applying move " << this->children[i].move.first << " " << this->children[i].move.second << "\n";
               tempgame.applymove(this->children[i].move.first,this->children[i].move.second);
               cout << "Recursing " << depth-1 << "\n";
               addlevel(tempgame, depth-1);
            }
         }
      }
};
