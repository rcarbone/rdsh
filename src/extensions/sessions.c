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
#include "hiredis-wrap.h"


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



/* The table of sessions */
static rdsh_t ** sessions = NULL;

/* The stack of most referenced sessions (current and previous) */
static rdsh_t * active   = NULL;
static rdsh_t * previous = NULL;


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


/* Evaluate maximum length of Redis server names */
static unsigned maxname (rdsh_t * argv [])
{
  unsigned m = 0;
  while (argv && * argv)
    {
      if (redis_name ((* argv)))
	m = RMAX (m, strlen (redis_name (* argv)));
      argv ++;
    }
  return m;
}


/* Evaluate maximum length of Redis server hosts */
static unsigned maxhost (rdsh_t * argv [])
{
  unsigned m = 0;
  while (argv && * argv)
    {
      if (redis_host ((* argv)))
	m = RMAX (m, strlen (redis_host (* argv)));
      argv ++;
    }
  return m;
}


/* Evaluate maximum length of Redis server ports */
static unsigned maxport (rdsh_t * argv [])
{
  unsigned m = 0;
  while (argv && * argv)
    {
      m = RMAX (m, redis_port (* argv));
      argv ++;
    }
  return m;
}


/* Evaluate maximum length of uptimes */
static unsigned maxuptime (rdsh_t * argv [])
{
  unsigned m = 0;
  while (argv && * argv)
    {
      m = RMAX (m, strlen (relapsed (redis_connected (* argv))));
      argv ++;
    }
  return m;
}


/* Evaluate maximum # of versions */
static unsigned maxversion (rdsh_t * argv [])
{
  unsigned m = 0;
  while (argv && * argv)
    {
      if (redis_version ((* argv)))
	m = RMAX (m, strlen (redis_version (* argv)));
      argv ++;
    }
  return m;
}


/* Format table header */
static char * sprint_header (unsigned n1, char * t1,
			     unsigned n2, char * t2,
			     unsigned n3, char * t3,
			     unsigned n4, char * t4,
			     unsigned n5, char * t5,
			     unsigned n6, char * t6,
			     unsigned n7, char * t7)
{
  static char line [1024];
  sprintf (line, " %*.*s | %*.*s | %*.*s | %*.*s | %*.*s | %*.*s | %*.*s |\n",
	   n1, n1, t1,
	   n2, n2, scenter (t2, n2),
	   n3, n3, scenter (t3, n3),
	   n4, n4, scenter (t4, n4),
	   n5, n5, scenter (t5, n5),
	   n6, n6, scenter (t6, n6),
	   n7, n7, scenter (t7, n7));
  return line;
}


/* Format table row content */
static char * sprint_item (rdsh_t * r, unsigned seq, char * fmt)
{
  static char line [1024];
  sprintf (line, fmt, seq,
	   redis_name (r),
	   redis_host (r),
	   redis_port (r),
	   relapsed (redis_connected (r)),
	   redis_name (r),
	   scenter (redis_status (r) ? "Y" : "N", strlen ("Connected")));
  return line;
}


/* Format rows with all user tables */
static char ** sprint_rows (unsigned rows, rdsh_t * sessions [])
{
  if (! sessions)
    return NULL;

  char ** lines = NULL;

  char * label_1 = "#";
  char * label_2 = "Id";
  char * label_3 = "Host";
  char * label_4 = "Port";
  char * label_5 = "Uptime";
  char * label_6 = "Version";
  char * label_7 = "Connected";

  unsigned d = digits (RMAX (strlen (label_1), rows));                    /* numeric */
  unsigned i = RMAX (strlen (label_2), maxname (sessions));               /* string  */
  unsigned h = RMAX (strlen (label_3), maxhost (sessions));               /* string  */
  unsigned p = RMAX (strlen (label_4), digits (maxport (sessions)));      /* numeric */
  unsigned u = RMAX (strlen (label_5), maxuptime (sessions));             /* string  */
  unsigned v = RMAX (strlen (label_6), maxversion (sessions));            /* string  */
  unsigned c = RMAX (strlen (label_7), 1);                                /* string  */

  unsigned seq = 0;
  char fmt [1024];
  char line [1024];

  /* Evaluate the format of each row */
  sprintf (fmt,
	   " %%%du | %%-%d.%ds | %%-%d.%ds | %%%du | %%-%d.%ds | %%-%d.%ds | %%-%d.%ds |\n",
	   d,
	   i, i,
	   h, h,
	   p,
	   u, u,
	   v, v,
	   c, c);

  /* Table header */
  sprintf (line, "%s", sprint_header (d, label_1,
				      i, label_2,
				      h, label_3,
				      p, label_4,
				      u, label_5,
				      v, label_6,
				      c, label_7));
  lines = argsmore (lines, line);

  /* Table content */
  while (rows -- && sessions && * sessions)
    lines = argsmore (lines, sprint_item (* sessions ++, ++ seq, fmt));

  return lines;
}


/* Print rows with all user sessions */
static void print_rows (rdsh_t * argv [])
{
  char ** lines = sprint_rows (arrlen (argv), argv);
  char ** l = lines;
  while (l && * l)
    printf ("%s", * l ++);
  argsclear (lines);
}


/* Return the current referenced session */
rdsh_t * active_session (void)
{
  return active;
}


/* Return the previous referenced session */
rdsh_t * previous_session (void)
{
  return previous;
}


/* Return the latest referenced session */
rdsh_t * lastest_session (void)
{
  return previous;
}


/* Set [redis] as the active session */
void setactive_session (rdsh_t * redis)
{
  previous = active;
  active = redis;
}


/* Reset the active session after [redis] has been closed */
void resetactive_session (rdsh_t * redis)
{
  if (redis == active)
    {
      active = previous;
      previous = NULL;
    }
  else if (redis == previous)
    previous = NULL;
}


/* Retrieve the name of the active session (if any) */
char * getredisname (void)
{
  return redis_name (active);
}


unsigned sessionslen (void)
{
  return arrlen (sessions);
}


rdsh_t ** getsessions (void)
{
  return sessions;
}


void moresession (rdsh_t * redis)
{
  sessions = arrmore (sessions, redis, rdsh_t);
}


void lesssession (rdsh_t * redis)
{
  sessions = arrless (sessions, redis, rdsh_t, NULL);
}


/* Builtin extension */
int rdsh_sessions (int argc, char * argv [])
{
  char * progname = basename (argv [0]);
  char * sopts    = optlegitimate (lopts);
  time_t now      = time (NULL);
  struct tm * tm  = localtime (& now);
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

  /*
   * a simple banner, something like:
   * 7:07:18pm   up   0 days,  0:00:14,    1 session
   */
  printf ("%2d:%02d:%02d%s   up %3d days, %2d:%02d:%02d,    %d session%s\n",
	  tm -> tm_hour == 12 ? 12 : tm -> tm_hour % 12,
	  tm -> tm_min,
	  tm -> tm_sec,
	  tm -> tm_hour >= 13 ? "pm" : "am",
	  _days_ (now, boottime . tv_sec),
	  _hours_ (now, boottime . tv_sec),
	  _mins_ (now, boottime . tv_sec),
	  (int) (now - boottime . tv_sec) % 60,
	  sessionslen (), sessionslen () > 1 ? "s" : "");
  printf ("\n");

  /*
   * information for each session, something like
   * (REDIS) --  user,   28 tables,   1757 records - up
   */
  if (sessions)
    print_rows (sessions);
  else
    printf ("No session.\n");

  return 0;
}
