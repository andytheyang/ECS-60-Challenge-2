// Author: Sean Davis 
// Created on May 18, 2015, 7:33 PM
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include "CPUTimer.h"
#include "RouterRunner.h"
#include "router.h"
using namespace std;


void checkFlows(CityInfo infos[], int numCities, 
  Transfer **transfers, int theirTotal)
{
  bool OK = true;
  int i, j, city, total = 0;
  
  for(city = 0; city < numCities; city++)
  {
    for(i = 0; i < 8; i++)
      if(transfers[city][i].amount > 0)
      {
        for(j = 0; j < infos[city].adjCount; j++)
          if(transfers[city][i].destCity == infos[city].adjList[j])
            break;
        
        if(j == infos[city].adjCount )
        {
          cout << city << " is not adjacent to " << transfers[city][i].destCity  << endl;
          OK = false;
        }
        else
        {
          infos[city].production -= transfers[city][i].amount;
          infos[transfers[city][i].destCity].production += transfers[city][i].amount;
          total += transfers[city][i].amount;
        }  
      }  // if non-zero transfer
  }  // for city
 
  if(OK)
  {
    for(city = 0; city < numCities  && OK; city++)
      if(infos[city].production > infos[city].usage)
      {
        cout << setw(5) << city << " has " << setw(5) 
          << infos[city].production - infos[city].usage
          << " MORE electricity than needed\n";
        OK = false;
      }  // if production > usage
      else  // production <= usage
        if(infos[city].production < infos[city].usage)
        {
          cout << setw(5) << city << " has " << setw(5) 
            << infos[city].usage - infos[city].production 
            << " LESS electricity than needed\n";
          OK = false;
        }  // production < usage
  } // if transfers OK     

  if(theirTotal != total)
  {
    cout << "Your total of " << theirTotal << " does not match real total of "
      << total << endl;
    OK = false;
  }

  if(!OK)
    cout << "There are errors" << endl;

} //checkFlows()

CityInfo* readFile(char *filename, int *numCities)
{
  char s[256], *ptr;
  int city;

  ifstream inf(filename);
  inf >> *numCities;
  CityInfo *infos = new CityInfo[*numCities];
  inf.getline(s, 256);
  while(inf.getline(s, 256))
  {
    city = atoi(strtok(s, ","));
    infos[city].usage = atoi(strtok(NULL, ","));
     infos[city].production = atoi(strtok(NULL, ","));
    while((ptr = strtok(NULL, ",")) != NULL)
      infos[city].adjList[infos[city].adjCount++] = atoi(ptr);
  }

  return infos;
}

int main(int argc, char** argv) 
{
  CPUTimer ct;
  int numCities;
  Transfer **transfers = new Transfer*[25000];

  for(int i = 0; i < 25000; i++)
    transfers[i] = new Transfer[8];

  CityInfo *infos = readFile(argv[1], &numCities);
  CityInfo *infos2 = new CityInfo[numCities];
  memcpy(infos2, infos, numCities * sizeof(CityInfo));
  ct.reset();
  Router *router = new Router(infos, numCities);
  delete [] infos;
  int theirTotal = router->setTransfers(transfers);
  double time = ct.cur_CPUTime();
  checkFlows(infos2, numCities, transfers, theirTotal);
  cout << "Transfer total: "  << theirTotal << " CPU Time: " << time << endl;
  return 0;
} // main())

