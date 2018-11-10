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


/* Encapsulation functions for hiredis */

/* System headers */
#include <stdbool.h>

/* Project headers */
#include "rdsh.h"
#include "hiredis-wrap.h"


rdsh_t * mksession (char * host, unsigned port, unsigned timeout, redisContext * rds)
{
  rdsh_t * redis    = calloc (1, sizeof (* redis));

  char * name       = calloc (1, strlen (host) + 1 + 5);

  sprintf (name, "%s:%u", host, port);

  redis -> updated  = nswall ();
  redis -> host     = safedup (host);
  redis -> port     = port;
  redis -> timeout  = timeout;

  redis -> name      = name;
  redis -> connected = nswall ();

  redis -> rds       = rds;

  return redis;
}


rdsh_t * rmsession (rdsh_t * redis)
{
  if (! redis)
    return NULL;

  safefree (redis -> host);
  safefree (redis -> name);

  if (redis -> rds)
    redisFree (redis -> rds);
  free (redis);

  return NULL;
}


char * redis_host (rdsh_t * redis)
{
  return redis ? redis -> host : NULL;
}


unsigned redis_port (rdsh_t * redis)
{
  return redis ? redis -> port : 0;
}


char * redis_name (rdsh_t * redis)
{
  return redis ? redis -> name : NULL;
}


rtime_t redis_connected (rdsh_t * redis)
{
  return redis ? redis -> connected : 0;
}


char * redis_version (rdsh_t * redis)
{
  return redis ? redis -> version : NULL;
}


bool redis_status (rdsh_t * redis)
{
  return redis ? true : false;
}


rdsh_t * redis_connect (char * host, unsigned port, unsigned timeout)
{
  redisContext * rds = redisConnect (host, port);
  if (rds)
    {
      if (! rds -> err)
	return mksession (host, port, timeout, rds);
      redisFree (rds);
    }
  return NULL;
}


void redis_disconnect (rdsh_t * redis)
{
  rmsession (redis);
}


/* Run a command on a Redis Server */
void redis_run (redisContext * rds, char * prog, unsigned argc, char * argv [])
{
  if (rds && argc && argv)
    {
      redisReply * reply = redisCommandArgv (rds, argc, (const char **) argv, NULL);
      if (reply)
	{
	  unsigned i;
	  switch (reply -> type)
	    {
	    case REDIS_REPLY_STRING:
	    case REDIS_REPLY_STATUS:
	      printf ("%s: %s\n", prog, reply -> str);
	      break;

	    case REDIS_REPLY_ARRAY:
	      for (i = 0; i < reply -> elements; i ++)
		printf ("%s: %s\n", prog, reply -> element [i] -> str);
	      break;

	    case REDIS_REPLY_INTEGER:
	      printf ("%s: %lld\n", prog, reply -> integer);
	      break;

	    case REDIS_REPLY_NIL:
	      printf ("%s: %s\n", prog, "NIL");
	      break;

	    case REDIS_REPLY_ERROR:
	      printf ("%s: %s\n", prog, "ERROR");
	      break;
	    }
	  freeReplyObject (reply);
	}
      else
	printf ("%s: Failed!\n", prog);
    }
}
