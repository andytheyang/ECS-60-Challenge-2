// Author: Andy Yang and Manvinder Sodhi

#include "router.h"
#include "RouterRunner.h"
#include "StackAr.h"
#include <iostream>
#include <algorithm>

using namespace std;

Router::Router(CityInfo *info, int num) : cities(info), numCities(num)
{
    // TODO: duplicate info??
} // Router()


int Router::setTransfers(Transfer **transfers)
{
    int numTransfer = 0;
    StackAr<int> surplus(25000);
    StackAr<int> starved(25000);
    
    for (int i = 0; i < numCities; i++)
    {
        if (getNet(i) > 0)
            surplus.push(i);
    }  // for all cities
    
    while (!surplus.isEmpty())	// while there are more surplus cities
    {
        int current = surplus.topAndPop();
        //    cout << current << endl;
        CityInfo *curCity = &(cities[current]);
        
        // TODO: change <= to !=
        for (int i = 0; i < curCity->adjCount && getNet(current) >= 0; i++)
        {
            if (getNet(curCity->adjList[i]) > 0)	// is a surplus
                continue;
            
            // transfer maximum possible
            int transferAmount = min(getNet(current), -getNet(curCity->adjList[i]));
            numTransfer += transferAmount;
            transfer(transfers, current, i, transferAmount);
            // TODO: implement multicity traversals
        }  // for all adjacencies
    }  // while
    
    for (int i = 0; i < numCities; i++)
    {
        if (getNet(i) > 0)
            surplus.push(i);
    }  // in case there are any more cities with more power left
    
    for (int i = 0; i < numCities; i++)
    {
        if (getNet(i) < 0)
            starved.push(i);
    }  // for all cities that need power or are starved.
    
    while (!starved.isEmpty()) {
        cout<< "Starved: " << starved.topAndPop()<< endl;
        cout<< "Surplus: " << surplus.topAndPop()<< endl;
        // Need a min flow network from surplus city to starved city using the adjacency list
        
        
    }   // while there are still starved cities left.
    
    return numTransfer;  // should be set to total of all transfers.
}  // setTransfers

// ---PRIVATE---

int Router::getNet(int city) const
{
    return cities[city].production - cities[city].usage;
}  // getNet()

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
        //    cout << "making new transfer to " << adj->destCity << endl;
    }
    
    return adj;
}  // getTransfer()

void Router::printCities() const
{
    for (int i = 0; i < numCities; i++)
    {
        cout << "City " << i << " nets " << getNet(i) << endl;
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
