#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <libgen.h>
#include <time.h>
#include <getopt.h>
#include "ddate-routine.h"

const unsigned short seasonStarts[] = {0, 3, 1, 4, 2};

#define DCAL_MODE 0
#define DDATE_MODE 1

void print_ddate(struct ddate_t *addate)
{
  printf("Season: %d Day: %d Year: %d\n", addate->season, addate->day_of_season, addate->d_year);
}

int istrlen(int someint)
{
  if (someint == 0)
    return 1;
  return ceil(log10(abs(someint)))+(someint < 0);
}

int main(int argc, char **argv)
{
  char *progname;
  int mode = DCAL_MODE;
  int i,j;
  int dayoffs;
  struct ddate_t myddate;
  struct tm mytm;
  time_t mytime;
  int userday=-1, usermonth=-1, useryear=-1;
  int userseason=-1;
  int foolen = 0;
  int padding = 0;

  mytime = time(NULL);

  if (localtime_r(&mytime, &mytm) == NULL)
    {
      exit(-1);
    }

  progname = basename(strdup(argv[0]));

  if (strncmp(progname, "ddate", 5) == 0)
    {
      mode = DDATE_MODE;
    }

  xlate_ddate(&myddate, mytm.tm_yday, mytm.tm_year + 1900, mytm.tm_mon, mytm.tm_mday);

  if (mode == DDATE_MODE)
    {
      print_ddate(&myddate);
    }
  else
    {
      /* draw header */
      foolen = strlen(longSeasonNames[myddate.season-1]) + istrlen(myddate.d_year) + 1;
      padding = 16.5 - (foolen/2);
      for (i = 1; i < padding; i++)
      {
	  putchar(' ');
      }
      printf("%s %d\n\n", longSeasonNames[myddate.season-1], myddate.d_year);
      dayoffs = seasonStarts[myddate.season - 1];
      putchar(' ');
      for (i=1; i < 11; i++)
	{
	  dayoffs += 1;

	  printf("%s%s",shortDayNames[dayoffs-1], dayoffs == 5 ? "|" : " ");

	  if (dayoffs == 5)
	    {
	      dayoffs = 0;
	    }
	}

      fputs("\n ",stdout);

      dayoffs = seasonStarts[myddate.season - 1];
      for (i=1; i < 11; i++)
	{
	  dayoffs += 1;

	  printf("--%s",dayoffs == 5 ? "+" : "-");

	  if (dayoffs == 5)
	    {
	      dayoffs = 0;
	    }
	}

      fputs("\n ", stdout);

      dayoffs = seasonStarts[myddate.season - 1];
      /* draw body */
      for (i=0; i < 8; i++)
	{
	  for (j=1; j < 11; j++)
	    {
	      if ((i == 7) && (j > 3))
		;
	      else
		{
		  dayoffs = dayoffs + 1;
		  printf("%2d",(i*10)+j);
		  if (((i * 10) + j) == myddate.day_of_season)
	          {
		    putchar(']');
		    if (dayoffs == 5)
		    {
			dayoffs = 0;
		    }
 		  }
		  else if (((i * 10) + j) == myddate.day_of_season-1)
	            {
		        putchar('[');
		        if (dayoffs == 5)
		        {
			  dayoffs = 0;
	                }
                     }

		  else if (dayoffs == 5)
		    {
		      putchar('|');
		      dayoffs = 0;
		    }
		  else
		    putchar(' ');
		}
	    }
	  fputs("\n ",stdout);
	}
    }
  putchar('\n');

}
