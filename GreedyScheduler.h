// ---------------------------------------------------------
// GreedyScheduler.h
//
// Header file for the GreedyScheduler class
// ---------------------------------------------------------
// This class greedily schedules a list of flights and 
// returns the delay associated with the schedule.
//----------------------------------------------------------


#ifndef _GreedyScheduler_h_ 
#define _GreedyScheduler_h_ 

#include "Ordering.h"
#include "CapacityData.h"
#include "ACData.h"
#include "CapacityTimeData.h"

class GreedyScheduler
{
public:
	GreedyScheduler(CapacityData *capacityData, ACData *acData );
	~GreedyScheduler(void);
	int greedilySchedule(Ordering ordering, Boolean saveSchedule, int *delayedFlightListSize, delayedFlight_t *delayedFlightList);
	
private:
	int totalGroundDelay;
	int totalAirDelay;

    CapacityData     *capacityData; 
	ACData           *acData;
	CapacityTimeData *capacityTimeData;

	delayedFlight_t  *delayedFlights;
	flight_t         *greedySchedule;

	static const int  ERROR = -1;
	
	static const int  MAX_SECTORS_FOR_FLIGHT      = TIME_SLICES;
	
    void initDwellEntryDelays(int dwellChange[MAX_SECTORS_FOR_FLIGHT], int entryChange[MAX_SECTORS_FOR_FLIGHT]);
	void addGroundDelay(int entry[MAX_SECTORS_FOR_FLIGHT]);
	void addAirDelay(int dwell[MAX_SECTORS_FOR_FLIGHT], int entry[MAX_SECTORS_FOR_FLIGHT], int index);

	void print(int ac_id, int depTime, int arrTime, int numNodes, flightpath_t  flight_path[MAX_FLIGHTPATH_NODES], int dwellChange[MAX_SECTORS_FOR_FLIGHT], int entryChange[MAX_SECTORS_FOR_FLIGHT]);
	void saveScheduledFlightData(int index, int ac_id, int depTime, int arrTime, int numNodes, flightpath_t  flight_path[MAX_FLIGHTPATH_NODES], int dwellChange[MAX_SECTORS_FOR_FLIGHT], int entryChange[MAX_SECTORS_FOR_FLIGHT]);
	void sortDelayedFlights(int numDelayedFlights, int *topDelayedFlightListSize, delayedFlight_t *topDelayedFlights);
};

#endif
