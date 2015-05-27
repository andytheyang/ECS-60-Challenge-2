// Author: Andy Yang and Manvinder Sodhi

#include "router.h"
#include "RouterRunner.h"
#include <iostream>

using namespace std;

Router::Router(CityInfo *info, int num) : cities(info), numCities(num)
{
  // TODO: duplicate info??
} // Router()


int Router::setTransfers(Transfer **transfers)
{
  int numTransfer = 0;

  transfer(transfers, 0, 1, 1000);
  printTransfer(transfers, 0);
  return numTransfer;  // should be set to total of all transfers.
}  // setTransfers

// ---PRIVATE---

void Router::transfer(Transfer **transfers, int from, int toIndex, int amount)
{
  Transfer *trans = getTransfer(transfers, from, toIndex);
  trans->amount = amount;
  // optimize by storing cities[from] first
  cities[from].production -= amount;
  cities[cities[from].adjList[toIndex]].production += amount;
}  // transfer()

Transfer* Router::getTransfer(Transfer **transfers, int from, int toIndex)
{
  // TODO: static?
  Transfer *adj = &(transfers[from][toIndex]);

  if (adj->destCity == 0 && adj->amount == 0)	// not initialized
  {
    adj->destCity = cities[from].adjList[toIndex];	// initialize
    cout << "making new transfer to " << adj->destCity << endl;

  }

  return adj;
}  // getTransfer()

void Router::printCities() const
{
  for (int i = 0; i < numCities; i++)
  {
    cout << "City " << i << " nets " << cities[i].production - cities[i].usage << endl;
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
