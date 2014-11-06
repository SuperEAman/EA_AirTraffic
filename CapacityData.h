// ---------------------------------------------------------
// CapacityData.h
//
// Header file for CapacityData class
// ---------------------------------------------------------

#ifndef _CapacityData_h_
#define _CapacityData_h_

//---------------------------
// constants
//---------------------------
const int SECTOR_NAME_STRING_LENGTH = 10;

//---------------------------
// types
//---------------------------
typedef struct sector_data_struct {
  char  sector_name[SECTOR_NAME_STRING_LENGTH];
  int   sector_capacity;
} sector_data_t;

typedef struct airport_arrival_data_struct {
  char  airport_name[SECTOR_NAME_STRING_LENGTH];
  int   airport_arrival_capacity;
} airport_arrival_data_t;

typedef struct airport_departure_data_struct {
  char  airport_name[SECTOR_NAME_STRING_LENGTH];
  int   airport_departure_capacity;
} airport_departure_data_t;

//---------------------------
// classes
//---------------------------
class CapacityData {

public:
         CapacityData(char *);
  void   print();
  void   get_node_counts(char *);
  void   get_node_counts_from_capacity_file(char *);
  void   load_CapacityData(char *);
  void   load_CapacityData_from_file(char *);
  int    get_sector_id(char *);
  char * get_sector_name(int);
  int    get_airport_id(char *);
  char * get_airport_name(int);
  int    get_sector_capacity(int);
  int    get_airport_departure_capacity(int);
  int    get_airport_arrival_capacity(int);

  inline int get_number_of_sectors(){return(num_sectors);}
  inline int get_number_of_airports(){return(num_airports);}

private:
  int                       num_sectors;
  int                       num_airports;
  sector_data_t             *sector_data_array;
  airport_departure_data_t  *airport_departure_data_array;
  airport_arrival_data_t    *airport_arrival_data_array;
};

//---------------------------
// prototypes
//---------------------------

#endif
