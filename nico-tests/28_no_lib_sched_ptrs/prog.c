#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "lwp.h"
#include "handsoff.h"

#define INITIALSTACK 4096
#define NUMTHREADS 20
#define ITERS 1

#define tnext sched_one
#define tprev sched_two

static void indentnum(uintptr_t num);

typedef void (*sigfun)(int signum);

int main(int argc, char *argv[]){
  long i;

  lwp_set_scheduler(Handsoff);

  printf("Launching LWPS\n");

  /* spawn a number of individual LWPs */
  for(i=1;i<=NUMTHREADS;i++) {
    lwp_create((lwpfun)indentnum,(void*)i);
  }

  lwp_start();                     /* returns when the last lwp exits */

  for(i=1;i<=NUMTHREADS;i++) {
    lwp_wait(NULL);
  }

  printf("Back from LWPS.\n");
  return 0;
}

static void indentnum(uintptr_t num) {
  /* print the number num num times, indented by 5*num spaces
   * Not terribly interesting, but it is instructive.
   */
  int howfar,i;

  howfar=(int)num;              /* interpret num as an integer */
  for(i=0;i<howfar;i++){
    printf("%*d\n",howfar*3,howfar);
    lwp_yield();                /* let another have a turn */
  }
  lwp_exit(0);                   /* bail when done.  This should
                                 * be unnecessary if the stack has
                                 * been properly prepared
                                 */
}


