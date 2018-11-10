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
#include <stdlib.h>

/* Public header */
#include "rwall.h"

#if !defined(CLOCK_MONOTONIC_RAW)
#define CLOCK_MONOTONIC_RAW CLOCK_MONOTONIC
#endif


static rtime_t ts2ns (struct timespec ts)
{
  return ts . tv_sec * 1e9 + ts . tv_nsec;
}


/* Return the current wall time in nanoseconds */
rtime_t nswall (void)
{
  struct timespec ts;
  clock_gettime (CLOCK_MONOTONIC_RAW, & ts);
  return ts2ns (ts);
}


/* Return the current wall time in microseconds */
rtime_t uswall (void)
{
  struct timespec ts;
  clock_gettime (CLOCK_MONOTONIC_RAW, & ts);
  return ts2ns (ts) / 1e3;
}


/* Return the current wall time in milliseconds */
rtime_t mswall (void)
{
  struct timespec ts;
  clock_gettime (CLOCK_MONOTONIC_RAW, & ts);
  return ts2ns (ts) / 1e6;
}


/* Return k operations per second */
double kops (rtime_t delta, unsigned n)
{
  return (1e6 / (delta / n)) / 1e3;
}


/* Evaluate a random number in the range [0 n-1] */
unsigned rrand (unsigned n)
{
  if (! n)
    return 0;

  srand (nswall ());
  return rand () % n;
}
