
#include "qulawk-routine.h"

const char *sessionNames[] = {"Hypnos", "Hemera", "Aether", "Hesperides", "Nyx"};
const char *periodNames[] = {"Geef", "Lep", "Shad", "Kwok", "Zin"};

void xlate_qulawk(struct qulawk_t *qulawk, struct tm *tm, int microseconds)
{
  float secs;
  /* first convert the gmtime tm into POEE Standard Time */
  tm->tm_hour -= 8;
  if (tm->tm_hour < 0)
    {
      tm->tm_hour += 24;
    }

  /* total seconds up */
  secs = tm->tm_sec;
  secs += (0.000001 * microseconds);
  secs += (60 * tm->tm_min);
  secs +=  (60 * 60 * tm->tm_hour);

  /* divide seconds into correct parts */
  qulawk->session = (int)(secs / 17280);
  secs = secs - (qulawk->session * 17280);
  qulawk->period = (int)(secs / 3456);
  secs = secs - (qulawk->period * 3456);
  qulawk->bit = (int)(secs / 138);
  secs = secs - (qulawk->bit * 138);
  qulawk->moment = (int)(secs / 5.32);
}
