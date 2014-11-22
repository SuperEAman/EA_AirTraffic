// ---------------------------------------------------------
// GreedyScheduler.cc
//
// Source file for GreedyScheduler class
// ---------------------------------------------------------
// This class greedily schedules a list of flights and 
// returns the delay associated with the schedule.
//----------------------------------------------------------

#include <stdio.h>
#include <string.h>
#include "GreedyScheduler.h"
#include "CapacityTimeData.h"
#include "ACData.h"
#include "boolean.h"


//-------------------------------------------------------------------------
// constructor
//-------------------------------------------------------------------------
GreedyScheduler::GreedyScheduler(CapacityData *capacityData, ACData *acData)
{
   
   this->capacityData = capacityData;
//   printf(this->capacityData);

   this->acData = acData;

   this->capacityTimeData = new CapacityTimeData(capacityData);

   greedySchedule = NULL;
   delayedFlights = NULL;
    
}

//-------------------------------------------------------------------------
// destructor
//-------------------------------------------------------------------------
GreedyScheduler::~GreedyScheduler(void)
{
	delete capacityTimeData;

	if (greedySchedule != NULL)
	{
		delete[] greedySchedule;
	}

	if (delayedFlights != NULL)
	{
		delete[] delayedFlights;
	}
	
}
// ------------------------------------------------------------------------
// initDwellEntryDelays initializes the dwellChange and entryChange
// arrays to 0. This method should be called when starting to schedule 
// a flight.  The dwellChange array keeps track of changes to the amount of
// time that a flight must dwell in a sector.  The entryChange array keeps
// track of changes to the time the flight enters each sector.
//-------------------------------------------------------------------------
void GreedyScheduler::initDwellEntryDelays(int dwellChange[MAX_SECTORS_FOR_FLIGHT], int entryChange[MAX_SECTORS_FOR_FLIGHT])
{
	for (int j=0; j<MAX_SECTORS_FOR_FLIGHT; j++)
	{
		dwellChange[j] = 0;
		entryChange[j] = 0;
	}
}
//------------------------------------------------------------------------
// addGroundDelay 
// The entry data structure keeps track of the change in the time
// that a flight can enter a sector.
// When a flight's departure is delayed, the time that it enters
// each of its sectors must also be delayed.
//-------------------------------------------------------------------------
void GreedyScheduler::addGroundDelay(int entry[MAX_SECTORS_FOR_FLIGHT])
{
	for (int i=0; i < MAX_SECTORS_FOR_FLIGHT; i++)
		entry[i] += DELAY_STEP;
}
//-------------------------------------------------------------------------
// addAirDelay
// When a flight is delayed in the air, the dwell time for the sector must be increased
// and the earliest entry time must also be increased for every sector following the sector where the
//delay occurred
//-------------------------------------------------------------------------
void GreedyScheduler::addAirDelay(int dwell[MAX_SECTORS_FOR_FLIGHT], int entry[MAX_SECTORS_FOR_FLIGHT], int index)
{
	// index is the sector where the delay occurs
	for (int i= index+1; i < MAX_SECTORS_FOR_FLIGHT; i++)
		entry[i] += DELAY_STEP;

	dwell[index] += DELAY_STEP;
}
//-------------------------------------------------------------------------
// print
// Prints out the new schedule for a flight in the format of the flight data
// input file.  (INITIAL_ITERATION is not included)
//-------------------------------------------------------------------------
void GreedyScheduler::print(int ac_id, int depTime, int arrTime, int numNodes, flightpath_t  flight_path[MAX_FLIGHTPATH_NODES], int dwellChange[MAX_SECTORS_FOR_FLIGHT], int entryChange[MAX_SECTORS_FOR_FLIGHT])
{
	char *org_airport_str_p, *dst_airport_str_p, *sector_str_p;
	char acName[AC_STRING_LENGTH ];

  
    printf("#####\n");
	acData->get_ac_name(ac_id, acName);
    printf("Flight: %s\n", acName);
    org_airport_str_p = capacityData->get_airport_name(
                       acData->get_origin_airport_id(ac_id));
    printf(" ORG: %s\n", org_airport_str_p);
    dst_airport_str_p = capacityData->get_airport_name(
                       acData->get_destination_airport_id(ac_id));
    printf(" DST: %s\n", dst_airport_str_p);
    printf("  DepartureTime = %d\n", depTime);
    printf("  Nodes %d :\n", numNodes);
	 for (int i=0; i< numNodes; i++)
	 {
      sector_str_p = capacityData->get_sector_name(flight_path[i].sector_id);
      printf("   %s : %d : %d\n", 
             sector_str_p,
             flight_path[i].min_time_in_sector + dwellChange[i], 
             flight_path[i].earliest_entry_time_into_sector + entryChange[i]);
    }
    printf("  ArrivalTime = %d\n", arrTime);
  
}

