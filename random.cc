#include <time.h>          // has time()

typedef unsigned char  byte_t;  //hack. should really get this from global.h
#include <math.h>

static int PM_RNG_SEED;
static int PM_RNG_SEED2;
static int PM_RNG_SEED3;
static int PM_RNG_SEED4;

// Park-Miller (PM) random number generator functions

// {{{ pm_random

// {{{ Explanation

// park-miller.cc   Park-Miller random number generator

// W. Langdon cs.ucl.ac.uk 5 May 1994
// W.B.Langdon@cs.bham.ac.uk 12 December 1996 port for alpha

/*********************************************************************

INTRODUCTION

This file contains (at your own risk) an implementation of the
Park-Miller minimum random number generator (Comm ACM Oct 1988
p1192-1201, Vol 31 Num 10).  It is suitable for use with GPQUICK-2.1

USAGE

All psuedo random number generators need a seed (ie initial starting
value from which to generate the sequence). In some cases this is
based on reading the current system time. This code allows you to
specify what it is. Any positive 32 bit number (NOT zero!) will do the
first time you call it. Use the 32 bit value it returns as the seed
for the next call, and so on for each sucessive call.

A positive psuedo random integer uniformly distributed between 1 and
2147483647 (ie 2**31-1) is returned.

Example

Taking the modulus 52 of the returned value will give you a value
between 0 and 51. This has a slight bias as 2**31-1 is not exactly
divisible by 52 but it may be good enough for your purposes.

NB use the whole 32 bit random value as the seed for the next call. Do
not use its modulus, as this will lead to a very short random number
sequence not the full 2147483647 which park-miller provides.

*********************************************************************/

// }}}

int
pm_random (void) {
  // 1<=seed<=m
#if LONG_MAX > (16807*2147483647)
  int const a    = 16807;      //ie 7**5
  int const m    = 2147483647; //ie 2**31-1
  PM_RNG_SEED    = (long(PM_RNG_SEED; * a))%m;
  return PM_RNG_SEED;;
#else
  double const a    = 16807;      //ie 7**5
  double const m    = 2147483647; //ie 2**31-1

  double temp = PM_RNG_SEED * a;
  PM_RNG_SEED = (int) (temp - m * floor ( temp / m ));
  return PM_RNG_SEED;
#endif
}

// }}}
// {{{ pm_random2

// {{{ Explanation

// park-miller.cc   Park-Miller random number generator

// W. Langdon cs.ucl.ac.uk 5 May 1994
// W.B.Langdon@cs.bham.ac.uk 12 December 1996 port for alpha

/*********************************************************************

INTRODUCTION

This file contains (at your own risk) an implementation of the
Park-Miller minimum random number generator (Comm ACM Oct 1988
p1192-1201, Vol 31 Num 10).  It is suitable for use with GPQUICK-2.1

USAGE

All psuedo random number generators need a seed (ie initial starting
value from which to generate the sequence). In some cases this is
based on reading the current system time. This code allows you to
specify what it is. Any positive 32 bit number (NOT zero!) will do the
first time you call it. Use the 32 bit value it returns as the seed
for the next call, and so on for each sucessive call.

A positive psuedo random integer uniformly distributed between 1 and
2147483647 (ie 2**31-1) is returned.

Example

Taking the modulus 52 of the returned value will give you a value
between 0 and 51. This has a slight bias as 2**31-1 is not exactly
divisible by 52 but it may be good enough for your purposes.

NB use the whole 32 bit random value as the seed for the next call. Do
not use its modulus, as this will lead to a very short random number
sequence not the full 2147483647 which park-miller provides.

*********************************************************************/

// }}}

//this is a second random number stream 

int
pm_random2 (void) {
  // 1<=seed<=m
#if LONG_MAX > (16807*2147483647)
  int const a    = 16807;      //ie 7**5
  int const m    = 2147483647; //ie 2**31-1
  PM_RNG_SEED2   = (long(PM_RNG_SEED2; * a))%m;
  return PM_RNG_SEED2;;
#else
  double const a    = 16807;      //ie 7**5
  double const m    = 2147483647; //ie 2**31-1

  double temp = PM_RNG_SEED2 * a;
  PM_RNG_SEED2 = (int) (temp - m * floor ( temp / m ));
  return PM_RNG_SEED2;
#endif
}

