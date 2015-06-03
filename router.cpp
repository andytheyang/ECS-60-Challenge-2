// Author: Andy Yang and Manvinder Sodhi

#include "router.h"
#include "RouterRunner.h"
#include "StackAr.h"
#include "QueueAr.h"
#include <iostream>
#include <cassert>
#include <cstring>

using namespace std;

Router::Router(CityInfo *info, int num) : numCities(num)
{
  cities = new CityInfo[numCities];
  memcpy(cities, info, numCities * sizeof(CityInfo));
    // TODO: duplicate info??
} // Router()

int Router::setTransfers(Transfer **transfers)
{
  int numTransfer = 0;
  int visited[numCities];
  memset(visited, 0, numCities * sizeof(int));
//  int levels[numCities];
//  int pathLength = 0;
  Queue<int> surplus(numCities);
  Queue<int> adjQ(numCities / 2);
  primeTransfers(transfers);

  for (int i = 0; i < numCities; i++)
  {
    if (getNet(i) > 0)
      surplus.enqueue(i);
  }  // for all cities

  int flag = 0;
  while (!surplus.isEmpty())
  {
    flag++;
    int currentParent = surplus.dequeue();
    CityInfo current = cities[currentParent];
    int currentNum = currentParent;
    adjQ.makeEmpty();
//    pathLength = 0;
    parents[currentParent][1] = -1;

    do
    {
      for (int i = 0; getNet(currentParent) > 0 && i < current.adjCount; i++)
      {
        int process = current.adjList[i];

        if (visited[process] == flag)
          continue;

//        curPath[pathLength][0] = i;
//        curPath[pathLength][1] = process;
//        pathLength++;

        parents[process][0] = i;		// store toIndex into parents storage (for forward traversal)
        parents[process][1] = currentNum;	// store parent to parents storage (for backward traversal)
//        levels[process] = pathLength;		// store pathLength into levels storage for later generation of path


        adjQ.enqueue(process);	// store city location
        visited[process] = flag;	// lazy flagging
        int transferAmount = min(getNet(currentParent), -getNet(process));

        if (transferAmount <= 0)
        {
//          pathLength--;
          continue;
        }

//        numTransfer += transferAmount * pathLength;	// TODO: check this
  //      transferPath(transfers, currentParent, pathLength, transferAmount);
        numTransfer += transferTo(transfers, currentParent, process, transferAmount);
//        pathLength--;	// remove last node from path
      }  // for each adjacency

      if (adjQ.isEmpty() || getNet(currentParent) <= 0)
        break;

      currentNum = adjQ.dequeue();
      current = cities[currentNum];
//      pathLength = levels[currentNum];

/*      int tempCur = currentNum;
      for (int i = pathLength - 1; i >= 0; i--)
      {
        curPath[i][1] = tempCur;
        curPath[i][0] = parents[tempCur][0];
        tempCur = parents[tempCur][1];
      }
*/
    } while (true);  // while there are more adjacencies
  }  // while more surplus cities

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

/*
void Router::transfer(Transfer **transfers, int from, int toIndex, int amount)
{
  Transfer *trans = getTransfer(transfers, from, toIndex);
  trans->amount += amount;
  // optimize by storing cities[from] first
  cities[from].production -= amount;
  cities[cities[from].adjList[toIndex]].production += amount;
}  // transfer()

void Router::transferPath(Transfer **transfers, int parent, int pathLength, int amount)
{
//  cout << "transferring " << parent << " to " << curPath[pathLength - 1][1] << ": " << amount << endl;
  cities[parent].production -= amount;
  for (int i = 0; i < pathLength; i++)
  {
    getTransfer(transfers, parent, curPath[i][0])->amount += amount;
//    trans->amount += amount;
//    transfer(transfers, parent, curPath[i][0], amount);
    parent = curPath[i][1];
  }
  cities[curPath[pathLength - 1][1]].production += amount;
}  // transferPath()
*/

int Router::transferTo(Transfer **transfers, int parent, int to, int amount)
{
  int transferNum = 0;
  cities[parent].production -= amount;
  cities[to].production += amount;
  while (to != parent)
  {
    getTransfer(transfers, parents[to][1], parents[to][0])->amount += amount;
    transferNum += amount;
    to = parents[to][1];
  }
  return transferNum;
}  // transferTo

Transfer* Router::getTransfer(Transfer **transfers, int from, int toIndex)
{
//  Transfer *adj = &(transfers[from][toIndex]);
//  adj->destCity = cities[from].adjList[toIndex];	// initialize
  return &(transfers[from][toIndex]);
//  return adj;
}  // getTransfer()

void Router::primeTransfers(Transfer **transfers)
{
  for (int i = 0; i < numCities; i++)
    for (int j = 0; j < cities[i].adjCount; j++)
      getTransfer(transfers, i, j)->destCity = cities[i].adjList[j];
}

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

void Router::printCurPath(int curPath[MAX_PATH][2], int pathLength) const
{
  int current = curPath[0][1];
  for (int i = 0; i < pathLength; i++)
  {
    cout << "Connecting " << current << " to " << cities[current].adjList[curPath[i][0]] << endl;
    current = cities[current].adjList[curPath[i][0]];
  }  // for all elements in path

}  // printCurPath()
/*
int min(int a, int b)
{
  if (a > b)
    return a;
  return b;
}
*/
