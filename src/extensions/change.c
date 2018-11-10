/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 *                      _     _
 *              _ __ __| |___| |__
 *             | '__/ _` / __| '_ \
 *             | | | (_| \__ \ | | |
 *             |_|  \__,_|___/_| |_|
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

/* Project headers */
#include "rdsh.h"


/* GNU short options */
enum
{
  /* Miscellanea */
  OPT_HELP    = 'h',

} ropt_t;


/* GNU long options */
static struct option lopts [] =
{
  /* Miscellanea */
  { "help",    no_argument, NULL, OPT_HELP    },

  { NULL,      0,           NULL, 0           }
};


/* Display the syntax for using the builtin */
static void usage (char * name, char * synopsis, char * help, struct option * options)
{
  /* longest option name */
  unsigned n = optmax (options);

  printf ("\n");
  printf ("Usage: %s - %s\n", name, synopsis);
  printf ("       %s [options]\n", name);
  printf ("\n");

  printf ("Options are:\n");
  usage_item (options, n, OPT_HELP,    "show this help message");
}


/* Change session */
int rdsh_change (int argc, char * argv [])
{
  char * progname = basename (argv [0]);
  char * sopts    = optlegitimate (lopts);
  rdsh_t * sess;
  int option;

  /* Lookup for the command in the table of extensions */
  builtin_t * b = builtin (progname);
  if (! b)
    {
      printf ("%s: Builtin [%s] not found.\n", progname, progname);
      return 1;
    }

  /* Parse command line options */
  optind = 0;
  optarg = NULL;
  argv [0] = progname;
  while ((option = getopt_long (argc, argv, sopts, lopts, NULL)) != -1)
    {
      switch (option)
	{
	default: printf ("Try '%s --help' for more information.\n", progname); return 1;

	  /* Miscellanea */
	case OPT_HELP:    usage (progname, b -> synopsis, b -> help, lopts);   return 0;
	}
    }

#if defined(ROCCO)
  /* Check for mandatory arguments */
  if (argc < optind + b -> args)
    {
      printf ("%s: Missing mandatory arguments.\n", progname);
      usage (progname, b -> synopsis, b -> help, lopts);
      return 1;
    }
#endif /* ROCCO */

  /* Retrieve the handle of the active session (if any) */
  sess = active_session ();
  if (sess)
    {
      rdsh_t * prev = previous_session ();

      if (! prev)
	{
	  printf ("%s: session stack empty.\n", progname);
	  return -1;
	}

      /* Swap now */
      setactive_session (prev);

      /* Update user prompt to include the active session */
      rdsh_prompt (redis_name (prev));
    }
  else
    printf ("%s: no session.\n", progname);

  return 0;
}
