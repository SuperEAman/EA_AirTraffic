// ---------------------------------------------------------
// CapacityTimeData.cc
//
// Source file for CapacityTimeData class
// ---------------------------------------------------------
// 
// This class keeps track of the airport arrival, airport
// departure, and sector capacities over time.
//
// For sectors, the capacity refer to one time slice.  
// Whereas for airport arrival and airport departure, the
// capacity refers to a range of time called a "bin".
// ---------------------------------------------------------

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "utils.h"
#include "CapacityTimeData.h"




//-------------------------------------------------------------
// constructor
//
// The constructor saves a reference to the capacity data for 
// the sectors and airports.  

// To increase efficiency, the data structures for airport and 
// sector capacity over time are not initialized up front but 
// rather as the sector or airport is encountered.
//-------------------------------------------------------------
CapacityTimeData::CapacityTimeData(CapacityData * capacityData)
{
	this->capacityData = capacityData;
    
	for (int i=0; i<MAX_SECTORS; i++)
	{
	   sectorCapacityInitialized[i] = false;
	}
    for (int i=0; i < MAX_AIRPORTS; i++)
	{
	    arrivalCapacityInitialized[i] = false;
	    departureCapacityInitialized[i] = false;
	}
}


//-------------------------------------------------------------
// initializeSectorCapacity
//
// This method sets sector_id's capacity for each time 
// slice to the sector's maximum capacity.
//-------------------------------------------------------------
void CapacityTimeData::initializeSectorCapacity(int sector_id)
{
	if (sector_id > MAX_SECTORS -1)
	{
	   sprintf(ERRMSG, "sector_id greater than MAX");
       pga_exit(__LINE__, __FILE__);
	}

    int sectorCapacity = capacityData->get_sector_capacity(sector_id);
	for (int t=0; t < TIME_SLICES; t++)
	{
		sectorCapacityByTime[sector_id][t] = sectorCapacity;
	}
	sectorCapacityInitialized[sector_id] = true;

	
}

//-------------------------------------------------------------
// initializeAirportArrivalCapacity
//
// This method sets airport_id's arrival capacity for each  
// bin to the airport's maximum arrival capacity.
//-------------------------------------------------------------
void CapacityTimeData::initializeAirportArrivalCapacity(int airport_id)
{
	if (airport_id > MAX_AIRPORTS -1)
	{
	   sprintf(ERRMSG, "airport_id greater than MAX");
       pga_exit(__LINE__, __FILE__);
	}

    int airportArrivalCapacity = capacityData->get_airport_arrival_capacity(airport_id);
	for (int n=0; n < NUM_BINS; n++)
	{
		airportArrivalCapacityByTime[airport_id][n] =  airportArrivalCapacity;
	}
	arrivalCapacityInitialized[airport_id] = true;
}
//-------------------------------------------------------------
// initializeAirportDepartureCapacity
//
// This method sets airport_id's departure capacity for each  
// bin to the airport's maximum departure capacity.
//-------------------------------------------------------------
void CapacityTimeData::initializeAirportDepartureCapacity(int airport_id)
{

	if (airport_id > MAX_AIRPORTS -1)
	{
	   sprintf(ERRMSG, "airport_id greater than MAX");
       pga_exit(__LINE__, __FILE__);
	}

    int airportDepartureCapacity = capacityData->get_airport_departure_capacity(airport_id);
	for (int n=0; n < NUM_BINS; n++)
	{
		airportDepartureCapacityByTime[airport_id][n] =  airportDepartureCapacity;
	}
	departureCapacityInitialized[airport_id] = true;
}

