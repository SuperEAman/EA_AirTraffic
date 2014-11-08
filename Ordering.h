// ---------------------------------------------------------
// Ordering.h
//
// Header file for Ordering class
// ---------------------------------------------------------

#ifndef _Ordering_h_
#define _Ordering_h_

#include "ACData.h"

//---------------------------
// constants
//---------------------------

//---------------------------
// types
//---------------------------
enum xover_type_t{POSITION_BASED_STD, POSITION_BASED_BIASED, ORDER_BASED};

typedef struct{
	int ac_id;
	int delay;
        int ordering_index;
} delayedFlight_t;

typedef struct{
    int             num_delayed_flights;
    delayedFlight_t *delayed_flight_list; } delayed_flights_t;


//---------------------------
// classes
//---------------------------
class Ordering {

public:
         Ordering();
         //Ordering(ACData *);
         Ordering(int);
  void   initialize(ACData *);
  void   print(FILE *);
  void   print_compressed();
  void   randomize();
  void   move_enroute_flights_to_top(ACData *);
  void   move_ac_id_to_top(int);
  void   copy(Ordering *);
  void   mutate(float);
  void   mutate_enroute_flights(float);
  void   mutate_non_enroute_flights(float);
  void   mutate_pbm(float);
  int    get_ac_id(int);
  inline int get_ac_id_inline(int index){return(ac_id_array[index]);}
  void   put_ac_id(int, int);
  int    get_number_of_flights();
  void   convert_from_string(char *);
  int    convert_to_string(char **);

private:
  int     num_flights;
  int     num_enroute_flights;
  int     *ac_id_array;
};

//---------------------------
// prototypes
//---------------------------

void  crossover(Ordering *, Ordering *, Ordering *, Ordering *, int, float, xover_type_t,
                delayed_flights_t *, delayed_flights_t *);

//void  crossover(Ordering *, Ordering *, Ordering *, Ordering *, int, float, xover_type_t);
//void  crossover_position_based(Ordering *, Ordering *, Ordering *, Ordering *, int, float);
//void  crossover_order_based(Ordering *, Ordering *, Ordering *, Ordering *, int, float);

#endif
