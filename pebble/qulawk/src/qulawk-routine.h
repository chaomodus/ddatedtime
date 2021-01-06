#ifndef QULAWK_ROUTINE_H
#define QULAWK_ROUTINE_H

#ifdef PEBBLE
#include <pebble.h>
#else
#include <time.h>
#endif /* PEBBLE */

struct qulawk_t
{
  unsigned char session;
  unsigned char period;
  unsigned char bit;
  unsigned char moment;
};

extern const char *sessionNames[];
extern const char *periodNames[];


void xlate_qulawk(struct qulawk_t *qulawk, struct tm *tm, int microseconds);

#endif /* QULAWK_ROUTINE_H */
