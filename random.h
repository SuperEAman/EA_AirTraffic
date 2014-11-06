

typedef unsigned char  byte_t;  //hack. should really get this from global.ha


void   pm_random_setseed(int);
void   pm_random_setseed2(int);
void   pm_random_setseed3(int);
void   pm_random_setseed4(int);
byte_t random_byte(int, int);
int    random_int(int, int);
int    random_int_stream2(int, int);
int    random_int_stream3(int, int);
int    random_int_stream4(int, int);
double random_double (double, double);
int    flip (float);
