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
#define NAME   "Create a physical connection to a Redis server."


/* GNU short options */
enum
{
  /* Miscellanea */
  OPT_HELP  = 'h',
  OPT_QUIET = 'q',

  /* Server */
  OPT_HOST    = 'H',    /* IPv4 host     */
  OPT_PORT    = 'p',    /* Port          */
  OPT_TIMEOUT = 't',    /* Reply timeout */
  OPT_AUTH    = 'a'     /* Auth          */

} ropt_t;


/* GNU long options */
static struct option lopts [] =
{
  /* Miscellanea */
  { "help",    no_argument,       NULL, OPT_HELP    },
  { "quiet",   no_argument,       NULL, OPT_QUIET   },

  /* Server */
  { "host",    required_argument, NULL, OPT_HOST    },
  { "port",    required_argument, NULL, OPT_PORT    },
  { "timeout", required_argument, NULL, OPT_TIMEOUT },
  { "auth",    required_argument, NULL, OPT_AUTH    },

  { NULL,    0,                   NULL, 0           }
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
  printf ("\n");

  printf ("Define Redis server parameters:\n");
  usage_item (options, n, OPT_HOST,    "host");
  usage_item (options, n, OPT_PORT,    "port");
  usage_item (options, n, OPT_TIMEOUT, "timeout");
  usage_item (options, n, OPT_AUTH,    "auth");
}


/* Builtin extension */
int rdsh_connect (int argc, char * argv [])
{
  char * progname  = basename (argv [0]);
  char * sopts     = optlegitimate (lopts);
  char * host      = DEFAULT_HOST;
  unsigned port    = DEFAULT_PORT;
  char * auth      = DEFAULT_AUTH;
  unsigned timeout = DEFAULT_TIMEOUT;
  bool quiet       = false;
  rdsh_t * redis;
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
	case OPT_HELP:  usage (progname, lopts);   return 0;
	case OPT_QUIET: quiet = true;              break;

	  /* Server */
	case OPT_HOST:    host    = optarg;        break;
	case OPT_PORT:    port    = atoi (optarg); break;
	case OPT_TIMEOUT: timeout = atoi (optarg); break;
	case OPT_AUTH:    auth    = optarg;        break;
	}
    }

  /* Create a physical connection to the Redis server */
  if (! quiet)
    printf ("%s: Connecting to Redis server %s@%u ... ", progname, host, port);
  if (! (redis = redis_connect (host, port, timeout)))
    {
      if (! quiet)
	printf ("Failed!\n");

      return 1;
    }
  if (! quiet)
    printf ("Ok!\n");

  if (auth)
    ;

  /* Keep track of the last active session */
  setactive_session (redis);

  /* Add to the table of sessions */
  moresession (redis);

  /* Update user prompt to include the active session */
  rdsh_prompt (redis_name (redis));

  return 0;
}