//-------------------------------------------------------------------------
// saveScheduledFlightData
// This method should only be called if the saveSchedule flag was set to
// true in the constructor and the greedily Schedule method has already
// been called.  
// This method returns the computed greedy schedule or NULL if no schedule
// has been saved.
//-------------------------------------------------------------------------
void GreedyScheduler::saveScheduledFlightData(int index, int ac_id, int depTime, int arrTime, int numNodes, flightpath_t  flight_path[MAX_FLIGHTPATH_NODES], int dwellChange[MAX_SECTORS_FOR_FLIGHT], int entryChange[MAX_SECTORS_FOR_FLIGHT])
{
	char acName[AC_STRING_LENGTH ];
	
	greedySchedule[index].ac_id = ac_id;

	acData->get_ac_name(ac_id, acName);
	strncpy(greedySchedule[index].ac_name, acName, AC_STRING_LENGTH );

	greedySchedule[index].origin_airport_id = acData->get_origin_airport_id(ac_id);
	greedySchedule[index].destination_airport_id = acData->get_destination_airport_id(ac_id);
	greedySchedule[index].departure_time = depTime;
	greedySchedule[index].arrival_time =   arrTime;
	greedySchedule[index].num_sectors  =   numNodes;

	for (int i=0; i< numNodes; i++)
	{
        greedySchedule[index].flightpath[i].sector_id = flight_path[i].sector_id;
	    greedySchedule[index].flightpath[i].min_time_in_sector = flight_path[i].min_time_in_sector + dwellChange[i];
	    greedySchedule[index].flightpath[i].earliest_entry_time_into_sector = flight_path[i].earliest_entry_time_into_sector + entryChange[i];
    }

	  
}

