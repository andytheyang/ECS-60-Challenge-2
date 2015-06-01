// Author: Andy Yang and Manvinder Sodhi

#include "router.h"
#include "RouterRunner.h"
#include "StackAr.h"
#include "QueueAr.h"
#include <iostream>
//#include <algorithm>
#include <cstring>

using namespace std;

Router::Router(CityInfo *info, int num) : numCities(num)
{
  cities = new CityInfo[numCities];
  memcpy(cities, info, numCities * sizeof(CityInfo));
    // TODO: duplicate info??
} // Router()
/*
int Router::setTransfers(Transfer **transfers)
{
  int curPath[25000][2];
  curPath[0][0] = 0;
  curPath[0][1] = 1;
  curPath[1][0] = 1;
  curPath[1][1] = 2;
  transferPath(transfers, 0, curPath, 2, 100);
  printTransfer(transfers, 0);
  return 200;
}
*/

int Router::setTransfers(Transfer **transfers)
{
  int numTransfer = 0;
  int visited[25000] = {0};
  int levels[25000] = {0};
  int paths[25000][40][2];
//    int level = 0;	// for level-order traversal
  int curPath[25][2];
  int pathLength = 0;
  StackAr<int> surplus(25000);
  Queue<int> adjQ(1000);

  for (int i = 0; i < numCities; i++)
  {
    if (getNet(i) > 0)
      surplus.push(i);
//    cout << i << " nets " << getNet(i) << endl;
  }  // for all cities

  while (!surplus.isEmpty())
  {
    int currentParent = surplus.topAndPop();
    CityInfo current = cities[currentParent];
    int currentNum = currentParent;
    adjQ.makeEmpty();
    pathLength = 0;
    for (int i = 0; i < 25000; i++)
    {
      visited[i] = 0;
    }

    do
    {
      for (int i = 0; getNet(currentParent) > 0 && i < current.adjCount; i++)
      {
//        if (visited[current.adjList[i]] || getNet(current.adjList[i]) > 0)
//        if (visited[current.adjList[i]])
//          continue;
        int process = current.adjList[i];
        if (visited[process])
          continue;
//        cout << "processing " << process << endl;

        curPath[pathLength][0] = i;
        curPath[pathLength][1] = process;
        pathLength++;

        levels[process] = pathLength;

	for (int j = 0; j < pathLength; j++)
        {
           paths[process][j][0] = curPath[j][0];
           paths[process][j][1] = curPath[j][1];
        }

        int transferAmount = min(getNet(currentParent), -getNet(process));
        adjQ.enqueue(process);	// store city location
        visited[process] = 1;

        if (transferAmount <= 0)
        {
          pathLength--;
          continue;
        }
//        cout << transferAmount << endl;
        numTransfer += transferAmount * pathLength;	// TODO: check this
        transferPath(transfers, currentParent, curPath, pathLength, transferAmount);
//        adjQ.enqueue(i);
        pathLength--;	// remove last node from path
      }  // for each adjacency

//      visited[currentNum] = 1;

      if (adjQ.isEmpty() || getNet(currentParent) <= 0)
        break;

//      int nextIndex = adjQ.dequeue();
      // TODO: fix this
//      currentNum = current.adjList[nextIndex];
//      current = cities[currentNum];

      currentNum = adjQ.dequeue();
      current = cities[currentNum];

      pathLength = levels[currentNum];
      for (int i = 0; i < pathLength; i++)
      {
        curPath[i][0] = paths[currentNum][i][0];
	curPath[i][1] = paths[currentNum][i][1];
      }

//      curPath[pathLength][0] = nextIndex;	// add dequeued item to the path stack
//      curPath[pathLength][1] = currentNum;
//      pathLength++;
    } while (true);  // while there are more adjacencies
  }  // while more surplus cities

  for (int i = 0; i < numCities; i++)
    if (getNet(i))
      cout << "City " << i << " nets " << getNet(i) << endl;

  // TODO: recalculate numTransfer
  return numTransfer;  // should be set to total of all transfers.
}  // setTransfers

// ---PRIVATE---

int Router::getNet(int city) const
{
  return cities[city].production - cities[city].usage;
}  // getNet()

int Router::getNet(CityInfo city) const
{
  return city.production - city.usage;
}  // getNet()

void Router::transfer(Transfer **transfers, int from, int toIndex, int amount)
{
  Transfer *trans = getTransfer(transfers, from, toIndex);
  trans->amount += amount;
  // optimize by storing cities[from] first
  cities[from].production -= amount;
  cities[cities[from].adjList[toIndex]].production += amount;
}  // transfer()

void Router::transferPath(Transfer **transfers, int parent, int curPath[50][2], int pathLength, int amount)
{
//  cout << "transferring " << parent << " to " << curPath[pathLength - 1][1] << ": " << amount << endl;
  for (int i = 0; i < pathLength; i++)
  {
    transfer(transfers, parent, curPath[i][0], amount);
    parent = curPath[i][1];
  }
}  // transferPath()

Transfer* Router::getTransfer(Transfer **transfers, int from, int toIndex)
{
    // TODO: static?
  Transfer *adj = &(transfers[from][toIndex]);
//  if (adj->destCity == 0 && adj->amount == 0)	// not initialized
//  {
  adj->destCity = cities[from].adjList[toIndex];	// initialize
//    cout << "making new transfer to " << adj->destCity << endl;
//  }
  return adj;
}  // getTransfer()

void Router::printCities() const
{
    for (int i = 0; i < numCities; i++)
    {
        cout << "City " << i << " nets " << getNet(i) << endl;
    }  // for all cities
}  // printCities()


void Router::printTransfer(Transfer **transfers, int city) const
{
    Transfer *cityTrans = transfers[city];
    for (int i = 0; i < 8; i++)
    {
        cout << "From city " << city << " to " << cityTrans[i].destCity << ": " << cityTrans[i].amount << endl;
    }  // for all adjacencies
}  // printTransfer()
/*
int min(int a, int b)
{
  if (a > b)
    return a;
  return b;
}
*/
