// ---------------------------------------------------------
// CapacityData.cc
//
// Source file for CapacityData object
// ---------------------------------------------------------
// 
// For speed, we lookup capacity data using ID numbers
// such as sector_id and airport_id.  These are simply
// the indexes into the three main arrays (sectors, 
// airport departure, airport arrival).  These ID numbers
// are used in the ACData object.
//
// The only time we should need to lookup an ID (int) given 
// a name (string) is for input/output purposes.  That lookup
// code is slow so it should only be used for input/output.
//
// ---------------------------------------------------------

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "CapacityData.h"
#include "utils.h"

// {{{ method: constructor 

CapacityData::CapacityData(char *capacity_buffer) {

  printf("CapacityData constructor called\n");

  // get the number of sectors and airports
  get_node_counts(capacity_buffer);

  // allocate space for the data arrays
  sector_data_array            = new sector_data_t[num_sectors];
  airport_departure_data_array = new airport_departure_data_t[num_airports];
  airport_arrival_data_array   = new airport_arrival_data_t[num_airports];

  // load the data into the arrays
  load_CapacityData(capacity_buffer);

}

// }}}
// {{{ method: print()

void
CapacityData::print(){
    printf("CapacityData object:\n");
    printf("  num_sectors: %d\n", num_sectors);
    printf("  num_airports: %d\n", num_airports);
    for (int i=0; i<num_sectors; i++){
      //printf("%s %d (sector_id: %d)\n", sector_data_array[i].sector_name, 
      //                                  sector_data_array[i].sector_capacity, i);
      printf("%s %d\n", sector_data_array[i].sector_name, sector_data_array[i].sector_capacity);
    }
    for (int i=0; i<num_airports; i++){
      //printf("%s %d %d (airport_id: %d)\n", airport_departure_data_array[i].airport_name, 
      //                     airport_departure_data_array[i].airport_departure_capacity,
      //                     airport_arrival_data_array[i].airport_arrival_capacity, i);
      printf("%s %d %d\n", airport_departure_data_array[i].airport_name, 
                           airport_departure_data_array[i].airport_departure_capacity,
                           airport_arrival_data_array[i].airport_arrival_capacity);
    }
}

// }}}
// {{{ method: get_sector_id()

int
CapacityData::get_sector_id(char *sector_name){

  //WARNING: this should only be called for input/output purposes
  //         as it is slow.  During main processing loop, strings
  //         should not be used - use sector_id and airport_id instead

  int sector_id=0;

  //given a sector name string, return the sector_id 
  while (strcmp(sector_data_array[sector_id].sector_name, sector_name)&&
         (sector_id<num_sectors))
    sector_id++;

  if (sector_id == num_sectors){
    sprintf(ERRMSG, "could not find sector_id for %s", sector_name);
    pga_exit(__LINE__, __FILE__);
  }

  return(sector_id);
}

// }}}
// {{{ method: get_sector_name()

char *
CapacityData::get_sector_name(int sector_id){

  if (sector_id >= num_sectors){
    sprintf(ERRMSG, "invalid sector_id %d", sector_id);
    pga_exit(__LINE__, __FILE__);
  }

  return(sector_data_array[sector_id].sector_name);
}

// }}}
// {{{ method: get_airport_id()

int
CapacityData::get_airport_id(char *airport_name){

  //WARNING: this should only be called for input/output purposes
  //         as it is slow.  During main processing loop, strings
  //         should not be used - use sector_id and airport_id instead

  int airport_id=0;

  //given an airport name string, return the airport_id 
  while (strcmp(airport_departure_data_array[airport_id].airport_name, airport_name)&&
         (airport_id<num_airports))
    airport_id++;

  if (airport_id >= num_airports){
    sprintf(ERRMSG, "could not find airport_id for %s\n", airport_name);
    pga_exit(__LINE__, __FILE__);
  }

  return(airport_id);
}

// }}}
// {{{ method: get_airport_name()

char *
CapacityData::get_airport_name(int airport_id){

  if (airport_id >= num_airports){
    sprintf(ERRMSG, "invalid airport_id %d", airport_id);
    pga_exit(__LINE__, __FILE__);
  }

  return(airport_departure_data_array[airport_id].airport_name);
}

// }}}
// {{{ method: get_sector_capacity()

int
CapacityData::get_sector_capacity(int sector_id){

  if ((sector_id < 0)||(sector_id >= num_sectors)){
    sprintf(ERRMSG, "bad sector_id %d", sector_id);
    pga_exit(__LINE__, __FILE__);
  }

  return(sector_data_array[sector_id].sector_capacity);
}

