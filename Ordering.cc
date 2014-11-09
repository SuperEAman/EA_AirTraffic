// ---------------------------------------------------------
// Ordering.cc
//
// Source file for Ordering object
// ---------------------------------------------------------
//
// This class holds an array of aircraft IDs, the order represents
// a raw schedule, not necessarily legal, that is used as input to
// a greedy scheduler that produces legal schedules.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Ordering.h"
#include "utils.h"
#include "random.h"
// {{{ method: default constructor

Ordering::Ordering() {

  //printf("Ordering default constructor called\n");

  num_flights = 0;
  ac_id_array = NULL;

}

// }}}
// {{{ method: constructor

Ordering::Ordering(int n) {

  //printf("Ordering constructor (int) called\n");

  num_flights = n;

  // allocate space for the data array
  ac_id_array  = new int[num_flights];

  for (int i=0; i<num_flights; i++){
    ac_id_array[i] = i;
  }

}

// }}}
// {{{ method: initialize

void
Ordering::initialize(ACData *ac_data) {


  // find out how many flights we need to work with
  num_flights = ac_data->get_number_of_flights();

  // allocate space for the data array
  ac_id_array  = new int[num_flights];

  for (int i=0; i<num_flights; i++){
    ac_id_array[i] = ac_data->get_ac_id_given_index(i);
  }

  num_enroute_flights = ac_data->get_number_of_enroute_flights();

  //printf("num_enroute_flights = %d\n", num_enroute_flights);

}

// }}}
// {{{ method: print()

void
Ordering::print(FILE *fp){

  fprintf(fp, "[printing Ordering]\n");

  for (int i=0; i<num_flights; i++){
    fprintf(fp, "[%4d] %d\n", i, ac_id_array[i]);
  }

  fflush(fp);
}

// }}}
// {{{ method: print_compressed()

void
Ordering::print_compressed(){

  printf("[printing Ordering]\n");

  for (int i=0; i<num_flights; i++){
    printf("%d:%d ", i, ac_id_array[i]);
  }
  printf("\n");

}

// }}}
// {{{ method: copy()

void
Ordering::copy(Ordering *p){

  num_flights = p->get_number_of_flights();
  for (int i=0; i<num_flights; i++){
    ac_id_array[i] = p->get_ac_id(i);
  }

}

// }}}
// {{{ method: randomize()

void
Ordering::randomize(){
  
  int i, rn;
  int *already_taken_array;

  already_taken_array = new int[num_flights];
  for (i=0; i<num_flights; i++)
    already_taken_array[i] = 0;

  // randomize the ordering, while making sure not to have duplicates
  for (i=0; i<num_flights; i++){
    rn = random_int(0, (num_flights-1));
    while (already_taken_array[rn])
      rn = random_int(0, (num_flights-1));
    ac_id_array[i]          = rn;
    already_taken_array[rn] = 1;
  }

  delete [] already_taken_array;

}

// }}}
// {{{ method: move_enroute_flights_to_top()

void
Ordering::move_enroute_flights_to_top(ACData *ac_data){
  
  int i, j, ac_id;
  int *tmp_ac_id_array;

  tmp_ac_id_array = new int[num_flights];
  for (i=0; i<num_flights; i++)
    tmp_ac_id_array[i] = -1;

  j=0;
  // first pass: find enroute flights and copy to tmp ordering
  for (i=0; i<num_flights; i++){
    ac_id = ac_id_array[i];
    if (ac_data->is_flight_enroute(ac_id)){
      tmp_ac_id_array[j] = ac_id;
      j++;
    }
  }

  // second pass: find non-enorute flight and append to tmp ordering
  for (i=0; i<num_flights; i++){
    ac_id = ac_id_array[i];
    if (!ac_data->is_flight_enroute(ac_id)){
      tmp_ac_id_array[j] = ac_id;
      j++;
    }
  }

  // third pass: copy tmp to real ac_id_array
  for (i=0; i<num_flights; i++){
    ac_id_array[i] = tmp_ac_id_array[i];
  }

  delete [] tmp_ac_id_array;

}

