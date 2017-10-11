#include <iostream>
#include <vector>
#include <stdlib.h>

using namespace std;

int factorial(int x)
{
   if (x == 1 || x == 0)
   {
      return 1;
   }
   else
   {
      return x*factorial(x-1);
   }
}

class boardstate
{
      vector<int> positions;
      int fitness, totalthreats;
   public:
      boardstate(int n)
      {
         for (int i=0; i<n; i++)
         {
            positions.push_back(0);
         }
         totalthreats = n*(n-1)/2;
      }
      void printboard()
      {
         for (int i=0; i<positions.size(); i++)
         {
            cout << positions[i];
         }
         cout << "\n";
      }
      int boardsize()
      {
         return positions.size();
      }
      int accessboard(int pos)
      {
         return positions[pos];
      }
      int assignvalue(int pos, int value)
      {
         if (value < 1 || value > positions.size())
         {
            cout << "assignvalue: Error invalid position " << pos << " " << value << "\n";
            return 0;
         }
         positions[pos] = value;
      }
      int assignvalues(int values[])
      {
         for (int i=0; i<positions.size(); i++)
         {
            if (values[i] < 1 || values[i] > positions.size())
            {
               cout << "assignvalues: Error invalid position " << i << " " << values[i] << "\n";
               return 0;
            }
            positions[i] = values[i];
         }
         return 1;
      }
      int randomboard()
      {
         for (int i=0; i<positions.size(); i++)
         {
            positions[i] = rand() % positions.size() + 1;
         }
      }
      int betterrandomboard()
      {
         vector<int> pospool;
         for (int i=0; i<positions.size(); i++)
         {
            pospool.push_back(i+1);
         }
         for (int i=0; i<positions.size(); i++)
         {
            int selectedpos;
            selectedpos = rand() % pospool.size();
            positions[i] = pospool[selectedpos];
            pospool.erase(pospool.begin()+selectedpos);
         }
      }
      int evaluatefitness()
      {
         int threats = 0;
         // Walk through the positions past the first queen
         for (int i=1; i<positions.size(); i++)
         {
            // Check against each queen in previous positions
            for (int j=i-1; j>=0; j--)
            {
               // Check for threat against row and diagonals
               if (positions[i] == positions[j] || positions[i] == positions[j]+(i-j) || positions[i] == positions[j]-(i-j))
               {
                  threats++;
               }
            }
         }
//         cout << "Post evaluate " << totalthreats << " " << threats << "\n";
         fitness = totalthreats - threats;
         return fitness;
      }
};

boardstate mate(boardstate parentA, boardstate parentB)
{
   boardstate child(parentA.boardsize());
   for (int i=0; i<child.boardsize(); i++)
   {
      if (i < child.boardsize()/2)
      {
         child.assignvalue(i,parentA.accessboard(i));
      }
      else
      {
         child.assignvalue(i,parentB.accessboard(i));
      }
   }
   return child;
}

class boardpopulation
{
      vector<boardstate> totalpop;
      vector<int> matingprob;
   public:
      boardpopulation(int nel, int npop)
      {
         boardstate newboard(nel);
         for (int i=0; i<npop; i++)
         {
            newboard.randomboard();
            totalpop.push_back(newboard);
         }
      }
      int clearpop()
      {
         totalpop.clear();
      }
      int addboard(boardstate newboard)
      {
         totalpop.push_back(newboard);
      }
      int printpop()
      {
         for (int i=0; i<totalpop.size(); i++)
         {
            cout << i << " Fitness " << totalpop[i].evaluatefitness() << " ";
            totalpop[i].printboard();
         }
      }
      boardpopulation createnewgeneration()
      {
         boardpopulation newgeneration(totalpop[0].boardsize(), totalpop.size());
         newgeneration.clearpop();
         // Create probability distribution of parents based on fitness
         for (int i=0; i<totalpop.size(); i++)
         {
            for (int lots=0; lots<totalpop[i].evaluatefitness(); lots++)
            {
               matingprob.push_back(i);
            }
         }
         // Create 2 children per pair of parents based on mate()
         for (int i=0; i<totalpop.size()/2; i++)
         {
            int parent1pos = rand() % matingprob.size();
            int parent2pos = rand() % matingprob.size();
            boardstate childboard = mate(totalpop[matingprob[parent1pos]],totalpop[matingprob[parent2pos]]);
            newgeneration.addboard(childboard);
            childboard = mate(totalpop[matingprob[parent2pos]],totalpop[matingprob[parent1pos]]);
            newgeneration.addboard(childboard);
         }
         return newgeneration;
      }
      int applymutation()
      {
         // Randomly mutate some children
         // 1 in 4 chance of random single mutation
         for (int i=0; i<totalpop.size(); i++)
         {
            int mutate = rand() % 4;
            if (mutate == 0)
            {
               int position = rand() % totalpop[i].boardsize();
               int value = rand() % totalpop[i].boardsize();
               totalpop[i].assignvalue(position,value);
               cout << "Mutating child " << i << " position " << position << " value " << value << "\n";
            }
         }
      }
};

int main()
{
   cout << "Hello World\n";
   cout << "4 " << factorial(4) << "\n";
   boardstate newboard(8);
   newboard.printboard();
   int temparray[8] = {2,4,6,8,3,1,7,5};
//   newboard.assignvalues(temparray);
//   newboard.randomboard();
//   newboard.betterrandomboard();
//   newboard.printboard();
//   cout << "Fitness " << newboard.evaluatefitness() << "\n";
//   boardstate parent1(8), parent2(8);
//   parent1.randomboard();
//   cout << "Parent 1 ";
//   parent1.printboard();
//   parent2.randomboard();
//   cout << "Parent 2 ";
//   parent2.printboard();
//   boardstate childboard = mate(parent1,parent2);
//   cout << "Child board ";
//   childboard.printboard();
   cout << "Creating population\n";
   boardpopulation gen1(8, 10);
   gen1.printpop();
   boardpopulation gen2 = gen1.createnewgeneration();
   cout << "Finished newgen2\n";
   gen2.printpop();
   cout << "Mutating gen2\n";
   gen2.applymutation();
   gen2.printpop();
//   boardpopulation gen3 = gen2.createnewgeneration();
//   cout << "Finished newgen3\n";
//   gen3.printpop();
}
