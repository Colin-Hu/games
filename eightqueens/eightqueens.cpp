#include <iostream>
#include <vector>

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

int main()
{
   cout << "Hello World\n";
   cout << "4 " << factorial(4) << "\n";
   boardstate newboard(8);
   newboard.printboard();
   int temparray[8] = {4,2,7,4,1,6,3,8};
   newboard.assignvalues(temparray);
   newboard.printboard();
   cout << "Fitness " << newboard.evaluatefitness() << "\n";
}
