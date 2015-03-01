#include "qulawk-routine.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
  struct tm now;
  time_t tnow;
  struct qulawk_t qnow;

  tnow = time(NULL);

  gmtime_r(&tnow, &now);

  xlate_qulawk(&qnow, &now, 0);

  printf("%i:%i'%i.%i\n", qnow.session, qnow.period, qnow.bit, qnow.moment);
}
