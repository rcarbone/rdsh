/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 *                   _ _ _
 *              _ __| (_) |__   ___
 *             | '__| | | '_ \ / __|
 *             | |  | | | |_) | (__
 *             |_|  |_|_|_.__/ \___|
 *
 *             rdsh, the Redis shell
 *
 * R. Carbone (rocco@tecsiel.it)
 *   4Q 2018
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 */


/* System headers */
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>


/* Project headers */
#include "interval.h"


/* useful macros */
#define SECS_PER_DAY   86400
#define SECS_PER_HOUR  3600
#define SECS_PER_MIN   60
#define MSEC_PER_SEC   1000      /* 1e3 */
#define USEC_PER_SEC   1000000   /* 1e6 */
#define USEC_PER_MIN   (1e6 * SECS_PER_MIN)
#define USEC_PER_HOUR  (SECS_PER_HOUR * USEC_PER_SEC)


/* Return time in days */
static int timeindays (time_t t)
{
  return (t / (MSEC_PER_SEC * SECS_PER_DAY));
}


/* Return time in hours */
static int timeinhours (time_t t)
{
  return (t / (MSEC_PER_SEC * SECS_PER_HOUR)) % 24;
}


/* Return time in minutes */
static time_t timeinmins (time_t t)
{
  return (t / (MSEC_PER_SEC * 60)) % 60;
}


/* Return time in seconds */
static time_t timeinsecs (time_t t)
{
  return (t / MSEC_PER_SEC) % SECS_PER_MIN;
}


/* Return time in milliseconds */
static time_t timeinmsecs (time_t t)
{
  return t % MSEC_PER_SEC;
}


/* Return time in microseconds */
static time_t timeinusecs (time_t t)
{
  return (time_t) t % USEC_PER_SEC;
}


/* The time difference in days */
int _days_ (time_t t2, time_t t1)
{
  return (t2 - t1) / SECS_PER_DAY;
}


/* The time difference in hours */
int _hours_ (time_t t2, time_t t1)
{
  return (t2 - t1 - (_days_ (t2, t1) * SECS_PER_DAY)) / SECS_PER_HOUR;
}


/* The time difference in minutes */
int _mins_ (time_t t2, time_t t1)
{
  return (t2 - t1 - (_days_ (t2, t1) * SECS_PER_DAY) - (_hours_ (t2, t1) * SECS_PER_HOUR)) / SECS_PER_MIN;
}


/* Return 1 is t1 == t2 at seconds resolution */
bool samet (struct timeval * t2, struct timeval * t1)
{
  return t2 -> tv_sec == t1 -> tv_sec;
}


/* The time since [t] in seconds */
time_t seconds (struct timeval * t)
{
  return t -> tv_sec + t -> tv_usec / 1000000.0;
}


/* The time since [t] in milliseconds */
time_t milliseconds (struct timeval * t)
{
  return t -> tv_sec * 1e3 + t -> tv_usec / 1e3;
}


/* The time since [t] in microseconds */
time_t microseconds (struct timeval * t)
{
  return t -> tv_sec * 1e6 + t -> tv_usec;
}


/* The time difference in days */
int days (struct timeval * t2, struct timeval * t1)
{
  return _days_ (t2 -> tv_sec, t1 -> tv_sec);
}


/* The time difference in hours */
int hours (struct timeval * t2, struct timeval * t1)
{
  return _hours_ (t2 -> tv_sec, t1 -> tv_sec);
}


/* The time difference in minutes */
int mins (struct timeval * t2, struct timeval * t1)
{
  return _mins_ (t2 -> tv_sec, t1 -> tv_sec);
}


/* The time difference in seconds */
int secs (struct timeval * t2, struct timeval * t1)
{
  return (t2 -> tv_sec - t1 -> tv_sec) % 60;
}


/* The time difference in milliseconds */
time_t msecs (struct timeval * t2, struct timeval * t1)
{
  /* Compute delta in second, 1/10's and 1/1000's second units */
  time_t delta_seconds      = t2 -> tv_sec - t1 -> tv_sec;
  time_t delta_milliseconds = (t2 -> tv_usec - t1 -> tv_usec) / 1e3;

  if (delta_milliseconds < 0)
    { /* manually carry a one from the seconds field */
      delta_milliseconds += 1e3;
      -- delta_seconds;
    }
  return (delta_seconds * 1e3) + delta_milliseconds;
}


/* The time difference in microseconds */
time_t usecs (struct timeval * t2, struct timeval * t1)
{
  /* Compute delta in second, 1/10's and 1/1000's second units */
  time_t delta_seconds      = t2 -> tv_sec - t1 -> tv_sec;
  time_t delta_microseconds = t2 -> tv_usec - t1 -> tv_usec;

  if (delta_microseconds < 0)
    { /* manually carry a one from the seconds field */
      delta_microseconds += 1e6;
      -- delta_seconds;
    }
  return (delta_seconds * 1e6) + delta_microseconds;
}


void print_time_in_secs (struct timeval * t, char * label)
{
  printf ("%s%*.*s\n", label, 24, 24, ctime (& t -> tv_sec));
}


/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */

struct timeval * tvnow (void)
{
  static struct timeval now;
  gettimeofday (& now, NULL);
  return & now;
}


/* Number of microseconds since 00:00:00 January 1, 1970 UTC */
time_t timenow (void)
{
  struct timeval now;
  gettimeofday (& now, NULL);
  return (time_t) now . tv_sec * USEC_PER_SEC + now . tv_usec;
}


/* Return a well formatted string with a time difference at millisecond resolution */
char * elapsedtime (struct timeval * start, struct timeval * stop)
{
  static char et [64];

  time_t elapsed = msecs (stop, start);

  if (timeindays (elapsed))
    sprintf (et, "%d days, %02d:%02ld:%02ld.%03ld",
	     timeindays (elapsed), timeinhours (elapsed), timeinmins (elapsed), timeinsecs (elapsed), timeinusecs (elapsed));
  else if (timeinhours (elapsed))
    sprintf (et, "%2d:%02ld:%02ld.%03ld",
	     timeinhours (elapsed), timeinmins (elapsed), timeinsecs (elapsed), timeinusecs (elapsed));
  else if (timeinmins (elapsed))
    sprintf (et, "%2ld:%02ld.%03ld", timeinmins (elapsed), timeinsecs (elapsed), timeinusecs (elapsed));
  else if (timeinsecs (elapsed))
    sprintf (et, "%ld.%03ld secs", timeinsecs (elapsed), timeinmsecs (elapsed));
  else
    sprintf (et, "%3ld msecs", timeinmsecs (elapsed));

  return et;
}