// }}}
// {{{ method: move_ac_id_to_top()  

void
Ordering::move_ac_id_to_top(int ac_id){

  int i, j;
  int *tmp_ac_id_array;

  tmp_ac_id_array = new int[num_flights];
  for (i=0; i<num_flights; i++)
    tmp_ac_id_array[i] = -1;

  tmp_ac_id_array[0] = ac_id;

  j=1;

  for (i=0; i<num_flights; i++){
    if (ac_id_array[i] != ac_id){
      tmp_ac_id_array[j] = ac_id_array[i];
      j++;
    }
  }

  // copy tmp to real ac_id_array
  for (i=0; i<num_flights; i++){
    ac_id_array[i] = tmp_ac_id_array[i];
  }

  delete [] tmp_ac_id_array;

}

// }}}
// {{{ method: mutate()

void
Ordering::mutate(float mutation_rate){
  
  int i, j, k, temp, num_mutations;

  // order-basd mutation
  // perform up to (num_flights*mutation_rate) mutations on the individual

  num_mutations = (int)((float)num_flights * mutation_rate);

  for (int i = 0; i < num_mutations; i = i + 2) {
	  j = random_int(0, num_flights - 1);
	  k = random_int(0, num_flights - 1);
	  temp = ac_id_array[j];
	  ac_id_array[j] = ac_id_array[k];
	  ac_id_array[k] = temp;
  }








}

// }}}
// {{{ method: mutate_enroute_flights()

void
Ordering::mutate_enroute_flights(float mutation_rate){

  //perform mutatations among enroute flights
  //assumes enroute flights are at the tpo of the array!
  
  int i, j, k, temp, num_mutations;

  // order-basd mutation
  // perform up to (num_enroute_flights*mutation_rate) mutations on the individual

  num_mutations = (int)((float)num_enroute_flights * mutation_rate);
  for (int i = 0; i < num_mutations; i = i + 2) {
  	  j = random_int(0, num_enroute_flights - 1);
  	  k = random_int(0, num_enroute_flights - 1);
  	  temp = ac_id_array[j];
  	  ac_id_array[j] = ac_id_array[k];
  	  ac_id_array[k] = temp;
   }













}

// }}}
// {{{ method: mutate_non_enroute_flights()

void
Ordering::mutate_non_enroute_flights(float mutation_rate){

  //perform mutatations among enroute flights
  //assumes enroute flights are at the tpo of the array!
  
  int i, j, k, temp, num_mutations;

  num_mutations = (int)((float)(num_flights - num_enroute_flights) * mutation_rate);
	for (int i = 0; i < num_mutations; i = i + 2) {
		  j = random_int(num_enroute_flights, num_flights - 1);
		  k = random_int(num_enroute_flights, num_flights - 1);
		  temp = ac_id_array[j];
		  ac_id_array[j] = ac_id_array[k];
		  ac_id_array[k] = temp;
	 }











}

// }}}
// {{{ method: mutate_pbm()

void
Ordering::mutate_pbm(float mutation_rate){
  
  int i, j, k, m, num_mutations, bottom_index, top_index, bottom_element;

  //position-based mutation
  //  1) select 2 tasks at random
  //  2) place 2nd task before the first

  num_mutations = (int)((float)num_flights * mutation_rate);

    for (int i = 0; i < num_mutations; i = i + 2) {
  	  j = random_int(0, num_flights - 1);
  	  k = random_int(0, num_flights - 1);
  	  if (j < k) {
  		  top_index = j;
  		  bottom_index = k;
  	  }
  	  else {
  		  top_index = k;
  		  bottom_index = j;
  	  }
  	  bottom_element = ac_id_array[bottom_index];
  	  for (int m = bottom_index - 1; m >= top_index; m --) {
  		  ac_id_array[m + 1] = ac_id_array[m];
  	  }
  	  ac_id_array[top_index] = bottom_element;
    }
















}

// }}}
// {{{ method: get_ac_id()

