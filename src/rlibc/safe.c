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


/* Project headers */
#include "safe.h"


void * safefree (void * a)
{
  if (a)
    free (a);
  return NULL;
}


void * safedup (void * a)
{
  return a ? strdup ((char *) a) : a;
}


void * safereplace (void * a, void * b)
{
  safefree (b);
  return a ? strdup ((char *) a) : a;
}


FILE * safeopen (char * f)
{
  return ! strcmp (f, "*") ? NULL : ! strcmp (f, "-") ? stdout : fopen (f, "a+");
}


FILE * safeclose (FILE * fd)
{
  if (fd && fd != stdout)
    fclose (fd);
  return NULL;
}
