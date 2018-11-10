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

/* Constants */
#define NAME   "Close a physical connection to a Redis server."


/* GNU short options */
enum
{
  /* Miscellanea */
  OPT_HELP  = 'h',
  OPT_QUIET = 'q'

} ropt_t;


/* GNU long options */
static struct option lopts [] =
{
  /* Miscellanea */
  { "help",  no_argument, NULL, OPT_HELP  },
  { "quiet", no_argument, NULL, OPT_QUIET },

  { NULL,    0,           NULL, 0         }
};


/* Display the syntax for using the builtin */
static void usage (char * name, struct option * options)
{
  /* Get the longest option name for optimal rendering */
  unsigned n = optmax (options);

  printf ("%s, %s\n", name, NAME);
  printf ("Usage: %s [options]\n", name);
  printf ("\n");

  printf ("Startup:\n");
  usage_item (options, n, OPT_HELP,  "show this help message");
  usage_item (options, n, OPT_QUIET, "run quietly");
}


/* Builtin extension */
int rdsh_disconnect (int argc, char * argv [])
{
  char * progname = basename (argv [0]);
  char * sopts    = optlegitimate (lopts);
  bool quiet      = false;
  rdsh_t * sess;
  int option;

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
	case OPT_HELP:  usage (progname, lopts); return 0;
	case OPT_QUIET: quiet = true;            break;
	}
    }

  /* Retrieve the handle of the active session (if any) */
  sess = active_session ();
  if (sess)
    {
      if (! quiet)
	printf ("%s: disconnecting from Redis '%s' ... ", progname, redis_name (sess));

      redis_disconnect (sess);

      if (! quiet)
	printf ("Ok!\n");

      /* Keep track of the last active session */
      resetactive_session (sess);

      /* Remove from the table of sessions */
      lesssession (sess);

      /* Update user prompt to exclude the no more active session */
      rdsh_prompt (redis_name (active_session ()));
    }
  else
    printf ("%s: no session.\n", progname);

  return 0;
}
