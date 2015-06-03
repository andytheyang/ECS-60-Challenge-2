#ifndef ROUTER_H
#define	ROUTER_H

#include "RouterRunner.h"
#include "StackAr.h"
#include <cstring>

const int MAX_PATH = 1000;

class Router {
public:
  Router(CityInfo *info, int num);
  int setTransfers(Transfer **transfers);
private:
  CityInfo *cities;
  int numCities;
  int parents[25000][2];
  int getNet(int city) const;
  int getNet(CityInfo city) const;
//  void transfer(Transfer **transfers, int from, int toIndex, int amount);	// moves production
//  void transferPath(Transfer **transfers, int parent, int curPath[MAX_PATH][2], int pathLength, int amount);
//  void transferPath(Transfer **transfers, int parent, int pathLength, int amount);
  void transferTo(Transfer **transfers, int parent, int to, int amount);
  Transfer* getTransfer(Transfer **transfers, int from, int toIndex);
  void primeTransfers(Transfer **transfers);
  int getNumTransfer(Transfer **transfers);
  void printCities() const;
  void printTransfer(Transfer **transfers, int city) const;
  void printCurPath(int curPath[MAX_PATH][2], int pathLength) const;
}; // class Router 

#endif	// ROUTER_H