// }}}
// {{{ pm_random3

// {{{ Explanation

// park-miller.cc   Park-Miller random number generator

// W. Langdon cs.ucl.ac.uk 5 May 1994
// W.B.Langdon@cs.bham.ac.uk 12 December 1996 port for alpha

/*********************************************************************

INTRODUCTION

This file contains (at your own risk) an implementation of the
Park-Miller minimum random number generator (Comm ACM Oct 1988
p1192-1201, Vol 31 Num 10).  It is suitable for use with GPQUICK-2.1

USAGE

All psuedo random number generators need a seed (ie initial starting
value from which to generate the sequence). In some cases this is
based on reading the current system time. This code allows you to
specify what it is. Any positive 32 bit number (NOT zero!) will do the
first time you call it. Use the 32 bit value it returns as the seed
for the next call, and so on for each sucessive call.

A positive psuedo random integer uniformly distributed between 1 and
2147483647 (ie 2**31-1) is returned.

Example

Taking the modulus 52 of the returned value will give you a value
between 0 and 51. This has a slight bias as 2**31-1 is not exactly
divisible by 52 but it may be good enough for your purposes.

NB use the whole 32 bit random value as the seed for the next call. Do
not use its modulus, as this will lead to a very short random number
sequence not the full 2147483647 which park-miller provides.

*********************************************************************/

// }}}

//this is a third random number stream 

int
pm_random3 (void) {
  // 1<=seed<=m
#if LONG_MAX > (16807*2147483647)
  int const a    = 16807;      //ie 7**5
  int const m    = 2147483647; //ie 2**31-1
  PM_RNG_SEED3   = (long(PM_RNG_SEED3; * a))%m;
  return PM_RNG_SEED3;;
#else
  double const a    = 16807;      //ie 7**5
  double const m    = 2147483647; //ie 2**31-1

  double temp = PM_RNG_SEED3 * a;
  PM_RNG_SEED3 = (int) (temp - m * floor ( temp / m ));
  return PM_RNG_SEED3;
#endif
}

// }}}
// {{{ pm_random4

// {{{ Explanation

// park-miller.cc   Park-Miller random number generator

// W. Langdon cs.ucl.ac.uk 5 May 1994
// W.B.Langdon@cs.bham.ac.uk 12 December 1996 port for alpha

/*********************************************************************

INTRODUCTION

This file contains (at your own risk) an implementation of the
Park-Miller minimum random number generator (Comm ACM Oct 1988
p1192-1201, Vol 31 Num 10).  It is suitable for use with GPQUICK-2.1

USAGE

All psuedo random number generators need a seed (ie initial starting
value from which to generate the sequence). In some cases this is
based on reading the current system time. This code allows you to
specify what it is. Any positive 32 bit number (NOT zero!) will do the
first time you call it. Use the 32 bit value it returns as the seed
for the next call, and so on for each sucessive call.

A positive psuedo random integer uniformly distributed between 1 and
2147483647 (ie 2**31-1) is returned.

Example

Taking the modulus 52 of the returned value will give you a value
between 0 and 51. This has a slight bias as 2**31-1 is not exactly
divisible by 52 but it may be good enough for your purposes.

NB use the whole 32 bit random value as the seed for the next call. Do
not use its modulus, as this will lead to a very short random number
sequence not the full 2147483647 which park-miller provides.

*********************************************************************/

// }}}

//this is a fourth random number stream 

int
pm_random4 (void) {
  // 1<=seed<=m
#if LONG_MAX > (16807*2147483647)
  int const a    = 16807;      //ie 7**5
  int const m    = 2147483647; //ie 2**31-1
  PM_RNG_SEED4   = (long(PM_RNG_SEED4; * a))%m;
  return PM_RNG_SEED4;;
#else
  double const a    = 16807;      //ie 7**5
  double const m    = 2147483647; //ie 2**31-1

  double temp = PM_RNG_SEED4 * a;
  PM_RNG_SEED4 = (int) (temp - m * floor ( temp / m ));
  return PM_RNG_SEED4;
#endif
}

// }}}
// {{{ pm_random_setseed

void
pm_random_setseed(int seed) {
  PM_RNG_SEED = seed;
}

// }}}
// {{{ pm_random_setseed2

void
pm_random_setseed2(int seed) {
  PM_RNG_SEED2 = seed;
}