//-------------------------------------------------------------------------
// sortDelayedFlights
//
// This method sorts the list of delayed flights from highest delay to lowest
// delay and returns the resulting list and the number of delayed flights in
// the list.
//
//            --> numDelayedFlights:  The number of flights that were delayed
//                                    for the greedy schedule
//            <-->topDelayedFlightListSize: The maximum number of delayed
//                                    flights to store in the topDelayedFlightList.
//                                    If numDelayedFlights is less, then
//                                    topDelayedFlightListSize will be set to numDelayedFlights
//                                    so it will indicate how many flights are actually stored in
//                                    the list.
//            <-->topDelayedFlightList: Space to store the list of delayed flights.
//
//-------------------------------------------------------------------------
void GreedyScheduler::sortDelayedFlights(int numDelayedFlights, int *topDelayedFlightListSize, delayedFlight_t *topDelayedFlights)
{
	

	if (*topDelayedFlightListSize > numDelayedFlights)
		*topDelayedFlightListSize = numDelayedFlights;

	for (int m=0; m< *topDelayedFlightListSize; m++)
	{
		int highestDelay =0;
	    int highestIndex =0;
		for (int n=0; n< numDelayedFlights; n++)
		{
			if (delayedFlights[n].delay > highestDelay)
			{
				highestDelay = delayedFlights[n].delay;
				highestIndex = n;
			}
		}
		topDelayedFlights[m].ac_id = delayedFlights[highestIndex].ac_id;
		topDelayedFlights[m].delay = delayedFlights[highestIndex].delay;
		topDelayedFlights[m].ordering_index = delayedFlights[highestIndex].ordering_index;

		// Set the delay to -1, so it won't be picked again
		delayedFlights[highestIndex].delay = -1;
	
	}
}
//-------------------------------------------------------------------------
// greedilySchedule
//
// parameters:
//           -> ordering:  The ac_ids to schedule in the given order.
//           -> saveSchedule: A boolean indicating whether or not to save 
//                            and print the schedule
//           <->delayedFlightListSize: The number of top delayed flights that should
//                                     be returned. This size should be no bigger 
//                                     than the number of records in delayedFlightList.
//                                     This parameter will be changed inside the method
//                                     to indicate the actual number of delayed flights
//                                     stored in the list, if the actual number of delayed
//                                     flights is less than the delayedFlightListSize that
//                                     was passed in.                                     less 
//
//           <->delayedFlightList:     Space that the caller must allocate to store the
//                                     delayedFlightList.
//
// returns:  the total delay for the flights for the computed
//           greedy schedule.  Note that airDelay is weighted twice
//           as heavily as groundDelay.
//                or
//           -1 if unable to schedule all of the flights.  The main reason
//              this would happen is if a flight is already enroute, but it
//              can not be accommodated in its initial sector.
//-------------------------------------------------------------------------
int GreedyScheduler::greedilySchedule(Ordering ordering, Boolean saveSchedule, int *delayedFlightListSize, delayedFlight_t *delayedFlightList)
{
	int numFlights;
	int depTime;
	int arrTime;
	int groundDelay;
	int airDelay;
	int currentTime;
	int numSectors;
	int ac_id;
	int origin;
	int dest;
        char acName[AC_STRING_LENGTH];
	
	Boolean departureScheduled;

	flightpath_t  flight_path[MAX_FLIGHTPATH_NODES];

	int dwellChange[MAX_SECTORS];
	int entryChange[MAX_SECTORS];

	int totalGroundDelay = 0;
	int totalAirDelay = 0;

	int numDelayedFlights =0;

/**
 * print input ordering
 */
//					FILE* file = fopen("result.txt","a");
//					ordering.print(file);


	numFlights = ordering.get_number_of_flights();

	// the greedySchedule space is freed in the destructor
	if (saveSchedule)
		greedySchedule = new flight_t[numFlights];

	// Allocate space to save each flight that is delayed.
	// The delayedFlights space is freed in the destructor.
	delayedFlights = new delayedFlight_t[numFlights];
  
	// loop through each flight in the ordering
	// and try to greedily schedule
	for (int i=0; i<numFlights; i++)
	{
		// initialize delays for this flight
		groundDelay    = 0;
		airDelay       = 0;

		// reset dwell and entry changes for this flight to 0
		initDwellEntryDelays(dwellChange, entryChange);
                
                // get the data for this flight
		ac_id = ordering.get_ac_id(i);
		origin = acData->get_origin_airport_id(ac_id);
		dest   = acData->get_destination_airport_id(ac_id);
		
		Boolean flightEnroute = acData->is_flight_enroute(ac_id);
		Boolean flightLand     = acData->will_flight_land(ac_id);
        
		acData->get_flight_path(ac_id, flight_path, &numSectors);

                Boolean good = false;
		Boolean error = false;
		while (!good)
		{
		   good = true;
		   departureScheduled = true;

		   depTime = acData->get_departure_time(ac_id)  + groundDelay;
		   arrTime = acData->get_arrival_time(ac_id)  + groundDelay+airDelay;
		   

		   // If the flight is not already enroute see if the departure
		   // needs to be delayed
		   if (!flightEnroute)
		   {
			   if (capacityTimeData->airportDepartureOverCapacity(origin, depTime))
			   {
			       good               = false;
				   departureScheduled = false;
				   addGroundDelay(entryChange);
				   groundDelay += DELAY_STEP;
			   }
			}
                   // If the flight will land during the simulation
		   // make sure the arrival airport can accommodate it.
		   // If the arrival airport would be over capacity, delay
		   // takeoff if the flight is not already enroute or
		   // delay in the first sector if the flight is enroute.
		   if (departureScheduled && flightLand )
		   {
			  if (capacityTimeData->airportArrivalOverCapacity(dest, arrTime))
			  {
			     good = false;
				 if (!flightEnroute)
				 {
					addGroundDelay(entryChange);
					groundDelay +=DELAY_STEP;
					departureScheduled = false;
				 }
				 else
				 {
					// add air delay in the first sector 
					addAirDelay(dwellChange, entryChange, 0);
					airDelay += DELAY_STEP;
				 }
			  }
		   }// end if departureScheduled and flight lands during the simulation

		   if (departureScheduled)
		   {
              // check that all enroute sectors can accommodate this flight
		      // If departure time is 0, it means that the flight is already
		      // enroute when the simulation starts.
		      if (acData->get_departure_time(ac_id) > 0)
		         currentTime = acData->get_departure_time(ac_id) + 1 + groundDelay;
		      else
			     currentTime = 0;

		      Boolean sectorCapacitiesOk = true;
		      for (int n=0; sectorCapacitiesOk && n<numSectors; n++)
		      {
			      if (currentTime == 0)
				      currentTime = flight_path[n].earliest_entry_time_into_sector;

			      for (int t=0; sectorCapacitiesOk && t< flight_path[n].min_time_in_sector + dwellChange[n]; t++)
			      {
				     if (capacityTimeData->sectorOverCapacity(flight_path[n].sector_id, currentTime + t))
				     {
					    good = false;
						sectorCapacitiesOk = false;
					    if (flightEnroute)
					    {
						   // If an enroute flight can't be accommodated in its initial sector
						   //  there is no possible solution, so return.
						   if (n==0)
						   {
							   error = true;
                                                           acData->get_ac_name(ac_id, acName);
							   return (-1 * ac_id);
						   }
						   else
						   {
							   // Add dwell time to previous sector
							   // enter all following sectors one timestep later.
							   addAirDelay(dwellChange, entryChange, n-1);
							   airDelay += DELAY_STEP;
							   
						   }
					    }
					    else
					    {
						   addGroundDelay(entryChange);
						   groundDelay += DELAY_STEP;
						  
					    }
				    } // end if sector over capacity

			     } // end for each time step in the sector

			   if (sectorCapacitiesOk)
			      currentTime +=  flight_path[n].min_time_in_sector + dwellChange[n];
			   
			 }// end for each node in the flight path
		   }// end if departureScheduled
		} // end while !good

        // Now that we have the right amount of delay for this flight
		// lock in this flight's influence by decrementing appropriate capacities

		// If this flight departs during our time horizon
		// then lock in its departure
		if (!flightEnroute)
		{
			if (capacityTimeData->airportDepartureOverCapacity(origin,depTime))
			{
                                printf("ERROR: greedilySchedule(): logic error: airportDepartureOverCapacity\n");
				return( ERROR);
			}
			else
			{
			   capacityTimeData->decrementAirportDepartureCapacity(origin, depTime);
			}
		}

		// If this flight lands during our time horizon
		//then lock in its arrival
		if (flightLand)
		{
			if (capacityTimeData->airportArrivalOverCapacity(dest,arrTime))
			{
                                printf("ERROR: greedilySchedule(): logic error: airportArrivalOverCapacity\n");
				return( ERROR);
			}
			else
			{
			   capacityTimeData->decrementAirportArrivalCapacity(dest, arrTime);
			}
		}

		// Now lock in all en route influence
		if (acData->get_departure_time(ac_id) >0)
			currentTime = acData->get_departure_time(ac_id) +1 + groundDelay;
		else
			currentTime = 0;

		for (int n=0; n<numSectors; n++)
	    {
	       if (currentTime == 0)
		      currentTime = flight_path[n].earliest_entry_time_into_sector;
		   for (int t=0; t< flight_path[n].min_time_in_sector + dwellChange[n]; t++)
		   {
			   if (capacityTimeData->sectorOverCapacity(flight_path[n].sector_id, currentTime + t))
			   {
                                printf("ERROR: greedilySchedule(): logic error: sectorOverCapacity\n");
                                return (ERROR);
			   }
			   else
			   {
				   capacityTimeData->decrementSectorCapacity(flight_path[n].sector_id, currentTime + t);
			   }

		   }
		   //Set up currentTime for next sector
		   currentTime += flight_path[n].min_time_in_sector + dwellChange[n];
		}

		if (groundDelay + airDelay > 0)
		{
			totalGroundDelay += groundDelay;
			totalAirDelay += airDelay;
		}

		if (saveSchedule && greedySchedule !=NULL)
		{
			saveScheduledFlightData(i, ac_id, depTime, arrTime, numSectors, flight_path, dwellChange, entryChange);
		}

		if (groundDelay > 0 || airDelay > 0)
		{
		   // Save the delay for this flight id
                   delayedFlights[numDelayedFlights].ac_id  = ac_id;
		   delayedFlights[numDelayedFlights].delay  = groundDelay + 2*airDelay;
		   delayedFlights[numDelayedFlights].ordering_index = i;
		   numDelayedFlights++;
		}

		// print the new schedule for this flight
   	    //print(ac_id, depTime, arrTime, numSectors, flight_path, dwellChange, entryChange);


	}// end for each flight in the Ordering
    
    if (saveSchedule && greedySchedule != NULL)
	{
		// convert the greedy Schedule to an acData object and then
		// print the schedule to a file
		ACData newACData(greedySchedule,numFlights, acData->get_initial_iteration());
                //FILE *fp = fopen("schedule.out", "w");
		//newACData.print(fp, capacityData);
		newACData.print(stdout, capacityData);
                //fclose(fp);

	}

	// Sort the delayed flights and return the flights with the most delay
	sortDelayedFlights(numDelayedFlights, delayedFlightListSize, delayedFlightList);

	// Return the total delay for this greedy schedule.
	// Note that the airdelay is doubled since air delay is less
	// desirable than ground delay.
	return( totalGroundDelay + 2*totalAirDelay);
}
