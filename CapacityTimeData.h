// ---------------------------------------------------------
// CapacityTimeData.h
//
// Header file for CapacityTimeData class
//----------------------------------------------------------
// This class keeps track of the airport arrival, airport
// departure, and sector capacities over time.
//
// For sectors, the capacity refer to one time slice.  
// Whereas for airport arrival and airport departure, the
// capacity refers to a range of time called a "bin".
// ---------------------------------------------------------

#ifndef _CapacityTimeData_h_
#define _CapacityTimeData_h_

#include "CapacityData.h"
#include "boolean.h"

//---------------------------
// constants
//---------------------------
const int  TIME_SLICES      = 900;
const int  BIN_SIZE         = 15;
const int  NUM_BINS         = TIME_SLICES/BIN_SIZE;
const int  MAX_SECTORS      = 2000;
const int  MAX_AIRPORTS     = 2000;

const int DELAY_STEP        = 1;


//---------------------------
// classes
//---------------------------
class CapacityTimeData {

public:
	CapacityTimeData(CapacityData * capacityData);

	Boolean sectorOverCapacity(int sector_id, int currentTime);
	Boolean airportDepartureOverCapacity(int airport_id, int departureTime);
	Boolean airportArrivalOverCapacity(int airport_id, int arrivalTime);
	
	void decrementSectorCapacity(int sector_id, int currentTime);
	void decrementAirportDepartureCapacity(int airport_id, int departureTime);
	void decrementAirportArrivalCapacity(int airport_id, int arrivalTime);
	

protected:
private:
	int sectorCapacityByTime[MAX_SECTORS][TIME_SLICES];
	int airportArrivalCapacityByTime[MAX_AIRPORTS][NUM_BINS];
	int airportDepartureCapacityByTime[MAX_AIRPORTS][NUM_BINS];

	Boolean sectorCapacityInitialized[MAX_SECTORS];
	Boolean arrivalCapacityInitialized[MAX_AIRPORTS];
	Boolean departureCapacityInitialized[MAX_AIRPORTS];

	CapacityData *capacityData;

    void initializeSectorCapacity(int sector_id);
	void initializeAirportArrivalCapacity(int airport_id);
	void initializeAirportDepartureCapacity(int airport_id);

	int  getBinNumber(int time);
	
};

#endif
