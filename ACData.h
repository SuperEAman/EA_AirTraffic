//---------------------------------------------------------
// ACData.h
//
// Header file for ACData class
//---------------------------------------------------------

#ifndef _ACData_h_
#define _ACData_h_

#include "boolean.h"
#include "CapacityData.h"

//---------------------------
// constants
//---------------------------
const int  AC_STRING_LENGTH      = 32;
const int  AIRPORT_STRING_LENGTH = 10;
const int  MAX_FLIGHTPATH_NODES  = 25;
const int  SECTOR_STRING_LENGTH  = 10;

//---------------------------
// types
//---------------------------
typedef struct flightpath_struct {
  int   sector_id;
  int   min_time_in_sector;
  int   earliest_entry_time_into_sector; 
} flightpath_t;

typedef struct flight_struct {
  int           ac_id;
  char          ac_name[AC_STRING_LENGTH];
  int           origin_airport_id;
  int           destination_airport_id;
  int           departure_time;
  int           arrival_time;
  int           num_sectors;
  flightpath_t  flightpath[MAX_FLIGHTPATH_NODES];
} flight_t;

//---------------------------
// classes
//---------------------------
class ACData {

public:
        ACData(char *, CapacityData *);
        ACData(flight_t * flightData, int numFlights, int initialIteration );
  void  print(FILE*, CapacityData *);
  void  sort_flights_by_earliest_departure();
  void  sort_flights_by_latest_departure();
  void  sort_flights_by_earliest_arrival();
  void  sort_flights_by_latest_arrival();
  void  get_number_of_flights(char *);
  void  get_number_of_flights_from_acdata_file(char *);
  void  load_ACData(char *, CapacityData *);
  void  load_ACData_from_file(char *, CapacityData *);
  inline int get_number_of_enroute_flights(){return(num_enroute_flights);}
  inline int get_number_of_flights(){return(num_flights);}
  inline int get_ac_id_given_index(int i){return(acdata_array[i].ac_id);}
  inline int get_initial_iteration(){return(initial_iteration);}
  inline int get_origin_airport_id(int i) {return(acdata_array[i].origin_airport_id);}
  inline int get_destination_airport_id(int i) {return(acdata_array[i].destination_airport_id);}
  inline int get_departure_time(int i) {return(acdata_array[i].departure_time);}
  inline int get_arrival_time(int i) {return(acdata_array[i].arrival_time);}
  void       get_ac_name(int i, char ac_name[AC_STRING_LENGTH]);
  void       get_flight_path(int i, flightpath_t  flight_path[MAX_FLIGHTPATH_NODES], int * num_nodes);
  Boolean    is_flight_enroute(int i);
  Boolean    will_flight_land(int i);


private:
  int       num_flights;
  int       num_enroute_flights;
  int       initial_iteration;
  flight_t  *acdata_array;
};

//---------------------------
// prototypes
//---------------------------

#endif
