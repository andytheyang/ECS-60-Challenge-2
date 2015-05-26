#ifndef ROUTER_H
#define	ROUTER_H

#include "RouterRunner.h"

class Router {
public:
  Router(CityInfo *infos, int numCities);
  int setTransfers(Transfer **transfers);
}; // class Router 

#endif	// ROUTER_H

