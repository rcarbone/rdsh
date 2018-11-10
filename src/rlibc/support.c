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


/* Project headers */
#include "support.h"
#include "rwall.h"


/*
 * http://stackoverflow.com/questions/6127503/shuffle-array-in-c
 *
 * Arrange the N elements of ARRAY in random order.
 * Only effective if N is much smaller than RAND_MAX;
 * if this may not be the case, use a better random
 * number generator.
 */
static void shuffle (unsigned n, unsigned * array)
{
  srand (nswall ());

  if (n > 1)
    {
      unsigned i;
      unsigned j;
      unsigned t;
      for (i = 0; i < n - 1; i ++)
        {
          j = i + rand () / (RAND_MAX / (n - i) + 1);
          t = array [j];
          array [j] = array [i];
          array [i] = t;
        }
    }
}


/* Evaluate a random order to run implementation */
unsigned * rndorder (unsigned n)
{
  unsigned * array = (unsigned *) calloc (n, sizeof (unsigned));
  unsigned i;
  for (i = 0; i < n; i ++)
    array [i] = i;
  shuffle (n, array);
  return array;
}


/* Convert an integer to a string (more efficient than sprintf) */
#define BASE 10
void itoa (int c, char * ret)
{
  static const char * exa = "0123456789abcdef";

  if (c == 0)
    ret [0] = '0',
      ret [1] = '\0';
  else
    {
      int l, x, y;
      char buf [16];

      for (l = 0, x = c < 0 ? -c : c; x > 0; x /= BASE)
        buf [l ++] = exa [x % BASE];

      if (c < 0)
        buf [l ++] = '-';

      for (x = l - 1, y = 0; x >= 0; x --)
        ret [y ++] = buf [x];

      ret [y] = '\0';
    }
}


/* Unsigned to Ascii */
char * utoa (unsigned n)
{
  /* Limits for static buffers in utoa() */
#define ITEMS 10      /* rows */
#define SIZE  16      /* cols */

  static char text [ITEMS] [SIZE];
  static unsigned i = 0;

  char * s = text [i ++ % ITEMS];
  sprintf (s, "%u", n);
  return s;
}


/* Evaluate the number of digits in n */
unsigned digits (unsigned n)
{
  return n < 10 ? 1 : 1 + digits (n / 10);
}


/* Evaluate the number of digits in n */
char * tentoa (unsigned n)
{
  static char txt [1024];

  if (n % 10)
    sprintf (txt, "%s", utoa (n));
  else
    sprintf (txt, "1e%s", utoa (digits (n) - 1));

  return txt;
}


void print_dots (char * name, char * label, unsigned n, unsigned seq, unsigned maxn)
{
  unsigned dots = name ? strlen (name) : 0;
  unsigned i;
  char fmt [1024];
  char str [1014];

  sprintf (fmt, "[%%%uu] %s %%s ", n, label);
  sprintf (str, fmt, seq, name);

  if (dots < maxn)
    dots = maxn - dots + 3;
  else
    dots = 3;

  printf ("%s", str);

  for (i = 0; i < dots; i ++)
    printf (".");
  printf (" ");
}


/* Well formatted percentage */
char * xpercentage (unsigned long partial, unsigned long total)
{
#define DECIMALS 2
#define ITEMS 10
  static char buffer [ITEMS] [64];
  static short k = -1;

  k = (k + 1) % ITEMS;

  if (partial && total)
    {
      float percent = (float) partial * 100 / (float) total;

      if (partial == total)
	sprintf (buffer [k], "(%3d%%)", (int) percent);
      else
	sprintf (buffer [k], "(%4.*f%%)", DECIMALS, percent);  /* vv.dd% */
    }
  else
    strcpy (buffer [k], "");    /* just the empty string */

  return buffer [k];
}
