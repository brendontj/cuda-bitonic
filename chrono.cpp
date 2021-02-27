// chrono.h
//
// A small library to measure time in programs
//
// by W.Zola (2017)

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>



#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#include <sys/time.h>     /* struct timeval definition           */
#include <unistd.h>       /* declaration of gettimeofday()       */

#include <time.h>


  typedef struct {

     struct timespec xadd_time1, xadd_time2;
     long long xtotal_ns;
     long xn_events;
    
  } chronometer_t;
 

  void chrono_reset( chronometer_t *chrono )
  {
      chrono->xtotal_ns = 0;
      chrono->xn_events = 0;
  }

  inline void chrono_start( chronometer_t *chrono ) {
      clock_gettime(CLOCK_MONOTONIC_RAW, &(chrono->xadd_time1) );
  }

  inline long long  chrono_getcount( chronometer_t *chrono ) {
      return chrono->xn_events;
  }

  inline void chrono_stop( chronometer_t *chrono ) {

      clock_gettime(CLOCK_MONOTONIC_RAW, &(chrono->xadd_time2) );
  
      long long ns1 = chrono->xadd_time1.tv_sec*1000*1000*1000 + 
                      chrono->xadd_time1.tv_nsec;
      long long ns2 = chrono->xadd_time2.tv_sec*1000*1000*1000 + 
                      chrono->xadd_time2.tv_nsec;
      long long deltat_ns = ns2 - ns1;
      
      chrono->xtotal_ns += deltat_ns;
      chrono->xn_events++;
  }

  void chrono_deltaT(chronometer_t *chrono) {
      printf("\n DeltaT(s): %.12f sec\n\n",chrono->xtotal_ns*1.0e-9);
  }