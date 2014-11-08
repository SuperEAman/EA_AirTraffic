//---------------------------------------------------------
// ACData.cc
//
// Source file for ACData and ACData_array objects
//---------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ACData.h"
#include "utils.h"

// {{{ comparison functions used by qsort

//---------------------------------------------------
int
compare_descend_arrival_time(const void *a, const void *b) {

  flight_t  *flight_p1, *flight_p2;
  int       arrival_time1, arrival_time2;

  flight_p1 = (flight_t *)a;
  flight_p2 = (flight_t *)b;
  arrival_time1 = flight_p1->arrival_time;
  arrival_time2 = flight_p2->arrival_time;

  if (arrival_time1 > arrival_time2)
    return -1;
  else if (arrival_time1 < arrival_time2)
    return 1;
  else
    return 0;
}

//---------------------------------------------------
int
compare_ascend_arrival_time(const void *a, const void *b) {

  flight_t  *flight_p1, *flight_p2;
  int       arrival_time1, arrival_time2;

  flight_p1 = (flight_t *)a;
  flight_p2 = (flight_t *)b;
  arrival_time1 = flight_p1->arrival_time;
  arrival_time2 = flight_p2->arrival_time;

  if (arrival_time1 < arrival_time2)
    return -1;
  else if (arrival_time1 > arrival_time2)
    return 1;
  else
    return 0;
}

//---------------------------------------------------
int
compare_descend_departure_time(const void *a, const void *b) {

  flight_t  *flight_p1, *flight_p2;
  int       departure_time1, departure_time2;

  flight_p1 = (flight_t *)a;
  flight_p2 = (flight_t *)b;
  departure_time1 = flight_p1->departure_time;
  departure_time2 = flight_p2->departure_time;

  if (departure_time1 > departure_time2)
    return -1;
  else if (departure_time1 < departure_time2)
    return 1;
  else
    return 0;
}

//---------------------------------------------------
int
compare_ascend_departure_time(const void *a, const void *b) {

  flight_t  *flight_p1, *flight_p2;
  int       departure_time1, departure_time2;

  flight_p1 = (flight_t *)a;
  flight_p2 = (flight_t *)b;
  departure_time1 = flight_p1->departure_time;
  departure_time2 = flight_p2->departure_time;

  if (departure_time1 < departure_time2)
    return -1;
  else if (departure_time1 > departure_time2)
    return 1;
  else
    return 0;
}

// }}}

// {{{ method: constructor

ACData::ACData(char *buffer, CapacityData *capacity_data) {

  int i;

  printf("ACData constructor called\n");

  // find out how many flights we need to work with
  get_number_of_flights(buffer);

  // allocate space for the data array
  acdata_array = new flight_t[num_flights];

  // load in the data from the buffer
  load_ACData(buffer, capacity_data);

  // compute number of enroute flights
  num_enroute_flights = 0;
  for (i=0; i<num_flights; i++){
    if (is_flight_enroute(i))
      num_enroute_flights++;
  }

  //compute_number_of_enroute_flights();
  printf("ACData constructor: num_flights = %d\n", num_flights);
  printf("ACData constructor: num_enroute_flights = %d\n", num_enroute_flights);

}

// }}}
// {{{ method: constructor

//----------------------------------------------------------------------
// overload of Constructor
// Puts scheduled flights in an ACData object, so the schedule
// can be printed out to a file in the same format that it is read in.
//----------------------------------------------------------------------
ACData::ACData(flight_t * flightData, int numFlights, int initialIteration){

  num_flights       = numFlights;
  initial_iteration = initialIteration;

  // allocate space for the data array and copy supplied data
  acdata_array = new flight_t[num_flights];
  memcpy(acdata_array, flightData, numFlights*sizeof(flight_t));

}

// }}}
// {{{ method: print()

