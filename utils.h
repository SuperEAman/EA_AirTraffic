#ifndef _utils_h_
#define _utils_h_

#ifndef _utils_cc_
  //give linking visibility to a global used by pga_exit()
  extern char ERRMSG[512];
#endif

void   pga_exit (int, const char*);
char * get_line_from_buffer(char *, char *);
char * read_file_into_buffer(char *);
void   print_buffer(char *);
int    options(int, char **, const char *);
void   get_time_str(char *);

#endif
