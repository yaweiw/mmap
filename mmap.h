
#ifndef MMAP_HPP
#define MMAP_HPP

#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
//#include <iostream>
#include <string.h>
#include <stdlib.h>
//#include <cstdio>
#include <pthread.h>

/* assert(/tmp mounted to tmpfs, i.e. resides in RAM) */
/* just use any file in /tmp */
static const char* mmapFilePath = "/tmp/arduino";


struct mmapData {
  bool led8_on;   // led on IO8
  bool led13_on;  // built-in led
  pthread_mutex_t mutex;
  pthread_cond_t cond;
};

#endif
