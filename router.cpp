// Author: Andy Yang and Manvinder Sodhi

#include "router.h"
#include "RouterRunner.h"
#include "StackAr.h"
#include "QueueAr.h"
#include <iostream>
#include <algorithm>
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
    int visited[25000] = {0};
    int curPath[25000][2];
    int pathLength = 0;
    StackAr<int> surplus(25000);
    Queue<int> adjQ(1000);

    for (int i = 0; i < numCities; i++)
    {
        if (getNet(i) > 0)
            surplus.push(i);
    }  // for all cities
/*    
    while (!surplus.isEmpty())	// while there are more surplus cities
    {
        int current = surplus.topAndPop();
        //    cout << current << endl;
        CityInfo *curCity = &(cities[current]);
        
        // TODO: change <= to !=
        for (int i = 0; i < curCity->adjCount && getNet(current) >= 0; i++)
        {
            if (getNet(curCity->adjList[i]) > 0)	// is a surplus
                continue;

            // transfer maximum possible
            int transferAmount = min(getNet(current), -getNet(curCity->adjList[i]));
            numTransfer += transferAmount;
            transfer(transfers, current, i, transferAmount);
            // TODO: implement multicity traversals
        }  // for all adjacencies
    }  // while
*/

/*
  while (!surplus.isEmpty())
  {
    CityInfo current = cities[surplus.topAndPop()];
    CityInfo currentParent = current;
    adjQ.makeEmpty();

    do
    {
      for(int i = 0; getNet(currentParent) > 0 && i < current.adjCount; i++)
      {
        path.push(i);
        // try to push power along path
      }  // for each adjacency
    } while (!adjQ.isEmpty());

  }  // while there are more surplus cities
*/
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
    trans->amount = amount;
    // optimize by storing cities[from] first
    cities[from].production -= amount;
    cities[cities[from].adjList[toIndex]].production += amount;
}  // transfer()

void Router::transferPath(Transfer **transfers, int parent, int **curPath, int pathLength, int amount)
{
  for (int i = 0; i < pathLength; i++)
  {
    transfer(transfers, parent, curPath[i][0], amount);
    parent = curPath[i][0];
  }
}  // transferPath()

Transfer* Router::getTransfer(Transfer **transfers, int from, int toIndex)
{
    // TODO: static?
    Transfer *adj = &(transfers[from][toIndex]);
    
    if (adj->destCity == 0 && adj->amount == 0)	// not initialized
    {
        adj->destCity = cities[from].adjList[toIndex];	// initialize
        //    cout << "making new transfer to " << adj->destCity << endl;
    }
    
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
