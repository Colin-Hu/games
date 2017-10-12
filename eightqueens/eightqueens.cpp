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
      int maxfitness, fitpos;
   public:
      boardpopulation(int nel, int npop)
      {
         boardstate newboard(nel);
         for (int i=0; i<npop; i++)
         {
            newboard.randomboard();
            totalpop.push_back(newboard);
         }
         maxfitness = -1;
         fitpos = -1;
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
         // 1 in 3 chance of random single mutation
         for (int i=0; i<totalpop.size(); i++)
         {
            int mutate = rand() % 3;
            if (mutate == 0)
            {
               int position = rand() % totalpop[i].boardsize();
               int value = rand() % totalpop[i].boardsize() + 1;
               totalpop[i].assignvalue(position,value);
//               cout << "Mutating child " << i << " position " << position << " value " << value << "\n";
            }
            if (totalpop[i].evaluatefitness() > maxfitness)
            {
               maxfitness = totalpop[i].evaluatefitness();
               fitpos = i;
            }
         }
      }
      int maxfit()
      {
         return maxfitness;
      }
      boardstate fitboard()
      {
         return totalpop[fitpos];
      }
};

int main()
{
   boardpopulation oldboard(8,50);
   for (int generation = 0; generation < 1000; generation++)
   {
      boardpopulation newboard = oldboard.createnewgeneration();
      newboard.applymutation();
      cout << "Generation " << generation << " Best Fit " << newboard.maxfit() << "\n";
      if (newboard.maxfit() == 28)
      {
         newboard.fitboard().printboard();
         break;
      }
      oldboard = newboard;
   }
}
