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


/* Utilities to handle dynamic arrays of strings */

#ifdef __cplusplus
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wwrite-strings"
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wwrite-strings"
#endif
#endif /* __cplusplus */


/* Project headers */
#include "sargv.h"


#if !defined(SMAX)
#define SMAX(a, b)  (((a) > (b)) ? (a) : (b))
#endif


static int argscmp (const void * _a, const void * _b)
{
  return strcmp (* (char **) _a, * (char **) _b);
}


/* Lookup for an element into the table of arguments */
int argsmember (char * argv [], char * item)
{
  int i = -1;

  while (item && argv && * argv)
    if (! strcmp (* argv ++, item))
      return i + 1;
    else
      i ++;

  return -1;
}


/* Return the # of items in an array */
unsigned argslen (char * argv [])
{
  return valen ((void **) argv);
}


/* Add an element to an array */
char ** argsmore (char * argv [], char * s)
{
  return (char **) vamore ((void **) argv, strdup (s));
}


/* Remove an item from an array */
char ** argsless (char * argv [], char * s)
{
  int argc = 0;
  int i;
  int j;

  if ((i = argsmember (argv, s)) != -1)
    {
      argc = argslen (argv);
      free (argv [i]);                   /* free the argument */

      for (j = i; j < argc - 1; j ++)    /* move pointers back one position */
	argv [j] = argv [j + 1];

      argv [j] = NULL;                   /* terminate the table */

      if (argc > 1)
	argv = (char **) realloc (argv, argc * sizeof (char *));  /* the size is argc not argc-1 because of trailing NULL */
      else
	free (argv);
    }

  return argc > 1 ? argv : NULL;
}


/* Lookup by reference for an item into an array */
int argshas (char * argv [], char * item)
{
  return vahas ((void **) argv, item);
}


/* Free memory associated to an array */
char ** argsclear (char * argv [])
{
  return (char **) vaclear ((void **) argv, free);
}


/* Duplicate array 'argv' */
char ** argsdup (char * argv [])
{
  char ** dst = NULL;
  while (argv && * argv)
    dst = argsmore (dst, * argv ++);
  return dst;
}


/* Concatenate array 'b' to 'a' */
char ** argscat (char * a [], char * b [])
{
  return (char **) vacat ((void **) a, (void **) b);
}


char ** argssort (char * argv [])
{
  return (char **) vasort ((void **) argv, argscmp);
}


/* Join items in argv[] */
char * argsjoin (char * argv [])
{
  unsigned size = 0;
  char * join = NULL;
  while (argv && * argv)
    {
      size += (strlen (* argv) + 1 + 1);  /* '\n' plus a blank separator */
      if (join)
	{
	  strncat (join, " ", 1);
	  join = (char *) realloc (join, size);
	  strcat (join, * argv ++);
	}
      else
	{
	  join = (char *) calloc (size + 1, 1);  /* one more char to avoid strncat overflow */
	  strcpy (join, * argv ++);
	}
    }
  return join;
}


/* Split a string into pieces */
char ** argssplit (char * str, char * sep)
{
  char ** argv = NULL;
  char * rest = NULL;
  char * param;
  char * data;
  char * m;

  if (! str || ! sep)
    return NULL;

  data = strdup (str);                       /* this is due strtok_r() modifies the input buffer 'str' */
  m = data;

  param = strtok_r (data, sep, & rest);
  while (param)
    {
      /* Add current field to the array */
      argv = argsmore (argv, param);

      /* Process empty fields (add the separator) */
      if (rest && * rest == * sep)
	{
	  char * p = rest;
	  while (* p ++ == * sep)
	    argv = argsmore (argv, sep);
	}
      /* Next field */
      param = strtok_r (NULL, sep, & rest);
    }

  if (m)
    free (m);

  return argv;
}


/* Split a string into pieces */
char ** argspieces (char * list, char * sep)
{
  char ** argv = NULL;
  char * param;
  char * names = list ? strdup (list) : NULL;

  while (names && (param = strtok (! argv ? names : NULL, sep)))
    argv = argsmore (argv, param);

  if (names)
    free (names);

  return argv;
}


/* Split a blank separated list of strings into pieces */
char ** argsblanks (char * list)
{
  return argspieces (list, " ");
}


/* Lookup for a name in a table */
bool argsexists (char * argv [], char * name)
{
  while (name && argv && * argv)
    if (! strcmp (* argv ++, name))
      return true;
  return false;
}


/* Add an item to a table (if not already in) */
char ** argsuniq (char * argv [], char * item)
{
  return argsexists (argv, item) ? argv : argsmore (argv, item);
}


/* Print the arguments in 'argc' rows (one argument for line) */
void argsrows (char * argv [])
{
  unsigned argc = 0;
  while (argv && * argv)
    printf ("%3d. \"%s\"\n", ++ argc, * argv ++);
}


/* Find the longest name */
unsigned argslongest (char * argv [])
{
  unsigned longest = 0;

  while (argv && * argv)
    {
      longest = SMAX (longest, strlen (* argv));
      argv ++;
    }
  return longest;
}


/* fit on a 25x80 terminal */
void argscols (char * argv [])
{
  unsigned argc = arrlen (argv);
  unsigned rows, cols;
  unsigned i, j;

  unsigned max = argslongest (argv);

  if (! max)
    return;

  /* how many columns? */
  cols = 80 / ((max + 8) &~ 7);
  if (cols == 0)
    cols = 1;
  rows = (argc + cols - 1) / cols;

  for (i = 0; i < rows; i ++)
    {
      for (j = 0; j < cols; j ++)
	if ((i + j * rows) < argc)
	  printf ("%-*.*s", max + 1, max + 1, argv [i + j * rows]);
	else
	  break;
      printf ("\n");
    }
}


/* fit on a 25x80 terminal by rows */
void args_2d_rows (char * argv [])
{
  unsigned argc = arrlen (argv);
  unsigned rows, cols;
  unsigned i, j;

  unsigned max = argslongest (argv);

  if (! max)
    return;

  /* how many columns? */
  cols = 80 / ((max + 8) &~ 7);
  if (cols == 0)
    cols = 1;
  rows = (argc + cols - 1) / cols;

  for (i = 0; i < rows; i ++)
    {
      for (j = 0; j < cols; j ++)
	if (i * cols + j < argc)
	  printf ("%-*.*s", max + 1, max + 1, argv [i * cols + j]);
	else
	  break;
      printf ("\n");
    }
}


/* Print all lines in [argv] with a newline */
void print_lines (char * argv [])
{
  if (argv)
    while (* argv)
      printf ("%s\n", * argv ++);
}