// }}}
// {{{ pm_random_setseed3

void
pm_random_setseed3(int seed) {
  PM_RNG_SEED3 = seed;
}

// }}}
// {{{ pm_random_setseed4

void
pm_random_setseed4(int seed) {
  PM_RNG_SEED4 = seed;
}

// }}}
// {{{ COMMENT main() to test pm_random

#if 0
#include <stdio.h>
main(){
  int u, seed;
  seed = 1;

  printf("\nProgram to test Park-Miller Random number generator\n");
  printf("Genenerating 10,000 random numbers with an initial seed of 1\n");
  //rndomize(1);
  pm_random_setseed(1);
  for (int i=1; i<=10000; i++){
    u = pm_random();
    //seed takes on u's value
    printf("i=%9d   u=%9d   seed=%9d\n", i,u,seed);
  }
  printf("10,000th random number is %d it should be 1043618065\n", u);

}
#endif

// }}}
// {{{ random_byte

byte_t random_byte (int min, int max){
  return (min + (pm_random() % (max-min+1))); }

// }}}
// {{{ random_int

int random_int (int min, int max){
  return (min + (pm_random() % (max-min+1))); }

// }}}
// {{{ random_int_stream2

int random_int_stream2 (int min, int max){
  return (min + (pm_random2() % (max-min+1))); }

// }}}
// {{{ random_int_stream3

int random_int_stream3 (int min, int max){
  return (min + (pm_random3() % (max-min+1))); }

// }}}
// {{{ random_int_stream4

int random_int_stream4 (int min, int max){
  return (min + (pm_random4() % (max-min+1))); }

// }}}
// {{{ random_double

double
random_double (double min, double max){

  return (((((double)pm_random() - 1.0) / 2147483646.0) * (max - min)) + min);
  
}

// }}}
// {{{ flip

// This function flips a biased coin with the probability "probability" 
// of having a 1 returned.  (flip(0.75) returns a 1 3/4 of the time)    
int
flip (float probability){
  float random;

  random = random_int(0,10000)/10000.0;
  if ( random < probability )
    return (1);
  else
    return (0);
}

// }}}

#if 0
//extern "C" void srandom(int);
//extern "C" long random();

// {{{ random_byte

byte_t random_byte (int min, int max){
  return (min + (random() % (max-min+1))); }

// }}}
// {{{ random_int

int random_int (int min, int max){
  return (min + (random() % (max-min+1))); }

// }}}
// {{{ flip

// This function flips a biased coin with the probability "probability" 
// of having a 1 returned.  (flip(0.75) returns a 1 3/4 of the time)    
int
flip (float probability){
  float random;

  random = random_int(0,10000)/10000.0;
  if ( random < probability )
    return (1);
  else
    return (0);
}

// }}}
#endif

#if 0
// {{{ seed_RNG_with_time 

//-----------------------------------------------------------------------
// seed_RNG_with_time 
//-----------------------------------------------------------------------
//                                                                       
//  This function seeds the random number generator using the current    
//  clock time.                                                          
//                                                                       
void
seed_RNG_with_time(){
  time_t     now;
  struct tm *date;
  int        i;

  //now  = time(NULL);
  now  = time(0);
  date = localtime(&now);

  srandom(500 * date->tm_sec + 25 * date->tm_min + date->tm_hour);
  for (i=0; i<date->tm_sec; i++)
    random();
}

// }}}
// {{{ random_int

//-----------------------------------------------------------------------
// random_int
//-----------------------------------------------------------------------
//                                                                    
//  This function returns a pseudo-random number between min and max, 
// inclusive.  Range always starts at 0                               
//                                                                    
int
random_int (int min, int max){
  return (min + (random() % (max-min+1)));
}

// }}}
// {{{ flip

//-----------------------------------------------------------------------
// flip
//-----------------------------------------------------------------------
//                                                                      
// This function flips a biased coin with the probability "probability" 
// of having a 1 returned.  (flip(0.75) returns a 1 3/4 of the time)    
//                                                                      
int
flip (float probability){
  float random;

  random = ((float)random_int(0,10000))/10000.0;
  if ( random < probability )
    return (1);
  else
    return (0);
}

// }}}
#endif

// {{{ Fold info

/* Local Variables: */
/* mode:c++ */
/* folded-file: t */
/* End: */

// }}}
