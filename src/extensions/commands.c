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

#if defined(ROCCO)

help with Unix command style

   printf ("NAME\n");
   printf ("\t%s - %s\n", name, help);
   printf ("\n");
   printf ("SYNOPSIS\n");
   printf ("\t%s\n", synopsis);
   printf ("\n");
   printf ("DESCRIPTION\n");
   printf ("\tRequires valid session.\n");
   printf ("\n");
   printf ("SEE ALSO\n");

#endif /* ROCCO */


/* GNU short options */
enum
{
  /* Miscellanea */
  OPT_HELP    = 'h',
  OPT_VERBOSE = 'v'

} ropt_t;


/* GNU long options */
static struct option lopts [] =
{
  /* Miscellanea */
  { "help",    no_argument, NULL, OPT_HELP    },
  { "verbose", no_argument, NULL, OPT_VERBOSE },

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
  usage_item (options, n, OPT_VERBOSE, "run verbosely");
}


/* Pass the command line options to Redis Server  */
int rdsh_command (int argc, char * argv [])
{
  char * progname = basename (argv [0]);
  char * sopts    = optlegitimate (lopts);
  bool verbose    = false;
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
	case OPT_VERBOSE: verbose = true;                                      break;
	}
    }

  /* Check for mandatory arguments */
  if (argc < optind + b -> args)
    {
      printf ("%s: Missing mandatory arguments.\n", progname);
      usage (progname, b -> synopsis, b -> help, lopts);
      return 1;
    }

  /* Retrieve the handle of the active session (if any) */
  sess = active_session ();
  if (sess)
    {
      if (verbose)
	printf ("Running command %s ...\n", progname);

      /* Replace command name */
      argv [0] = b -> command;

      /* Run the command here */
      redis_run (sess -> rds, b -> name, argc, argv);
    }
  else
    printf ("%s: no session.\n", progname);

  return 0;
}
