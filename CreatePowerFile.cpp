#include <fstream>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

using namespace std;

void createGrid(int numCounties, int **grid)
{
  int side = (int) ceil(sqrt(numCounties));
  int count = numCounties, **country = new int*[side], *ids = new int[numCounties];

  for(int i = 0; i < numCounties; i++)
  {
    ids[i] = i;
    for(int j = 0; j < 9; j++)
      grid[i][j] = -1;
  }

  for(int i = 0; i < side; i++)
  {
    country[i] = new int[side];
    for(int j = 0; j < side; j++)
      country[i][j] = -1;
    for(int j = 0; j < side && count > 0; j++)
    {
       int pos = rand() % count;
       country[i][j] = ids[pos];
       ids[pos] = ids[--count]; // move last to this position
    }
  } // for i

  count = 0;
  for(int i = 0; i < side; i++)
    for(int j = 0; j < side; j++)
    {
      if(country[i][j] < 0)
        continue;
      grid[count][0] = country[i][j];  // county number
      int adj = 1;
      if(i > 0 && j > 0)
        grid[count][adj++] = country[i - 1][j - 1];
      if(i > 0)
        grid[count][adj++] = country[i - 1][j];
      if(i > 0 && j < side - 1)
        grid[count][adj++] = country[i - 1][j + 1];
       if(j > 0)
        grid[count][adj++] = country[i][j - 1];
      if(j < side - 1)
        grid[count][adj++] = country[i][j + 1];
      if(i < side - 1 && j > 0)
        grid[count][adj++] = country[i + 1][j - 1];
      if(i < side - 1)
        grid[count][adj++] = country[i + 1][j];
      if(i < side - 1 && j < side -1)
        grid[count][adj++] = country[i + 1][j + 1];
      count++;
    }
}

void writeFile(int **grid, int numCounties, int seed)
{
  char filename[80];
  int i, usage = 0, production = 0, value;
  sprintf(filename,"power%d_%d.csv",numCounties, seed);
  ofstream outf (filename);
  
  outf << numCounties << endl;

  for(i = 0; i < numCounties - 1; i++)
  {
    outf << grid[i][0] << ",";
    usage += value = rand() % 10000;
    outf << value << ",";
    production += value = rand() % 10000;
    outf << value;
    for(int j = 1; j < 9; j++)
      if(grid[i][j] >= 0)
        outf << "," << grid[i][j];
    outf << endl;
  }

  outf << grid[i][0] << ",";
  if(production > usage)
    usage += value = production - usage;
  else // production < usage
    usage += value = rand() % 10000;
  outf << value << ",";

  value = usage - production;
  outf << value;
  for(int j = 1; j < 9; j++)
    if(grid[i][j] >= 0)
        outf << "," << grid[i][j];
  outf << endl;


  outf.close();
} // writeFile()


int main(int argc, char* argv[])
{
  int seed, numCounties, **grid;
  cout << "Number of counties: ";
  cin >> numCounties;
  cout << "Seed: ";
  cin >> seed;
  srand(seed);
  grid = new int*[numCounties];
  for(int i = 0; i < numCounties; i++)
    grid[i] = new int[9];
  createGrid(numCounties, grid);
  writeFile(grid, numCounties, seed);
  return 0;
}
//---------------------------------------------------------------------------