void
ACData::print(FILE* fp, CapacityData *capacity_data){

  char *org_airport_str_p, *dst_airport_str_p, *sector_str_p;

  fprintf(fp, "[printing ACData, formatted as in the input file]\n");

  fprintf(fp, "Number of flights: %d\n", num_flights);
  fprintf(fp, "INITIAL_ITERATION %d\n", initial_iteration);

  for (int ac_id=0; ac_id<num_flights; ac_id++){
    fprintf(fp, "#####\n");
    fprintf(fp, "Flight: %s\n", acdata_array[ac_id].ac_name);
    org_airport_str_p = capacity_data->get_airport_name(
                       acdata_array[ac_id].origin_airport_id);
    fprintf(fp, " ORG: %s\n", org_airport_str_p);
    dst_airport_str_p = capacity_data->get_airport_name(
                       acdata_array[ac_id].destination_airport_id);
    fprintf(fp, " DST: %s\n", dst_airport_str_p);
    fprintf(fp, "  DepartureTime = %d\n", acdata_array[ac_id].departure_time);
    fprintf(fp, "  Nodes %d :\n", acdata_array[ac_id].num_sectors);
    for (int i=0; i<acdata_array[ac_id].num_sectors; i++){
      sector_str_p = capacity_data->get_sector_name(
                                 acdata_array[ac_id].flightpath[i].sector_id);
      fprintf(fp, "   %s : %d : %d\n", 
             sector_str_p,
             acdata_array[ac_id].flightpath[i].min_time_in_sector, 
             acdata_array[ac_id].flightpath[i].earliest_entry_time_into_sector);
    }
    fprintf(fp, "  ArrivalTime = %d\n", acdata_array[ac_id].arrival_time);
  }
}

// }}}
// {{{ method: sort_flights_by_earliest_departure

void
ACData::sort_flights_by_earliest_departure(){

  //WARNING: this reorders the acdata_array so that the indexes 
  //         are no longer ac_id numbers.  This should only be 
  //         used on copies of the original ACData input files.

  qsort(acdata_array, num_flights, sizeof(flight_t), 
        compare_ascend_departure_time);

}

// }}}
// {{{ method: sort_flights_by_latest_departure

void
ACData::sort_flights_by_latest_departure(){

  //WARNING: this reorders the acdata_array so that the indexes 
  //         are no longer ac_id numbers.  This should only be 
  //         used on copies of the original ACData input files.

  qsort(acdata_array, num_flights, sizeof(flight_t), 
        compare_descend_departure_time);

}

// }}}
// {{{ method: sort_flights_by_earliest_arrival

void
ACData::sort_flights_by_earliest_arrival(){

  //WARNING: this reorders the acdata_array so that the indexes 
  //         are no longer ac_id numbers.  This should only be 
  //         used on copies of the original ACData input files.

  qsort(acdata_array, num_flights, sizeof(flight_t), 
        compare_ascend_arrival_time);
}

// }}}
// {{{ method: sort_flights_by_latest_arrival

void
ACData::sort_flights_by_latest_arrival(){

  //WARNING: this reorders the acdata_array so that the indexes 
  //         are no longer ac_id numbers.  This should only be 
  //         used on copies of the original ACData input files.

  qsort(acdata_array, num_flights, sizeof(flight_t), 
        compare_descend_arrival_time);

}

// }}}
// {{{ method: get_number_of_flights

void
ACData::get_number_of_flights(char *buffer){
  
  const int  LINE_LENGTH  = 256;  
  char       line[LINE_LENGTH], field[LINE_LENGTH];

  // count the number of flights
  // also sneak in pulling out INITIAL_ITERATION
  num_flights = 0;

  while (*buffer != '\0'){
    buffer = get_line_from_buffer(buffer, line);
    if ((line[0] == 'F')&&(line[1] == 'l')&&(line[2] == 'i')&&
        (line[3] == 'g')&&(line[4] == 'h')&&(line[5] == 't')){
      num_flights++;
    }
    if ((line[0] == 'I')&&(line[1] == 'N')&&(line[2] == 'I')&&
        (line[3] == 'T')&&(line[4] == 'I')&&(line[5] == 'A')&&
        (line[6] == 'L')&&(line[7] == '_')&&(line[8] == 'I')){
      sscanf(line, "%s%i", field, &initial_iteration);
    }
  }
}

// }}}

// {{{ method: get_number_of_flights_from_acdata_file

