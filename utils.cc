/*
 * utils.cc
 *
 *  Created on: Nov 5, 2014
 *      Author: Xiatao Jin
 */




#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include <unistd.h>
#include <time.h>
char ERRMSG[512];
char* options_argument = new char[512];
void pga_exit (int line, const char* file) {
	printf("ERRMSG: %s",ERRMSG);
	printf("line: %d, file: %s", line, file);
	exit(-1);
}

char * get_line_from_buffer(char * buffer, char * line) {
	int i = 0;
	while (buffer[i] != '\n' && buffer[i] != '\0') {
		line[i] = buffer[i];
		i ++;
	}
	if (buffer[i] == '\n') {
		line[i] = '\n';
		return buffer + i + 1;
	}
	else {
		line[i] = '\0';
		return buffer + i;
	}
//	 *buffer = '\0';
//	 return buffer;
}

char * read_file_into_buffer(char * filename) {
//	char* source = new char[1000000];
//
//	FILE *fp = fopen(filename, "r");
//	printf("filename: %s", filename);
//	char symbol;
//	if(fp != NULL)
//	{
//	    while((symbol = getc(fp)) != EOF)
//	    {
//	        strcat(source, &symbol);
//	    }
//	    fclose(fp);
//	}

	FILE * pFile;
	  long lSize;
	  char * buffer;
	  size_t result;

	  pFile = fopen ( filename , "r" );
	  if (pFile==NULL) {fputs ("File error",stderr); exit (1);}

	  // obtain file size:
	  fseek (pFile , 0 , SEEK_END);
	  lSize = ftell (pFile);
	  rewind (pFile);

	  // allocate memory to contain the whole file:
	  buffer = (char*) malloc (sizeof(char)*lSize);
	  if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}

	  // copy the file into the buffer:
	  result = fread (buffer,1,lSize,pFile);
	  if (result != lSize) {fputs ("Reading error",stderr); exit (3);}

	  /* the whole file is now loaded in the memory buffer. */

	  // terminate
	  fclose (pFile);
	return buffer;
}
void   print_buffer(char * buffer) {
	printf("buffer:%s", buffer);
}
int    options(int argc, char ** argv, const char * legal_options) {
	int ch = getopt(argc, argv, legal_options);
	if (ch != 's' || ch != '?') {
		return -1;
	}
	options_argument = optarg;
	return ch;
}
void   get_time_str(char *time_str) {
	time_t rawtime;
	struct tm * timeinfo;

	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	char* time_str_old = asctime (timeinfo);
	int i = 0;
	while (time_str_old[i] != '\0') {
		time_str[i] = time_str_old[i];
		i ++;
	}
	printf("time:%s", time_str);
}
