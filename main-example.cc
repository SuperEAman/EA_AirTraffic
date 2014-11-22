#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <math.h>
#include <string.h>

#define MAIN 1


#include "ACData.h"
#include "CapacityData.h"
#include "Ordering.h"
#include "utils.h"
#include "random.h"
#include "GreedyScheduler.h"

typedef float fitness_t;

//-----------------------------------------------------------------------
int main(int argc, char *argv[]) {

	const double CROSSOVER_P = 0.1;
  const int  NUM_INDIVIDUALS=10 ;
  const int LINE_LENGTH=200;
  const int FILENAME_LENGTH=100;
  char TFM_CFG_FILENAME[] = "tfm-60-80.cfg";
  float DELAYED_PERCENTAGE = 0.5;
  fitness_t WORST_FITNESS = 5000;
  int        i, iteration, num_iterations, RNG_seed, stop_if_no_change,
             print_output, iterations_per_temperature_change, output_frequency=0,
             best_yet_iteration, less_fit_replacement_count=0, *offending_ac_id = new int[NUM_INDIVIDUALS], *offending_ac_found = new int[NUM_INDIVIDUALS],
             num_iterations_with_constant_fitness=0, converged_iterations=0;
  long int   timed, elapsed_time, best_yet_found_time, WALLCLOCKSTART, WALLCLOCKEND;
  char       *time_str = new char[LINE_LENGTH], run_status_string[LINE_LENGTH], cmd[LINE_LENGTH],
             parameter_name[64], equal[5], output_line[1024], tmp_string[256];
  float      mut_prob, prob, temperature, initial_temperature, temperature_factor,
             stop_if_fitness_below, convergence_factor=0.0;
  fitness_t  child_fitness[NUM_INDIVIDUALS], parent_fitness[NUM_INDIVIDUALS], delta_fitness, best_yet_fitness, previous_parent_fitness[NUM_INDIVIDUALS];
  time_t     start,end;

  // -------------------------
  // TFM-related declarations
  // -------------------------
  int        num_flights, delayed_list_size, delayed_list_size_max;
  //char     tfm_prefix_name[FILENAME_LENGTH];
  char       *capacity_data_buffer, *acdata_buffer,
             acdata_filename[FILENAME_LENGTH], cap_data_filename[FILENAME_LENGTH];
  Ordering          *parent_ordering = new Ordering[NUM_INDIVIDUALS], *child_ordering = new Ordering[NUM_INDIVIDUALS], *tmp_ordering = new Ordering[NUM_INDIVIDUALS];
  delayed_flights_t *delayed_flights_array;
  delayed_flights_t *dft = new delayed_flights_t[NUM_INDIVIDUALS];

  //-----------------------------------------------------------------------
  // {{{ process command line args

  extern char     *options_argument;
  const char      *legal_options="s:";
  int              RNG_seed_override=0;

  // set defaults
  int done = 0;
  while (!done){
    switch (options(argc, argv, legal_options)){
      case 's': RNG_seed_override=atoi(options_argument);
                break;
      case '?': fprintf(stderr,
                        "usage: main "
                        "<-s RNG-seed-to-override-the-seed.cfg> "
                        "<-?> "
                        "\n");
                exit(1);
      case -1:  done = 1;
                break;
    }
  }

  // }}}
  // {{{ read config file (foo.cfg)

  char line[LINE_LENGTH];
  int  line_number=0;

  FILE *cfg_file_p = fopen("foo.cfg", "r");
  if (!cfg_file_p){
    sprintf(ERRMSG, "could not open foo.cfg");
    pga_exit(__LINE__, __FILE__);
  }

  // {{{ example config file

#if 0
num_iterations                      = 1000
mutation_probability                = 0.001
seed                                = 6130
initial_temperature                 = 25
temperature_factor                  = 0.92
iterations_per_temperature_change   = 1000
output_frequency                    = 10
stop_if_constant_for_X_generations  = 10
stop_if_fitness_below               = 0
convergence_factor                  = 0.05
#endif

  // }}}

  fscanf(cfg_file_p,
         "%s%s%i%s%s%f%s%s%u%s%s%f%s%s%f%s%s%i%s%s%i%s%s%i%s%s%f%s%s%f",
         parameter_name, equal, &num_iterations,
         parameter_name, equal, &mut_prob,
         parameter_name, equal, &RNG_seed,
         parameter_name, equal, &initial_temperature,
         parameter_name, equal, &temperature_factor,
         parameter_name, equal, &iterations_per_temperature_change,
         parameter_name, equal, &output_frequency,
         parameter_name, equal, &stop_if_no_change,
         parameter_name, equal, &stop_if_fitness_below,
         parameter_name, equal, &convergence_factor);

  const int NUM_PARAMETERS=10;

  converged_iterations = (int)((float)num_iterations * convergence_factor);

  if (RNG_seed == 0){
    sprintf(ERRMSG, "must initialize PM RNG with nonzero value");
    pga_exit(__LINE__, __FILE__);
  }

  if (RNG_seed_override){
    printf("using RNG_seed_override %d\n", RNG_seed_override);
    RNG_seed = RNG_seed_override;
  }

  pm_random_setseed(RNG_seed); //Initialize Park-Miller RNG
  pm_random_setseed2(RNG_seed+111); //Initialize Park-Miller RNG
  pm_random_setseed3(RNG_seed+222); //Initialize Park-Miller RNG
  pm_random_setseed4(RNG_seed+333); //Initialize Park-Miller RNG

  // ------------------------------------------------
  // run sanity checks on the parameters
  // ------------------------------------------------
  rewind(cfg_file_p);
  while (fgets(line, LINE_LENGTH, cfg_file_p))
    line_number++;

  if (line_number != NUM_PARAMETERS){
    sprintf(ERRMSG, "incorrect # of parameters (%d) in config file (%d)",
            NUM_PARAMETERS, line_number);
    pga_exit(__LINE__, __FILE__);
  }
  // ---------------------------------------------------------------
  if (num_iterations < 0){
    sprintf(ERRMSG, "num_iterations (%d) out of range", num_iterations);
    pga_exit(__LINE__, __FILE__);
  }
  // ---------------------------------------------------------------

  // }}}
  // {{{ print parameters

  printf("\nParameters:\n"
         "   num_iterations             = %d\n"
         "   mut_prob                   = %f\n"
         "   RNG_seed                   = %d\n"
         "   output_frequency           = %d\n"
         "   stop_if_no_change          = %d\n"
         "   stop_if_fitness_below      = %f\n"
         "   convergence_factor         = %5.3f\n"
         "   converged_iterations       = %d\n",
         num_iterations,
         mut_prob,
         RNG_seed,
         output_frequency,
         stop_if_no_change,
         stop_if_fitness_below,
         convergence_factor,
         converged_iterations);

  // }}}
  // {{{ CapacityData & ACData: read files and make objects

  FILE *tfm_cfg_file_p = fopen(TFM_CFG_FILENAME, "r");
  if (!tfm_cfg_file_p){
    sprintf(ERRMSG, "could not open %s", TFM_CFG_FILENAME);
    pga_exit(__LINE__, __FILE__);
  }

  fscanf(tfm_cfg_file_p,
         "%s%s%s%s%s%s",
         parameter_name, equal, acdata_filename,
         parameter_name, equal, cap_data_filename);
  printf("ac:%s, cap:%s ", acdata_filename, cap_data_filename);

  fclose(tfm_cfg_file_p);

  // --------------------------------
  // read in CapacityData and ACData
  // --------------------------------
  capacity_data_buffer = read_file_into_buffer(cap_data_filename);
  acdata_buffer        = read_file_into_buffer(acdata_filename);

  // ----------------------------------
  // construct a CapacityData object
  // ----------------------------------
  CapacityData capacity_data(capacity_data_buffer);
  //capacity_data.print();

  // ----------------------------------
  // construct an ACData object
  // ----------------------------------
  ACData acdata(acdata_buffer, &capacity_data);
  //acdata.print(&capacity_data);

  num_flights = acdata.get_number_of_flights();

  printf("--set up capacity data and ACData objects\n"); fflush(stdout);

  // }}}
  // {{{ set up delayed flights data structures

  delayed_list_size_max = (int)((DELAYED_PERCENTAGE * num_flights) + 0.5);
  delayed_list_size     = delayed_list_size_max;
  delayed_flights_array = new delayed_flights_t[NUM_INDIVIDUALS];

  for (i=0; i<NUM_INDIVIDUALS; i++){
    delayed_flights_array[i].num_delayed_flights = 0;
    delayed_flights_array[i].delayed_flight_list = new delayedFlight_t[delayed_list_size];
  }

  printf("--set up delayed flights\n"); fflush(stdout);

  // }}}

for (int i = 0; i < NUM_INDIVIDUALS; ++ i) {
	parent_ordering[i].initialize(&acdata);
	parent_ordering[i].randomize();
	parent_ordering[i].move_enroute_flights_to_top(&acdata);

	child_ordering[i].initialize(&acdata);
	child_ordering[i].randomize();
	child_ordering[i].move_enroute_flights_to_top(&acdata);


	  parent_fitness[i]     = WORST_FITNESS;
	  child_fitness[i]      = WORST_FITNESS;
	  offending_ac_found[i] = 0;
}

  best_yet_fitness   = WORST_FITNESS;

  get_time_str(time_str);
  printf("\n----------------------------------------------------------"
         "\n-----------------entering main loop-----------------------"
         "\n------------- %s ----------------"
         "\n----------------------------------------------------------\n",
         time_str);

  // ---------
  // MAIN LOOP
  // ---------
  for (iteration=0; iteration<num_iterations; iteration++){

    print_output = !(iteration % output_frequency);

    fitness_t totalFitness = 0.0;
//	for (int i = 0; i < NUM_INDIVIDUALS; ++ i) {
//		previous_parent_fitness[i] = parent_fitness[i];
//
//		// {{{ mutate parent into child
//
//		// must do this copy since we always need to keep a copy of the parent
//		//  (ie we can't swap pointers to avoid the copy)
//		child_ordering[i].copy(&parent_ordering[i]);
//	}

    for (int i = 0; i < NUM_INDIVIDUALS; i = i + 2) {
    		previous_parent_fitness[i] = (WORST_FITNESS-(parent_fitness[i]) + 1000)*5;
    		previous_parent_fitness[i + 1] = (WORST_FITNESS-(parent_fitness[i]) + 1000)*5;
    		totalFitness += previous_parent_fitness[i] + previous_parent_fitness[i + 1];
    }

    for (int i = 0; i < NUM_INDIVIDUALS; i = i + 2) {
			double randNum = random_double(0.0,1.0) * totalFitness;
			int idx;
			for (idx=0; idx<NUM_INDIVIDUALS && randNum>0; ++idx) {
				randNum -= previous_parent_fitness[idx];
			}

			double randNum1 = random_double(0.0,1.0) * totalFitness;
			int idx1;
			for (idx1=0; idx1<NUM_INDIVIDUALS && randNum1>0; ++idx1) {
				randNum1 -= previous_parent_fitness[idx1];
			}

			crossover(&parent_ordering[idx - 1], &parent_ordering[idx1 - 1],&child_ordering[i], &child_ordering[i+1],
								0, CROSSOVER_P, ORDER_BASED,&dft[i],&dft[i+1]);
    }

	for (int i = 0; i < NUM_INDIVIDUALS; ++ i) {
		// mutate the copy of the parent

		// option 1: perform mutation across the entire permutation
		child_ordering[i].mutate(mut_prob);

		// option 2: perform mutation only among enroute flights, only among non-enroute flights
		//child_ordering->mutate_enroute_flights(mut_prob);
		//child_ordering->mutate_non_enroute_flights(mut_prob);

		if (offending_ac_found[i]){

		  //printf("SA_main: moving offensive ac_id (%d) to the top of the"
		  //       " child ordering\n", offending_ac_id);

		  //move the offending aircraft to the top of the ordering
		  child_ordering[i].move_ac_id_to_top(offending_ac_id[i]);
		}

		// }}}
		// {{{ compute fitness of the child

		{
		//delayed_list_size = delayed_list_size_max;

		GreedyScheduler g(&capacity_data, &acdata);

		//---------------------------------------------------------
		//greedilySchedule() -- the key function call for the slave
		//---------------------------------------------------------
		child_fitness[i] = (float) g.greedilySchedule(child_ordering[i], false,
										&delayed_list_size,
										delayed_flights_array[0].delayed_flight_list);

		//a negative delay value denotes an ordering that was impossible to schedule
		//the negative value is the offending ac_id that could not be scheduled
		//
		//if delay=-1, greedilySchedule will not fill in the
		//delayed_list_size nor the delayed_flight_list

		if (child_fitness[i] < 0.0){
		  offending_ac_found[i] = 1;
		  offending_ac_id[i] = (int)(child_fitness[i] * -1.0);
		  child_fitness[i] = WORST_FITNESS;
		}
		else {
		  offending_ac_found[i] = 0;
		}

		}

		// }}}

		if (child_fitness[i] < best_yet_fitness){
		  best_yet_fitness    = child_fitness[i];
		  best_yet_iteration  = iteration;
		}

		// {{{ print output

		if (print_output)
		  sprintf(output_line, "%6d byf: %4.0f | ", iteration, best_yet_fitness);

		// }}}

		if (child_fitness[i] <= parent_fitness[i]){
		  //child is more fit, make child the new parent





		  // {{{ print output

		  if (print_output){
			sprintf(tmp_string,
				  "child (f=%4.0f) more fit than parent (f=%4.0f), replacing parent\n",
				  child_fitness[i], parent_fitness[i]);
			strcat(output_line, tmp_string);
		  }

		  // }}}

		  //copy child into parent by swapping pointers
		  //  this will work, but is slower: parent_ordering.copy(&child_ordering);
		  tmp_ordering[i]    = parent_ordering[i];
		  parent_ordering[i] = child_ordering[i];
		  child_ordering[i]  = tmp_ordering[i];

		  parent_fitness[i] = child_fitness[i];
		}
	}



    // {{{ print output

#if 0
    if (print_output){
      sprintf(tmp_string, "num_const=%d\n", num_constant_fitness_iterations);
      strcat(output_line, tmp_string);
    }
#endif

    // }}}

    if (print_output)
      printf("%s", output_line);
    fflush(stdout);

  }

  // {{{ print end message

  elapsed_time = WALLCLOCKEND-WALLCLOCKSTART;

  printf("\n"
         "results: num_iterations: %d "
         "best_yet_fitness: %4.0f "
         "by_iteration: %d "
         "by_found_time: %ld "
         "less_fit_replacements: %d "
         "wallclock: %ld "
         "cputime: %f\n\n",
         num_iterations, best_yet_fitness, best_yet_iteration, best_yet_found_time,
         less_fit_replacement_count, elapsed_time, (float)timed/1000.0);


//printf("-------------------------------------------------------------\n");

//double dif;
//dif = difftime (end,start);
//printf ("elapsed time:  %lf seconds\n", dif );


  printf("processed %d schedules in %ld milliseconds"
         " (%6.2f schedules/s) (%5.2f ms/schedule)\n",
         num_iterations, timed, (float)(1000.0*num_iterations/timed),
         ((float)timed / (float)num_iterations));


//  printf("elapsed wall-clock time = %2ld hrs, %2ld mins (%ld sec)\n",
//         elapsed_time/3600, (elapsed_time%3600)/60, elapsed_time);
//  printf("cpu time                = %2ld hrs, %2ld mins (%ld milliseconds)\n",
//         timed/3600000, (timed%3600000)/60000, timed); fflush(stdout);

  get_time_str(time_str);
  printf("\nSA run finished on %s\n", time_str);

  // }}}

}
