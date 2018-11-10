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
#include <stdint.h>
#include <limits.h>

/* Project headers */
#include "rwall.h"
#include "elapsed.h"


/* Evaluate decimal value of t according to n (eg. t=123456 n=1000 => 4.5) */
static rtime_t ntrunc (rtime_t t, unsigned n)
{
  return (t - (t / n) * n) / (n / 10);            /* just 1 digit */
}


/* Convert nanoseconds in microseconds */
static rtime_t n2us (rtime_t t)
{
  return t / 1000;               /* 1e3 */
}


/* Convert nanoseconds in millseconds */
static rtime_t n2ms (rtime_t t)
{
  return t / 1000000;            /* 1e6 */
}


/* Convert nanoseconds in seconds */
static rtime_t n2ss (rtime_t t)
{
  return t / 1000000000;         /* 1e9 */
}


/* Convert nanoseconds in minutes */
static rtime_t n2mm (rtime_t t)
{
  return t / 1000000000 / 60;
}


/* Convert nanoseconds in hours */
static rtime_t n2hh (rtime_t t)
{
  return t / 1000000000 / 60 / 60;
}


/* Convert nanoseconds in days */
static rtime_t n2dd (rtime_t t)
{
  return t / 1000000000 / 60 / 60 / 24;
}


/* Eval seconds in minutes */
static unsigned ssinn (rtime_t t)
{
  return n2ss (t - (n2mm (t) * 60 * 1000000000));
}


/* Eval minutes in hours
 *
 * eval:
 *  hours in t       => hh = n2hh (t)
 *  ns in hours      => ns = n2hh (t) * 60 * 60 * 1000000000
 *  delta ns         => dt = t - n2hh (t) * 60 * 60 * 1000000000
 *  minutes in delta => mm = n2mm (t - n2hh (t) * 60 * 60 * 1000000000)
 *
 */
static unsigned mminn (rtime_t t)
{
  return n2mm (t - (n2hh (t) * 60 * 60 * 1000000000));
}


static unsigned hhinn (rtime_t t)
{
  return n2hh (t - (n2dd (t) * 24 * 60 * 60 * 1000000000));
}


/* Return a well formatted string rapresenting given time at nanosecond resolution */
char * ns2a (rtime_t nsecs)
{
  static char elapsed [64];

  if (nsecs < 1e3)
    sprintf (elapsed, "%3ju nsec", nsecs);
  else if (nsecs < 1e6)
    sprintf (elapsed, "%3ju.%1ju usec", n2us (nsecs), ntrunc (nsecs, 1000));
  else if (nsecs < 1e9)
    sprintf (elapsed, "%3ju.%1ju msec", n2ms (nsecs), ntrunc (nsecs, 1000000));
  else if (nsecs < 60 * 1e9)
    sprintf (elapsed, "%3ju.%1ju  sec", n2ss (nsecs), ntrunc (nsecs, 1000000000));
  else if (nsecs < 60 * 60 * 1e9)
    sprintf (elapsed, "%3ju:%02u min", n2mm (nsecs), ssinn (nsecs));
  else if (nsecs < 24 * 60 * 60 * 1e9)
    sprintf (elapsed, "%3ju:%02u hour", n2hh (nsecs), mminn (nsecs));
  else
    sprintf (elapsed, "%3ju, %02u  day", n2dd (nsecs), hhinn (nsecs));

  return elapsed;
}


char * relapsed (rtime_t nsecs)
{
  return ns2a (nswall () - nsecs);
}


#if defined(TEST)

int main (int argc, char * argv [])
{
  rtime_t t;

  /* ===================== */

  /* 1 nanosecond (lower limit) */
  t = 1;
  printf ("%16ju -> [%s]\n", t, ns2a (t));

  /* 999 nanoseconds (upper limit) */
  t = 1e3 - 1;
  printf ("%16ju -> [%s]\n", t, ns2a (t));

  printf ("\n");

  /* ===================== */

  /* 1 microsecond (lower limit) */
  t = 1e3;
  printf ("%16ju -> [%s]\n", t, ns2a (t));

  /* about half */
  t = 50 * 1e3 + 123456;
  printf ("%16ju -> [%s]\n", t, ns2a (t));

  /* 999 microseconds (upper limit) */
  t = 1e6 - 1;
  printf ("%16ju -> [%s]\n", t, ns2a (t));

  printf ("\n");

  /* ===================== */

  /* 1 millisecond (lower limit) */
  t = 1e6;
  printf ("%16ju -> [%s]\n", t, ns2a (t));

  /* about half */
  t = 50 * 1e6 + 123456789;
  printf ("%16ju -> [%s]\n", t, ns2a (t));

  /* 999 milliseconds (upper limit) */
  t = 1e9 - 1;
  printf ("%16ju -> [%s]\n", t, ns2a (t));

  printf ("\n");

  /* ===================== */

  /* 1 second (lower limit) */
  t = 1e9;
  printf ("%16ju -> [%s]\n", t, ns2a (t));

  /* about half */
  t = 30 * 1e9 + 987654321;
  printf ("%16ju -> [%s]\n", t, ns2a (t));

  /* 59 seconds (upper limit) */
  t = 60 * 1e9 - 1;
  printf ("%16ju -> [%s]\n", t, ns2a (t));

  printf ("\n");

  /* ===================== */

  /* 1 minute (lower limit) */
  t = 60 * 1e9;
  printf ("%16ju -> [%s]\n", t, ns2a (t));

  /* about half */
  t = 30 * 60 * 1e9 - 1;
  printf ("%16ju -> [%s]\n", t, ns2a (t));

  /* 59 minutes (upper limit) */
  t = 60 * 60 * 1e9 - 1;
  printf ("%16ju -> [%s]\n", t, ns2a (t));

  printf ("\n");

  /* ===================== */

  /* 1 hour (lower limit) */
  t = 60 * 60 * 1e9;
  printf ("%16ju -> [%s]\n", t, ns2a (t));

  /* about half */
  t = 12 * 60 * 60 * 1e9 - 1;
  printf ("%16ju -> [%s]\n", t, ns2a (t));

  /* 23 hours (lower limit) */
  t = 24 * 60 * 60 * 1e9 - 1;
  printf ("%16ju -> [%s]\n", t, ns2a (t));

  printf ("\n");

  /* ===================== */

  /* 1 day (lower limit) */
  t = 1 * 24 * 60 * 60 * 1e9;
  printf ("%16ju -> [%s]\n", t, ns2a (t));

  /* 30 days (lower limit) */
  t = 30 * 24 * 60 * 60 * 1e9;
  printf ("%16ju -> [%s]\n", t, ns2a (t));

  return 0;
}

/* gcc -DTEST elapsed.c -o elapsed && ./elapsed */

#endif /* TEST */