void
ACData::get_number_of_flights_from_acdata_file(char *flights_filename){
  
  const int  LINE_LENGTH  = 256;  
  char       line[LINE_LENGTH], field[LINE_LENGTH];

  FILE *flight_file_p = fopen(flights_filename, "r");
  if (!flight_file_p){
    sprintf(ERRMSG, "could not open %s", flights_filename);
    exit(1);
  }

  // count the number of flights
  // also sneak in pulling out INITIAL_ITERATION
  num_flights = 0;
  while (fgets(line, LINE_LENGTH, flight_file_p) ){
    if ((line[0] == 'F')&&(line[1] == 'l')&&(line[2] == 'i')&&
        (line[3] == 'g')&&(line[4] == 'h')&&(line[5] == 't')){
      num_flights++;
    }
    if ((line[0] == 'I')&&(line[1] == 'N')&&(line[2] == 'I')&&
        (line[3] == 'T')&&(line[4] == 'I')&&(line[5] == 'A')&&
        (line[6] == 'L')&&(line[7] == '_')&&(line[8] == 'I')){
      sscanf(line, "%s%i", field, &initial_iteration);
    }
  }

  fclose(flight_file_p);
}

// }}}
// {{{ method: load_ACData

void
ACData::load_ACData(char *buffer, CapacityData *capacity_data){

  const int  LINE_LENGTH  = 256;  
  const int  FIELD_LENGTH = 64;  
  char       keyword[FIELD_LENGTH], field1[FIELD_LENGTH], field2[FIELD_LENGTH], 
             ac_name[FIELD_LENGTH], 
             org_airport[FIELD_LENGTH], dest_airport[FIELD_LENGTH], 
             sector_name[LINE_LENGTH], line[LINE_LENGTH];
  int        k, line_number=0, ac_id=0, 
             departure_time=0, arrival_time=0, nodes=0, 
             min_time_in_sector=0, earliest_entry_time_into_sector=0;

  if (!buffer){
    sprintf(ERRMSG, "empty ACData buffer");
    exit(0);
  }

  //------------------------------------------------------------
  // read the flights into acdata_array
  //------------------------------------------------------------
  while (*buffer != '\0'){
    buffer = get_line_from_buffer(buffer, line);
    line_number++;
    if ((line[0] != '#')&&(line[0] != '\n')){
      sscanf(line, "%s", keyword);
      if (!strcmp("Flight:", keyword)){
        sscanf(line, "%s%s", keyword, ac_name);
        sprintf(acdata_array[ac_id].ac_name, "%s", ac_name);
        //sprintf(acdata_temp_array[ac_id].ac_name, "%s", ac_name);
        acdata_array[ac_id].ac_id = ac_id;
        //acdata_temp_array[ac_id].ac_id = ac_id;
        //printf("Flight: %s\n", ac_name);
      }
      else {
        if (!strcmp("ORG:", keyword)){
          sscanf(line, "%s%s", keyword, org_airport);
          acdata_array[ac_id].origin_airport_id = 
            capacity_data->get_airport_id(org_airport);
          //acdata_temp_array[ac_id].origin_airport_id = 
          //                         capacity_data->get_airport_id(org_airport);
          //sprintf(ACData_array[ac_id].origin, "%s", org_airport);
          //printf(" ORG: %s\n", org_airport);
        }
        else {
          if (!strcmp("DST:", keyword)){
            sscanf(line, "%s%s", keyword, dest_airport);
            acdata_array[ac_id].destination_airport_id =
              capacity_data->get_airport_id(dest_airport);
            //acdata_temp_array[ac_id].destination_airport_id =
            //                       capacity_data->get_airport_id(dest_airport);
            //sprintf(ACData_array[ac_id].destination, "%s", dest_airport);
            //printf(" DST: %s\n", dest_airport);
          }
          else {
            if (!strcmp("DepartureTime", keyword)){
              sscanf(line, "%s%s%i", keyword, field1, &departure_time);
              acdata_array[ac_id].departure_time = departure_time;
              //acdata_temp_array[ac_id].departure_time = departure_time;
              //printf("  DepartureTime = %d\n", departure_time);
            }
            else {
              if (!strcmp("Nodes", keyword)){
                sscanf(line, "%s%i%s", keyword, &nodes, field1);
                acdata_array[ac_id].num_sectors = nodes;
                //acdata_temp_array[ac_id].num_sectors = nodes;
                //printf("  Nodes %d :\n", nodes);
                if (nodes > MAX_FLIGHTPATH_NODES){
                  sprintf(ERRMSG, 
                          "need to recompile with MAX_FLIGHTPATH_NODES = %d", nodes);
                  exit(1);
                }
                for (k=0; k<nodes; k++){
                  buffer = get_line_from_buffer(buffer, line);
                  sscanf(line, "%s%s%i%s%i", sector_name, field1, 
                         &min_time_in_sector, field2, 
                         &earliest_entry_time_into_sector);
                  //sprintf(ACData_array[ac_id].flightpath[k].sector_name, "%s", 
                  //        sector_name);
                  acdata_array[ac_id].flightpath[k].sector_id = 
                    capacity_data->get_sector_id(sector_name);
                  //acdata_temp_array[ac_id].flightpath[k].sector_id = 
                  //           capacity_data->get_sector_id(sector_name);
                  acdata_array[ac_id].flightpath[k].min_time_in_sector = 
                    min_time_in_sector;
                  //acdata_temp_array[ac_id].flightpath[k].min_time_in_sector = 
                  //           min_time_in_sector;
                  acdata_array[ac_id].flightpath[k].earliest_entry_time_into_sector = earliest_entry_time_into_sector;
                  //acdata_temp_array[ac_id].flightpath[k].earliest_entry_time_into_sector = earliest_entry_time_into_sector;
                  //printf("   %s : %d : %d\n", sector_name, min_time_in_sector, 
                  //       earliest_entry_time_into_sector);
                }
              }
              else {
                if (!strcmp("ArrivalTime", keyword)){
                  sscanf(line, "%s%s%i", keyword, field1, &arrival_time);
                  acdata_array[ac_id].arrival_time = arrival_time;
                  //acdata_temp_array[ac_id].arrival_time = arrival_time;
                  //printf("  ArrivalTime = %d\n", arrival_time);
                  ac_id++;
                }
              }
            }
          }
        }
      }
    }
  }
}

