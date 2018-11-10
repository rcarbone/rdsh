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
#include <stdlib.h>
#include <string.h>


#define BUFFERS    128
#define BUFFERSIZE 512

#define SPACE ' '


/* Format a centered string */
char * scenter (char * s, unsigned max)
{
  static char centered [BUFFERS] [BUFFERSIZE];
  static short i = -1;

  int x;       /* initial blanks    */
  unsigned y;  /* the string itself */
  int z;       /* final blanks      */
  char * a;
  char * b;
  char * c;

  i = (i + 1) % BUFFERS;   /* round-robin in the array of local buffers */

  /* Blank line */
  if (! s)
    {
      sprintf (centered [i], "%*c", max, ' ');
      return centered [i];
    }

  y = strlen (s);
  x = (max - y) / 2;
  z = max - x - y;

  a = calloc (1, max + 1);
  b = calloc (1, max + 1);
  c = calloc (1, max + 1);

  if (x > 0)
    sprintf (a, "%*c", x, ' ');         /* header */
  if (y < max)
    sprintf (b, "%-*.*s", y, y, s);     /* string */
  else
    sprintf (b, "%-*.*s", max, max, s);
  if (z > 0)
    sprintf (c, "%*c", z, ' ');         /* footer */

  sprintf (centered [i], "%s%s%s", a, b, c);

  free (a); free (b); free (c);

  return centered [i];
}


void printcenter (char * s)
{
  if (s)
    printf ("%s", scenter (s, strlen (s)));
}


/* Print a string centered in n characters */
void rcenter (char * s, int n)
{
  int x;  /* # of initial blanks */
  int y;  /* the string itself */
  int z;  /* # of final blanks */

  if (! s || ! n)
    {
      printf ("%*c", n, SPACE);
      return;
    }

  y = strlen (s);
  x = (n - y) / 2;
  z = n - x - y;

  if (x > 0)
    printf ("%*c", x, SPACE);       /* header */
  if (y < n)
    printf ("%-*.*s", y, y, s);     /* string */
  else
    printf ("%-*.*s", n, n, s);
  if (z > 0)
    printf ("%*c", z, SPACE);       /* footer */
}