// }}}
// {{{ method: get_airport_departure_capacity()

int
CapacityData::get_airport_departure_capacity(int airport_id){

  if ((airport_id < 0)||(airport_id >= num_airports)){
    sprintf(ERRMSG, "bad airport_id %d", airport_id);
    pga_exit(__LINE__, __FILE__);
  }

  return(airport_departure_data_array[airport_id].airport_departure_capacity);
}

// }}}
// {{{ method: get_airport_arrival_capacity()

int
CapacityData::get_airport_arrival_capacity(int airport_id){

  if ((airport_id < 0)||(airport_id >= num_airports)){
    sprintf(ERRMSG, "bad airport_id %d", airport_id);
    pga_exit(__LINE__, __FILE__);
  }

  return(airport_arrival_data_array[airport_id].airport_arrival_capacity);
}

// }}}
// {{{ method: get_node_counts

void
CapacityData::get_node_counts(char *buffer){

  // the input buffer contains the capacity data file
  
  const int  LINE_LENGTH  = 256;  
  char       line[LINE_LENGTH];
  int        sectors=0, airports=0;
  int        counting_sectors=0, counting_airports=0;

  if (!buffer){
    sprintf(ERRMSG, "empty CapacityData buffer");
    pga_exit(__LINE__, __FILE__);
  }

  while (*buffer != '\0'){
    buffer = get_line_from_buffer(buffer, line);
    if ((line[0] == '#')&&(line[1] == ' ')&&(line[2] == 'S')&&
        (line[3] == 'e')&&(line[4] == 'c')&&(line[5] == 't')&&
        (line[6] == 'o')&&(line[7] == 'r')){
      counting_sectors  = 1;
      counting_airports = 0;
    }
    if ((line[0] == '#')&&(line[1] == ' ')&&(line[2] == 'A')&&
        (line[3] == 'i')&&(line[4] == 'r')&&(line[5] == 'p')&&
        (line[6] == 'o')&&(line[7] == 'r')&&(line[8] == 't')){
      counting_sectors  = 0;
      counting_airports = 1;
    }
    if ((counting_sectors)&&(line[0] != '#'))
      sectors++;
    if ((counting_airports)&&(line[0] != '#'))
      airports++;
  }

  num_sectors  = sectors;
  num_airports = airports;

}

// }}}
// {{{ method: get_node_counts_from_capacity_file

void
CapacityData::get_node_counts_from_capacity_file(char *capacity_filename){
  
  const int     LINE_LENGTH  = 256;  
  int           sectors=0, airports=0;
  int           counting_sectors=0, counting_airports=0;
  char          line[LINE_LENGTH];

  FILE *capacity_file_p = fopen(capacity_filename, "r");
  if (!capacity_file_p){
    sprintf(ERRMSG, "could not open %s", capacity_filename);
    pga_exit(__LINE__, __FILE__);
  }

  //count the number of sectors and airports
  while (fgets(line, LINE_LENGTH, capacity_file_p) ){
    if ((line[0] == '#')&&(line[1] == ' ')&&(line[2] == 'S')&&
        (line[3] == 'e')&&(line[4] == 'c')&&(line[5] == 't')&&
        (line[6] == 'o')&&(line[7] == 'r')){
      counting_sectors  = 1;
      counting_airports = 0;
    }
    if ((line[0] == '#')&&(line[1] == ' ')&&(line[2] == 'A')&&
        (line[3] == 'i')&&(line[4] == 'r')&&(line[5] == 'p')&&
        (line[6] == 'o')&&(line[7] == 'r')&&(line[8] == 't')){
      counting_sectors  = 0;
      counting_airports = 1;
    }
    if ((counting_sectors)&&(line[0] != '#'))
      sectors++;
    if ((counting_airports)&&(line[0] != '#'))
      airports++;
  }

  num_sectors     = sectors;
  num_airports    = airports;

  fclose(capacity_file_p);
}

// }}}
// {{{ method: load_CapacityData