// }}}
// {{{ method: load_ACData_from_file

void
ACData::load_ACData_from_file(char *flights_filename, CapacityData *capacity_data){

  const int  LINE_LENGTH  = 256;  
  const int  FIELD_LENGTH = 64;  
  char       keyword[FIELD_LENGTH], field1[FIELD_LENGTH], field2[FIELD_LENGTH], 
             ac_name[FIELD_LENGTH], 
             org_airport[FIELD_LENGTH], dest_airport[FIELD_LENGTH], 
             sector_name[LINE_LENGTH], line[LINE_LENGTH];
  int        i, line_number=0, ac_id=0, 
             departure_time=0, arrival_time=0, nodes=0, 
             min_time_in_sector=0, earliest_entry_time_into_sector=0;

  FILE *flight_file_p = fopen(flights_filename, "r");
  if (!flight_file_p){
    sprintf(ERRMSG, "could not open %s", flights_filename);
    exit(1);
  }

  //------------------------------------------------------------
  // read the flights into acdata_array
  //------------------------------------------------------------
  while (fgets(line, LINE_LENGTH, flight_file_p)){
    line_number++;
    if ((line[0] != '#')&&(line[0] != '\n')){
      sscanf(line, "%s", keyword);
      if (!strcmp("Flight:", keyword)){
        sscanf(line, "%s%s", keyword, ac_name);
        sprintf(acdata_array[ac_id].ac_name, "%s", ac_name);
        //sprintf(acdata_temp_array[ac_id].ac_name, "%s", ac_name);
        acdata_array[ac_id].ac_id = ac_id;
        //acdata_temp_array[ac_id].ac_id = ac_id;
        //printf("Flight: %s\n", ac_name);
      }
      else {
        if (!strcmp("ORG:", keyword)){
          sscanf(line, "%s%s", keyword, org_airport);
          acdata_array[ac_id].origin_airport_id = 
                                   capacity_data->get_airport_id(org_airport);
          //acdata_temp_array[ac_id].origin_airport_id = 
          //                         capacity_data->get_airport_id(org_airport);
          //sprintf(ACData_array[ac_id].origin, "%s", org_airport);
          //printf(" ORG: %s\n", org_airport);
        }
        else {
          if (!strcmp("DST:", keyword)){
            sscanf(line, "%s%s", keyword, dest_airport);
            acdata_array[ac_id].destination_airport_id =
                                   capacity_data->get_airport_id(dest_airport);
            //acdata_temp_array[ac_id].destination_airport_id =
            //                       capacity_data->get_airport_id(dest_airport);
            //sprintf(ACData_array[ac_id].destination, "%s", dest_airport);
            //printf(" DST: %s\n", dest_airport);
          }
          else {
            if (!strcmp("DepartureTime", keyword)){
              sscanf(line, "%s%s%i", keyword, field1, &departure_time);
              acdata_array[ac_id].departure_time = departure_time;
              //acdata_temp_array[ac_id].departure_time = departure_time;
              //printf("  DepartureTime = %d\n", departure_time);
            }
            else {
              if (!strcmp("Nodes", keyword)){
                sscanf(line, "%s%i%s", keyword, &nodes, field1);
                acdata_array[ac_id].num_sectors = nodes;
                //acdata_temp_array[ac_id].num_sectors = nodes;
                //printf("  Nodes %d :\n", nodes);
                if (nodes > MAX_FLIGHTPATH_NODES){
                  sprintf(ERRMSG, 
                       "need to recompile with MAX_FLIGHTPATH_NODES = %d", nodes);
                  exit(1);
                }
                for (i=0; i<nodes; i++){
                  fgets(line, LINE_LENGTH, flight_file_p);
                  sscanf(line, "%s%s%i%s%i", sector_name, field1, 
                         &min_time_in_sector, field2, 
                         &earliest_entry_time_into_sector);
                  //sprintf(ACData_array[ac_id].flightpath[i].sector_name, "%s", 
                  //        sector_name);
                  acdata_array[ac_id].flightpath[i].sector_id = 
                             capacity_data->get_sector_id(sector_name);
                  //acdata_temp_array[ac_id].flightpath[i].sector_id = 
                  //           capacity_data->get_sector_id(sector_name);
                  acdata_array[ac_id].flightpath[i].min_time_in_sector = 
                             min_time_in_sector;
                  //acdata_temp_array[ac_id].flightpath[i].min_time_in_sector = 
                  //           min_time_in_sector;
                  acdata_array[ac_id].flightpath[i].earliest_entry_time_into_sector = earliest_entry_time_into_sector;
                  //acdata_temp_array[ac_id].flightpath[i].earliest_entry_time_into_sector = earliest_entry_time_into_sector;
                  //printf("   %s : %d : %d\n", sector_name, min_time_in_sector, 
                  //        earliest_entry_time_into_sector);
                }
              }
              else {
                if (!strcmp("ArrivalTime", keyword)){
                  sscanf(line, "%s%s%i", keyword, field1, &arrival_time);
                  acdata_array[ac_id].arrival_time = arrival_time;
                  //acdata_temp_array[ac_id].arrival_time = arrival_time;
                  //printf("  ArrivalTime = %d\n", arrival_time);
                  ac_id++;
                }
              }
            }
          }
        }
      }
    }
  }

  fclose(flight_file_p);

}

