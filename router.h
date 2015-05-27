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
}; // class Router 

#endif	// ROUTER_H