//-------------------------------------------------------------
// sectorOverCapacity
//
// This method returns whether the sector is at capacity for
// the indicated time.
// Returns:
//     true - sector has reached its capacity for that time
//    false - sector has not reached its capacity for that time
//-------------------------------------------------------------
Boolean CapacityTimeData::sectorOverCapacity(int sector_id, int currentTime)
{
	Boolean overCapacity = false;

	if (sector_id > MAX_SECTORS -1)
	{
	   sprintf(ERRMSG, "sector_id greater than MAX");
       pga_exit(__LINE__, __FILE__);
	}
	if (currentTime > TIME_SLICES -1)
	{
		sprintf(ERRMSG, "currentTime (%d) greater than TIME_SLICES (%d)", currentTime, TIME_SLICES);
        pga_exit(__LINE__, __FILE__);
	}

    if (!sectorCapacityInitialized[sector_id])
		initializeSectorCapacity(sector_id);

	if (sectorCapacityByTime[sector_id][currentTime] <=0)
		overCapacity = true;
	
	return (overCapacity);
	
	
}
//-------------------------------------------------------------
// airportDepartureOverCapacity
//
// This method returns whether the airport is at its departure
// capacity for the indicated time.
// Returns:
//     true - airport departures have reached capacity for that time
//    false - airport departures have not reached capacity for that time
//-------------------------------------------------------------
Boolean CapacityTimeData::airportDepartureOverCapacity(int airport_id, int departureTime)
{
	Boolean overCapacity = false;  
	int     binNumber     = getBinNumber(departureTime);

	if (airport_id > MAX_AIRPORTS -1)
	{
	   sprintf(ERRMSG, "airport_id greater than MAX");
       pga_exit(__LINE__, __FILE__);
	}
	
	if (binNumber > NUM_BINS - 1)
	{
		sprintf(ERRMSG, "departureTime is too big");
        pga_exit(__LINE__, __FILE__);
	}
    if (!departureCapacityInitialized[airport_id])
		initializeAirportDepartureCapacity(airport_id);

	if (airportDepartureCapacityByTime[airport_id][binNumber] <=0)
		overCapacity = true;
	
	return (overCapacity);
	
}
//-------------------------------------------------------------
// airportArrivalOverCapacity
//
// This method returns whether the airport is at its arrival
// capacity for the indicated time.
// Returns:
//     true - airport arrivals have reached capacity for that time
//    false - airport arrivals have not reached capacity for that time
//-------------------------------------------------------------
Boolean CapacityTimeData::airportArrivalOverCapacity(int airport_id, int arrivalTime)
{
	Boolean overCapacity = false;
	int     binNumber     = getBinNumber(arrivalTime);

	if (airport_id > MAX_AIRPORTS -1)
	{
	   sprintf(ERRMSG, "airport_id (%d) greater than MAX_AIRPORTS-1 (%d)", airport_id, MAX_AIRPORTS-1);
       pga_exit(__LINE__, __FILE__);
	}
	
	if (binNumber > NUM_BINS - 1)
	{
		sprintf(ERRMSG, "arrivalTime (%d) is too big (binNumber=%d, NUM_BINS=%d)", 
                        arrivalTime, binNumber, NUM_BINS);
        pga_exit(__LINE__, __FILE__);
	}

    if (!arrivalCapacityInitialized[airport_id])
		initializeAirportArrivalCapacity(airport_id);

	if (airportArrivalCapacityByTime[airport_id][binNumber] <= 0)
		overCapacity = true;
	
	return (overCapacity);
	
}
//-------------------------------------------------------------
// decrementSectorCapacity
//
// This method decrements the sector's capacity for the indicated
// time.  If the sector is already full, it will cause the program
// to exit.
//-------------------------------------------------------------
void CapacityTimeData::decrementSectorCapacity(int sector_id, int currentTime)
{
	if (sectorOverCapacity(sector_id, currentTime) )
	{
		sprintf(ERRMSG, "Can not decrement sector capacity. Capacity is used up.");
        pga_exit(__LINE__, __FILE__);
	}
		
	sectorCapacityByTime[sector_id][currentTime]--;
		
}
//-------------------------------------------------------------
// decrementAirportDepartureCapacity
//
// This method decrements the airport's departure capacity for the indicated
// time.  If the airport departure capacity is already used up, it will 
// cause the program to exit.
//-------------------------------------------------------------
void CapacityTimeData::decrementAirportDepartureCapacity(int airport_id, int departureTime)
{
	
	if (airportDepartureOverCapacity(airport_id, departureTime) )
	{
		sprintf(ERRMSG, "Can not decrement airport departure capacity. Capacity is used up.");
        pga_exit(__LINE__, __FILE__);
	}
	
	int binNumber = getBinNumber(departureTime);

	airportDepartureCapacityByTime[airport_id][binNumber]--;
		
}
//-------------------------------------------------------------
// decrementAirportArrivalCapacity
//
// This method decrements the airport's arrival capacity for the indicated
// time.  If the airport arrival capacity is already used up, it will 
// cause the program to exit.
//-------------------------------------------------------------
void CapacityTimeData::decrementAirportArrivalCapacity(int airport_id, int arrivalTime)
{
	if (airportArrivalOverCapacity(airport_id, arrivalTime) )
	{
		sprintf(ERRMSG, "Can not decrement airport arrival capacity. Capacity is used up.");
        pga_exit(__LINE__, __FILE__);
	}
	int binNumber = getBinNumber(arrivalTime);
		
	airportArrivalCapacityByTime[airport_id][binNumber]--;
}
//-------------------------------------------------------------
// getBinNumber
//
// This method returns the bin number associated with the 
// supplied time.  This is needed to determine whether airport
// arrivals and departures are within capacity, since airport
// capacities refer to a "bin" of time instead of a single
// time slice.
//
// The -1 is needed since times 1-15 should be in the first bin
// which is indexed by '0' in the array. ie: (t - 1)/15 is 0
// for times 1-15.  No flights will depart at t=0.  The earlist is
// acData.INITIAL_ITERATION
//-------------------------------------------------------------
int CapacityTimeData::getBinNumber(int time) {

  int binNumber = (time -1)/BIN_SIZE;
   
  return (binNumber);

}