void
CapacityData::load_CapacityData(char *buffer){

  // the input buffer contains the capacity data file
  
  const int  LINE_LENGTH  = 256;  
  char       line[LINE_LENGTH];
  int        sector_id=0, airport_id=0;
  int        capacity_value=0, airport_departure_capacity=0, 
             airport_arrival_capacity=0;
  int        reading_sectors=0, reading_airports=0;
  char       sector_name[SECTOR_NAME_STRING_LENGTH], 
             airport_name[SECTOR_NAME_STRING_LENGTH];

  if (!buffer){
    sprintf(ERRMSG, "empty CapacityData buffer");
    pga_exit(__LINE__, __FILE__);
  }

  while (*buffer != '\0'){
    buffer = get_line_from_buffer(buffer, line);
    if ((line[0] == '#')&&(line[1] == ' ')&&(line[2] == 'S')&&
        (line[3] == 'e')&&(line[4] == 'c')&&(line[5] == 't')&&
        (line[6] == 'o')&&(line[7] == 'r')){
      reading_sectors  = 1;
      reading_airports = 0;
    }
    if ((line[0] == '#')&&(line[1] == ' ')&&(line[2] == 'A')&&
        (line[3] == 'i')&&(line[4] == 'r')&&(line[5] == 'p')&&
        (line[6] == 'o')&&(line[7] == 'r')&&(line[8] == 't')){
      reading_sectors  = 0;
      reading_airports = 1;
    }
    if ((reading_sectors)&&(line[0] != '#')){
      sscanf(line, "%s%i", sector_name, &capacity_value);
      sprintf(sector_data_array[sector_id].sector_name, "%s", sector_name);
      sector_data_array[sector_id].sector_capacity = capacity_value;
      sector_id++;
    }
    if ((reading_airports)&&(line[0] != '#')){
      sscanf(line, "%s%i%i", airport_name, &airport_departure_capacity, &airport_arrival_capacity);
      sprintf(airport_departure_data_array[airport_id].airport_name, "%s", airport_name);
      sprintf(airport_arrival_data_array[airport_id].airport_name, "%s", airport_name);
      airport_departure_data_array[airport_id].airport_departure_capacity = airport_departure_capacity;
      airport_arrival_data_array[airport_id].airport_arrival_capacity = airport_arrival_capacity;
      airport_id++;
    }
  }
}

// }}}
// {{{ method: load_CapacityData_from_file

void
CapacityData::load_CapacityData_from_file(char *capacity_filename){

  // use load_CapacityData() instead of this method
  // only keeping this around just in case

  const int  LINE_LENGTH  = 256;  
  int        sector_id=0, airport_id=0;
  int        capacity_value=0, airport_departure_capacity=0, airport_arrival_capacity=0;
  int        reading_sectors=0, reading_airports=0;
  char       line[LINE_LENGTH], 
             sector_name[SECTOR_NAME_STRING_LENGTH], 
             airport_name[SECTOR_NAME_STRING_LENGTH];

  FILE *capacity_file_p = fopen(capacity_filename, "r");
  if (!capacity_file_p){
    sprintf(ERRMSG, "could not open %s", capacity_filename);
    pga_exit(__LINE__, __FILE__);
  }

  while (fgets(line, LINE_LENGTH, capacity_file_p) ){
    if ((line[0] == '#')&&(line[1] == ' ')&&(line[2] == 'S')&&
        (line[3] == 'e')&&(line[4] == 'c')&&(line[5] == 't')&&
        (line[6] == 'o')&&(line[7] == 'r')){
      reading_sectors  = 1;
      reading_airports = 0;
    }
    if ((line[0] == '#')&&(line[1] == ' ')&&(line[2] == 'A')&&
        (line[3] == 'i')&&(line[4] == 'r')&&(line[5] == 'p')&&
        (line[6] == 'o')&&(line[7] == 'r')&&(line[8] == 't')){
      reading_sectors  = 0;
      reading_airports = 1;
    }
    if ((reading_sectors)&&(line[0] != '#')){
      sscanf(line, "%s%i", sector_name, &capacity_value);
      sprintf(sector_data_array[sector_id].sector_name, "%s", sector_name);
      sector_data_array[sector_id].sector_capacity = capacity_value;
      sector_id++;
    }
    if ((reading_airports)&&(line[0] != '#')){
      sscanf(line, "%s%i%i", airport_name, &airport_departure_capacity, &airport_arrival_capacity);
      sprintf(airport_departure_data_array[airport_id].airport_name, "%s", airport_name);
      sprintf(airport_arrival_data_array[airport_id].airport_name, "%s", airport_name);
      airport_departure_data_array[airport_id].airport_departure_capacity = airport_departure_capacity;
      airport_arrival_data_array[airport_id].airport_arrival_capacity = airport_arrival_capacity;
      airport_id++;
    }
  }

  fclose(capacity_file_p);

}

// }}}








