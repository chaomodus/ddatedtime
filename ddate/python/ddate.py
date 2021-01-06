import time

(tm_year, tm_mon, tm_mday, tm_hour, tm_min, tm_sec, tm_wday, tm_yday, tm_isdst) = range(9)

WEEKDAYS = ('Sweetmorn','Boomtime','Pungenday','Prickle-Prickle','Setting Orange')
WEEKDAYS_ABBREV = ('SM','BT','PD','PP','SO')
SEASONS = ('Chaos','Discord','Confusion','Bureaucracy','The Aftermath')
SEASONS_ABBREV = ('Chs','Dsc','Cfn','Bcy','Afm')
FIVE_HOLYDAYS = (('Mungday', 'MD'), ('Mojoday','MD'), ('Syaday','SD'), ('Zaraday','ZD'), ('Maladay','MD'))
FIFTY_HOLYDAYS = (('Chaoflux','CF'), ('Discoflux','DF'), ('Confuflux','CF'),('Bureflux','BF'),('Afflux','AF'))

def is_leap_year(year):
    if (year % 400.0 == 0):
        return True
    if (year % 100.0 == 0):
        return False
    if (year % 4.0 == 0):
        return True
    return False

def ddate(time_t = None):
    if time_t is None:
        lt = time.localtime()
    else:
        lt = time.localtime(time_t)

    myday = lt[tm_yday] - 1
    isleap = is_leap_year(lt[tm_year])
    if (lt[tm_yday] > 60) and isleap:
        myday -= 1

    ddateYear = lt[0] + 1166;
    ddateSeason = myday / 73 + 1
    ddateDayOfSeason = (myday - ((ddateSeason - 1)  * 73)) + 1
    ddateDayOfWeek = (myday % 5) + 1
    work = ddateDayOfSeason - 1
    ddateCycle = 0
    ddateDayOfCycle = 0
    if (work > 45):
        work -= 45
        ddateCycle = (work / 14) + 1
        ddateDayOfCycle = (work - ((ddateCycle - 1) * 14)) + 1
    else:
        ddateCycle = (work / 15) + 1
        ddateDayOfCycle = (work - ((ddateCycle - 1) * 15)) + 1

    ddateStTibbs = 0
    if (lt[tm_yday] == 60) and isleap:
        ddateStTibbs = 1
        ddateDayOfWeek = -1
        ddateDayOfSeason = -1
        ddateDayOfCycle = -1

    return ddateStTibbs, ddateYear, ddateSeason, ddateDayOfSeason, ddateDayOfWeek, ddateCycle, ddateDayOfCycle

def get_holyday(season, day_of_season):
    if day_of_season == 5:
        return FIVE_HOLYDAYS[season-1]
    if day_of_season == 50:
        return FIFTY_HOLYDAYS[season-1]
    if day_of_season == -1:
        return "St. Tibb's Day", "STD"
    return "",""

def format(formatStr, time_t=None):
    """ Format codes:
%Y - ddate year
%S - long season name
%s - season abbrev.
%z - season number
%D - day of season
%W - long weekday name
%w - weekday abbrev.
%h - Holiday name
%H - Holiday abbrev.

%h - normal hour
%H - 24 hour hour
%m - normal minute
%c - normal second
%a - AM/PM
"""

    localtime = time.localtime(time_t)
    ddateStTibbs, ddateYear, ddateSeason, ddateDayOfSeason, ddateDayOfWeek, ddateCycle, ddateDayOfCycle = ddate(time_t)

    seasonLong =  SEASONS[ddateSeason - 1]
    seasonAbbrev = SEASONS_ABBREV[ddateSeason - 1]
    if not ddateStTibbs:
        dowLong = WEEKDAYS[ddateDayOfWeek - 1]
        dowShort = WEEKDAYS_ABBREV[ddateDayOfWeek - 1]
        dos = str(ddateDayOfSeason)
        holidayLong, holidayShort = get_holyday(ddateSeason, ddateDayOfSeason)
    else:
        dos = ''
        dowLong = "St. Tibb's Day"
        dowShort = "STD"
        holidayLong, holidayShort = get_holyday(ddateSeason, ddateDayOfSeason)
        ddateDayOfSeason = 'ST'

    ampm = 'AM'
    hr = localtime[tm_hour]
    if localtime[tm_hour] > 11:
        ampm = 'PM'
        if hr > 12:
            hr = hr - 12
    # fixme better formatting system
    return formatStr.replace("%Y",str(ddateYear)).replace("%S", seasonLong).replace("%s", seasonAbbrev).replace("%z",str(ddateSeason)).replace("%D", str(ddateDayOfSeason)).replace("%W",dowLong).replace("%w", dowShort).replace("%h", holidayLong).replace("%H",holidayShort).replace("%H", str(localtime[tm_hour])).replace("%h", str(hr)).replace("%m",str(localtime[tm_min])).replace("%c",str(localtime[tm_sec])).replace("%a",ampm)
