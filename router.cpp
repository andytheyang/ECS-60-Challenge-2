// Author: Andy Yang and Manvinder Sodhi

#include "router.h"
#include "RouterRunner.h"
#include "QueueAr.h"
#include <iostream>
#include <cassert>
#include <cstring>

using namespace std;

Router::Router(CityInfo *info, int num) : numCities(num)
{
  cities = new CityInfo[numCities];
  memcpy(cities, info, numCities * sizeof(CityInfo));
} // Router()

int Router::setTransfers(Transfer **transfers)
{
//  int numTransfer = 0;
  int visited[numCities];
  memset(visited, 0, numCities * sizeof(int));
//  int levels[numCities];
//  int pathLength = 0;
//  StackAr<int> surplus(numCities);
  Queue<int> adjQ(numCities);	// approximate pathlength
  primeTransfers(transfers);
/*
  for (int i = 0; i < numCities; i++)
  {
    if (getNet(i) > 0)
      surplus.enqueue(i);
  }  // for all cities
*/
//  while (!surplus.isEmpty())
//  {
  int flag = 0;
  for (int currentParent = 0; currentParent < numCities; currentParent++)	// loop until is a surplus, then execute for code
  {
    flag++;
    if (getNet(currentParent) <= 0)
      continue;
//    int currentParent = surplus.dequeue();
    CityInfo current = cities[currentParent];
    int currentNum = currentParent;
    adjQ.makeEmpty();
//    pathLength = 0;
    parents[currentParent][1] = -1;
    int net = getNet(currentParent);
    do
    {
      for (int i = 0; net > 0 && i < current.adjCount; i++)
      {
        int process;

        if (visited[process = current.adjList[i]] == flag)
          continue;

//        curPath[pathLength][0] = i;
//        curPath[pathLength][1] = process;
//        pathLength++;

        parents[process][0] = i;		// store toIndex into parents storage (for forward traversal)
        parents[process][1] = currentNum;	// store parent to parents storage (for backward traversal)
//        levels[process] = pathLength;		// store pathLength into levels storage for later generation of path


        adjQ.enqueue(process);	// store city location
        visited[process] = flag;	// lazy flagging
//        int transferAmount = min(net, -getNet(process));
        int transferAmount = min(net, cities[process].usage - cities[process].production);

        if (transferAmount <= 0)
          continue;

//        numTransfer += transferAmount * pathLength;	// TODO: check this
  //      transferPath(transfers, currentParent, pathLength, transferAmount);
        net -= transferAmount;
        transferTo(transfers, currentParent, process, transferAmount);
//        pathLength--;	// remove last node from path
      }  // for each adjacency

      if (adjQ.isEmpty() || net <= 0)
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
  return getNumTransfer(transfers);  // should be set to total of all transfers.
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

void Router::transferTo(Transfer **transfers, int parent, int to, int amount)
{
  cities[parent].production -= amount;
  cities[to].production += amount;
  while (to != parent)
  {
    getTransfer(transfers, parents[to][1], parents[to][0])->amount += amount;
    to = parents[to][1];
  }
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

int Router::getNumTransfer(Transfer **transfers)
{
  int numTransfer = 0;
  for (int i = 0; i < numCities; i++)
    for (int j = 0; j < cities[i].adjCount; j++)
      numTransfer += getTransfer(transfers, i, j)->amount;
  return numTransfer;
}  // getNumTransfer()

void Router::printCities() const
{
    for (int i = 0; i < numCities; i++)
        cout << "City " << i << " nets " << getNet(i) << endl;
}  // printCities()


void Router::printTransfer(Transfer **transfers, int city) const
{
  Transfer *cityTrans = transfers[city];
  for (int i = 0; i < 8; i++)
      cout << "From city " << city << " to " << cityTrans[i].destCity << ": " << cityTrans[i].amount << endl;
}  // printTransfer()

/*
void Router::printCurPath(int curPath[MAX_PATH][2], int pathLength) const
{
  int current = curPath[0][1];
  for (int i = 0; i < pathLength; i++)
  {
    cout << "Connecting " << current << " to " << cities[current].adjList[curPath[i][0]] << endl;
    current = cities[current].adjList[curPath[i][0]];
  }  // for all elements in path

}  // printCurPath()
*/
/*
int min(int a, int b)
{
  if (a > b)
    return a;
  return b;
}
*/
