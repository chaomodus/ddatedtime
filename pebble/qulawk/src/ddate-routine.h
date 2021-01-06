#ifndef _DDATE_H
#define _DDATE_H

/*

  DDATE

  Converts date into Discordian/Illuminati reconing.

  (C) Copyright 1999 by Nick (Cas) A. Rusnov <nick@fargus.net>
  Updates 2015 by Cas Rusnov

 */

struct ddate_t
{
  unsigned char season; /* which season, 1-5 */
  unsigned char day_of_season; /* which day in the season, 1-73 */
  unsigned char sttibbs;  /* this is set to 1 if its St. Tibbs/Adam Weishaupt day (Feb 29) */
  unsigned char day_of_week; /* which day in seasonal week, 1-5 */
  unsigned char cycle; /* which bicycle/tricycle month, 1-3 (tricycle) 4-5 (bicycle) */
  unsigned char day_of_cycle; /* which day in the cycle 1-28 */
  short i_year; /* which year according to Illuminati (A=A) reconing */
  short d_year; /* which year according to LDD/ELF Discordian reconing */
};

extern const char *shortDayNames[];
extern const char *longDayNames[];
extern const char *shortSeasonNames[];
extern const char *longSeasonNames[];

void xlate_ddate(struct ddate_t *ddate, unsigned short yday, unsigned short year, unsigned short mon, unsigned short mday);

#endif