// }}}

// {{{ method: get_ac_name

void 
ACData::get_ac_name(int i, char ac_name[AC_STRING_LENGTH]) {
	strncpy(ac_name, acdata_array[i].ac_name, AC_STRING_LENGTH);
	
}

// }}}
// {{{ method: get_flight_path

void 
ACData::get_flight_path(int i, flightpath_t  flight_path[MAX_FLIGHTPATH_NODES], int * num_nodes){

  *num_nodes = acdata_array[i].num_sectors;
  
  for (int n=0; n<acdata_array[i].num_sectors; n++) {
    flight_path[n].sector_id = acdata_array[i].flightpath[n].sector_id;
    flight_path[n].min_time_in_sector = acdata_array[i].flightpath[n].min_time_in_sector;
    flight_path[n].earliest_entry_time_into_sector = acdata_array[i].flightpath[n].earliest_entry_time_into_sector;
    
  }
}

// }}}
// {{{ method: is_flight_enroute

Boolean 
ACData::is_flight_enroute(int i){
  if (acdata_array[i].departure_time == 0)
    return true;
  else
    return false;
}

// }}}
// {{{ method: will_flight_land

Boolean 
ACData::will_flight_land(int i) {
  if (acdata_array[i].arrival_time != 0)
    return true;
  else
    return false;
  
}

// }}}


