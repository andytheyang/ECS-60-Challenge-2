// Author: Sean Davis

#ifndef ROUTERRUNNER_H
#define	ROUTERRUNNER_H


typedef struct 
{
  int adjList[8];
  short adjCount;
  int usage;
  int production;
} CityInfo;

class Transfer
{
public:
  int destCity;
  unsigned amount;
  Transfer(): destCity(0), amount(0) {}
}; // class Transfer

#endif	/* ROUTERRUNNER_H */