int
Ordering::get_ac_id(int index){

  if ((index<0)||(index>=num_flights)){
    sprintf(ERRMSG, "index out of bounds (%d)\n", index);
    pga_exit(__LINE__, __FILE__);
  }

  return(ac_id_array[index]);
}

// }}}
// {{{ method: put_ac_id()

void
Ordering::put_ac_id(int index, int ac_id){

  if ((index<0)||(index>=num_flights)){
    sprintf(ERRMSG, "index out of bounds (%d)\n", index);
    pga_exit(__LINE__, __FILE__);
  }

  // had to comment this out since crossover makes use of negative ac_id 
  // to denote unscheduled/scheduled aircraft
  //if ((ac_id<0)||(ac_id>=num_flights)){
  //  sprintf(ERRMSG, "aircraft ID out of bounds (%d)\n", ac_id);
  //  pga_exit(__LINE__, __FILE__);
  //}

  ac_id_array[index] = ac_id;

}

// }}}
// {{{ method: get_number_of_flights()

int
Ordering::get_number_of_flights(){

  return(num_flights);
}

// }}}
// {{{ method: convert_from_string()

void
Ordering::convert_from_string(char *buffer){

  const int LINE_LENGTH=200;
  int  i=0;
  char *buffer_start, line[LINE_LENGTH];

  // converts a string representation of the Ordering ac_id_array
  // into an array of integers

  buffer_start = buffer;

  while (*buffer != '\0'){
    buffer         = get_line_from_buffer(buffer, line);
    ac_id_array[i] = atoi(line);
    i++;
  }

  if (i != num_flights){
    sprintf(ERRMSG, 
            "convert_from_string did not receive the correct # of flights\n");
    pga_exit(__LINE__, __FILE__);
  }

  // although we could do a delete [] here, we don't, and leave it 
  // up to the caller to do that

}

// }}}
// {{{ method: convert_to_string()

int
Ordering::convert_to_string(char **buffer_start){

  int i, n, bufsize=0, num_bytes=0;
  char *buffer;

  // converts the Ordering ac_id_array into a string of char's
  // caller is responsible for doing delete [] to free up memory
  // returns the number of bytes in the string (much less than is allocated)

  // compute the buffer size needed
  // account for the ac_id integers (use 7 digits should be safe as there
  //   are only a few thousand flights per day)
  bufsize = num_flights * 7;

  // account for newlines (1 '\n' per ac_id) and EOB marker
  bufsize = bufsize + num_flights + 2;

  // allocate memory for the string
  *buffer_start = new char[bufsize];

  buffer = *buffer_start;
  for (i=0; i<num_flights; i++){
    n = sprintf(buffer, "%7d\n", ac_id_array[i]);
    buffer = buffer + n;
  }

  // end the buffer with an EOB marker
  *buffer = '\0';

  num_bytes = 1 + (int)(buffer - *buffer_start);

  //printf("bufsize=%d (allocated), num_bytes=%d (written to string), "
  //       "strlen gives: %d\n", bufsize, num_bytes, strlen(*buffer_start));

  if (num_bytes > bufsize){
    sprintf(ERRMSG, "did not allocate enough memory (bufsize=%d)", bufsize);
    pga_exit(__LINE__, __FILE__);
  }

  return(num_bytes);

}

// }}}

// {{{ crossover()

void 
crossover(Ordering *p1, Ordering *p2, Ordering *c1, Ordering *c2, 
          int thread_id, float xover_prob, xover_type_t xover_type, 
          delayed_flights_t *delayed_flights_p1, delayed_flights_t *delayed_flights_p2) {

  //------------------------------------------------------------------------------
  // Note on the arguments:
  //   delayed_flights pointers are only used when xover_type=POSITION_BASED_BIASED
  //   otherwise, they are ignored.  To turn on collection of delayed_flights,
  //   set USE_DELAYED_FLIGHTS to 1 in pga.h
  //------------------------------------------------------------------------------

  const int SCHEDULED   = -1;
  const int UNSCHEDULED = -2;




























}

// }}}




