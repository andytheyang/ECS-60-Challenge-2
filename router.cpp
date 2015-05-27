// Author: Andy Yang and Manvinder Sodhi

#include "router.h"
#include "RouterRunner.h"
#include <iostream>

using namespace std;

Router::Router(CityInfo *info, int num) : cities(info), numCities(num)
{
} // Router()


int Router::setTransfers(Transfer **transfers)
{
  return 0;  // should be set to total of all transfers.
}  // setTransfers

// ---PRIVATE---

void Router::transfer(Transfer **transfers, int from, int toIndex, int amount)
{
  Transfer trans = getTransfer(transfers, from, toIndex);
  trans.amount = amount;
  // optimize by storing cities[from] first
  cities[from].production -= amount;
  cities[cities[from].adjList[toIndex]].production += amount;
}  // transfer()

Transfer& Router::getTransfer(Transfer **transfers, int from, int toIndex)
{
  Transfer adj = transfers[from][toIndex];

  if (adj.destCity == 0 && adj.amount == 0)	// not initialized
    adj.destCity = cities[from].adjList[toIndex];	// initialize

  return adj;
}  // getTransfer()
