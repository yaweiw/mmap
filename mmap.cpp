
#include "mmap.h"

void exitError(const char* errMsg) {
  perror(errMsg);
  exit(EXIT_FAILURE);
}


using namespace std;


/**
 * @brief: for this example uses a binary string "<led8><led13>"; e.g. "11": both leds on
 * if no arg equals "00" 
 * For "random" blink you may run following commands in the command line:
 * while [ 1 ]; do ./mmap $(($RANDOM % 2))$(($RANDOM % 2)); done
 */
int main(int argc, char** argv) {
  struct mmapData* p_mmapData; // here our mmapped data will be accessed
  int fd_mmapFile; // file descriptor for memory mapped file

  /* Create shared memory object and set its size */
  fd_mmapFile = open(mmapFilePath, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
  if (fd_mmapFile == -1) exitError("fd error; check errno for details");
  
  /* Map shared memory object read-writable */
  p_mmapData = static_cast<struct mmapData*>(mmap(NULL, sizeof(struct mmapData), PROT_READ | PROT_WRITE, MAP_SHARED, fd_mmapFile, 0));
  if (p_mmapData == MAP_FAILED) exitError("mmap error");
  /* the Arduino sketch might still be reading - by locking this program will be blocked until the mutex is unlocked from the reading sketch 
   * in order to prevent race conditions */
  if (pthread_mutex_lock(&(p_mmapData->mutex)) != 0) exitError("pthread_mutex_lock");
  if (argc == 1) { 
    cout << "8:0" << endl;
    cout << "13:0" << endl;
    p_mmapData->led8_on = false;
    p_mmapData->led13_on = false;
  }
  else if (argc > 1) {
    // assert(correct string given)
    int binNr = atol(argv[1]);
    if (binNr >= 10) {
      cout << "8:1" << endl;
      p_mmapData->led8_on = true; 
    }
    else {
      cout << "8:0" << endl;
      p_mmapData->led8_on = false;
    }
    binNr %= 10;
    if (binNr == 1) {
      cout << "13:1" << endl;
      p_mmapData->led13_on = true; 
    }
    else {
      cout << "13:0" << endl;
      p_mmapData->led13_on = false;
    }
  }
  // signal to waiting thread
  if (pthread_mutex_unlock(&(p_mmapData->mutex)) != 0) exitError("pthread_mutex_unlock");
  if (pthread_cond_signal(&(p_mmapData->cond)) != 0) exitError("pthread_cond_signal");
}