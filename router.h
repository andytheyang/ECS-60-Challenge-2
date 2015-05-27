#ifndef ROUTER_H
#define	ROUTER_H

#include "RouterRunner.h"

class Router {
public:
  Router(CityInfo *info, int num);
  int setTransfers(Transfer **transfers);
private:
  CityInfo *cities;
  int numCities;
  void transfer(Transfer **transfers, int from, int toIndex, int amount);	// moves production
  Transfer& getTransfer(Transfer **transfers, int from, int toIndex);
}; // class Router 

/*
class City {
public:
  City();
  void setData(int *aList, short aCount, int use, int prod)
  {
    for (int i = 0; i < 8; i++)
    {
      adjList[i] = aList[i];
    }

    adjCount = (int)aCount;
    usage = use;
    production = prod;
  }
  int adjList[8];
  int adjCount;		// don't deal with data types
  int usage;
  int production;
};  // class City
*/

#endif	// ROUTER_H

