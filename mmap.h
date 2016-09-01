
#ifndef MMAP_HPP
#define MMAP_HPP

#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
//#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
//#include <cstdio>
#include <pthread.h>

/* assert(/tmp mounted to tmpfs, i.e. resides in RAM) */
/* just use any file in /tmp */
static const char* mmapFilePath = "/tmp/arduino";


typedef struct mmapData_TAG {
  int led8_on;   // led on IO8
  int led13_on;  // built-in led
  pthread_mutex_t mutex;
  pthread_cond_t cond;
}MMAPDATA;

typedef MMAPDATA* MMAPDATA_HANDLE;

#define MAP_FAILED NULL

#endif
