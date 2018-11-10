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
#include <string.h>

#include "safe.h"
#include "sargv.h"


static void * x_safedup (void * a, void * b)
{
  safefree (b);
  return a ? strdup (a) : a;
}


int ireplace (int a, int b, int c)
{
  return a != -1 ? a : b != -1 ? b : c;
}


void * sreplace (void * a, void * b, void * c)
{
  return a ? x_safedup (a, c) : b ? x_safedup (b, c) : c;
}


char ** areplace (char * a [], char * b [], char * c [])
{
  if (a || b)
    argsclear (c);

  return a ? argsdup (a) : b ? argsdup (b) : c;
}
