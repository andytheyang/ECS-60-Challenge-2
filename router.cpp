// Author: Andy Yang and Manvinder Sodhi

#include "router.h"
#include "RouterRunner.h"
#include <iostream>

using namespace std;

Router::Router(CityInfo *info, int num) : cities(info), numCities(num)
{
  for (int i = 0; i < numCities; i++)
  {
    cout << "City " << i << " nets " << cities[i].production - (cities + i)->usage << endl;
//    (infos + i)->production = (infos + i)->usage;
  }  // for all cities
  
} // Router()


int Router::setTransfers(Transfer **transfers)
{
  return 0;  // should be set to total of all transfers.
}  // setTransfers
