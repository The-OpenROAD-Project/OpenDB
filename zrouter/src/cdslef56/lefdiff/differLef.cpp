//
//       Copyright (C) 1997 Cadence Design Systems Inc.
//                     All Rights Reserved.
//
 
 
// This program will diff two lef files or two def files and list the
// different between the two files.  This problem is not intend to
// diff a real design.  If user runs this program will a big design,
// they may experience long execution time and may even ran out of
// memory.
//
// This program is to give user a feel of whether they are using the
// parser correctly.  After they read the lef/def file in, and
// write them back out in lef/def format.
//
// This program support lef/def 5.6.

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#ifdef ibmrs
#   include <strings.h>
#endif
#ifndef WIN32
#   include <unistd.h>
#endif /* not WIN32 */
#include "../lef/lefrReader.hpp"
#include "diffLefRW.hpp"

extern char VersionIdent[];

char * exeName;    // use to save the executable name

// This program requires 2 inputs, fileName1 and fileName2
void diffUsage() {
   printf("Usage: lefdefdiff -lef|-def fileName1 fileName2 [-o outputFileName]\n");
}

int main(int argc, char** argv) {
  char *fileName1, *fileName2;   // For the filenames to compare
  char *lefOut1, *lefOut2;       // For the tmp output files

  exeName = argv[0];
  
  if ((argc == 2) && strcmp(argv[1], "-version") == 0) { 
      // User asking for version only
      (void)fprintf(stderr, "lefdefdiff %s\n", VersionIdent);
      (void)fflush(stderr);
      return(0);
  }

  if (argc != 5) {               // If pass in from lefdefdiff, argc is
      diffUsage();               // always 5
      return(1);
  }

  fileName1 = argv[1];
  fileName2 = argv[2];

  // These are temporary files to hold the lef data as they are read in,
  // later these files will be sorted for comparison.
  lefOut1 = argv[3];
  lefOut2 = argv[4];

  // just read in the 1st file to memory
  printf("Reading file: %s\n", fileName1);
  if (diffLefReadFile(fileName1, lefOut1) != 0)
      return(1);
  // read and compare with the 1st file in memory
  printf("Reading file: %s\n", fileName2);
  if (diffLefReadFile(fileName2, lefOut2) != 0)
      return(1);

  return (0);
}
