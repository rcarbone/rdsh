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
  OPT_VERBOSE = 'v',

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
  { "verbose", no_argument, NULL, OPT_VERBOSE       },

  /* Server */
  { "host",    required_argument, NULL, OPT_HOST    },
  { "port",    required_argument, NULL, OPT_PORT    },
  { "timeout", required_argument, NULL, OPT_TIMEOUT },
  { "auth",    required_argument, NULL, OPT_AUTH    },

  { NULL,    0,                   NULL, 0           }
};


/* Display the syntax for using the builtin */
static void usage (char * name, char * synopsis, char * help, struct option * options)
{
  /* longest option name */
  unsigned n = optmax (options);

  common_usage (n, name, synopsis, help, options);

  printf ("Connection:\n");
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

	  /* Redis server address and options */
	case OPT_HOST:    host    = optarg;        break;
	case OPT_PORT:    port    = atoi (optarg); break;
	case OPT_TIMEOUT: timeout = atoi (optarg); break;
	case OPT_AUTH:    auth    = optarg;        break;
	}
    }

  /* Check for optional arguments */
  if (argc < optind + b -> args)
    ;

  /* Create a physical connection to the Redis server */
  if (verbose)
    printf ("%s: connecting to Redis server at %s@%u ... ", progname, host, port);
  if (! (sess = redis_connect (host, port, timeout)))
    {
      if (! verbose)
	printf ("Failed!\n");

      return 1;
    }
  if (verbose)
    printf ("Ok!\n");

  if (auth)
    ;

  /* Keep track of the last active session */
  setactive_session (sess);

  /* Add to the table of sessions */
  moresession (sess);

  /* Update user prompt to include the active session */
  rdsh_prompt (redis_name (sess));

  return 0;
}
