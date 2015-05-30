#ifndef ROUTER_H
#define	ROUTER_H

#include "RouterRunner.h"
#include "StackAr.h"
#include <cstring>

class Router {
public:
  Router(CityInfo *info, int num);
  int setTransfers(Transfer **transfers);
private:
  CityInfo *cities;
  int numCities;
  int getNet(int city) const;
  int getNet(CityInfo city) const;
  void transfer(Transfer **transfers, int from, int toIndex, int amount);	// moves production
  void transferPath(Transfer **transfers, int parent, int curPath[25000][2], int pathLength, int amount);
  Transfer* getTransfer(Transfer **transfers, int from, int toIndex);
  void printCities() const;
  void printTransfer(Transfer **transfers, int city) const;
}; // class Router 

#endif	// ROUTER_H

