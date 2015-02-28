/* 

   Fargus D-date

   Converts date into Discordian/Illuminati reconing.

   (C) Copyright 1999 by Nick A. Rusnov <nick@fargus.net>
 
 */

#include "ddate-routine.h"

void xlate_ddate(struct ddate_t *ddate, unsigned short yday, unsigned short year, unsigned short mon, unsigned short mday)
{
  unsigned short myyday;
  int work;

  if (!ddate)
    return;

  myyday = yday;
  
  if ((!yday) && (!year))
    {
      ddate->i_year = 0;
      ddate->sttibbs = 0;
      ddate->d_year = 0;
      ddate->season = 0;
      ddate->day_of_season = 0;
      ddate->day_of_week = 0;
      ddate->cycle = 0;
      ddate->day_of_cycle = 0;
      return;
    }

  if (!(year % 4) && (mon > 2))
    {
      myyday--;
    }
 
  if ((mon == 2) && (mday == 29))
    {
      ddate->sttibbs = 1;
      ddate->day_of_season = 0;
      ddate->day_of_week = 0;
      ddate->season = 1;
      ddate->cycle = 0;
      ddate->day_of_cycle = 0;
    }
  else
    {
      ddate->sttibbs = 0;
      ddate->season = (myyday / 73) + 1;
      ddate->day_of_season = (myyday - ((ddate->season - 1) * 73)) + 1;
      ddate->day_of_week = (myyday % 5) + 1;
      work = ddate->day_of_season - 1;
      if (work > 45)
	{
	  /* bicycle */
	  work -= 45;
	  ddate->cycle = (work / 14) + 1;
	  ddate->day_of_cycle = (work - ((ddate->cycle - 1) * 14)) + 1;
	  ddate->cycle += 3;
	}
      else
	{
	  /* tricycle */
	  ddate->cycle = (work / 15) + 1;
	  ddate->day_of_cycle = (work - ((ddate->cycle - 1) * 15)) + 1;
	}
    }
  ddate->i_year = 4000 + year;
  ddate->d_year = 1166 + year;
}
