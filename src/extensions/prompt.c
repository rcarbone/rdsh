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


/* tcsh header file(s) */
#include "sh.h"

/* Project headers */
#include "rdsh.h"


/* Change the prompt */
void rdsh_prompt (char * text)
{
  Char Prompt [256];
  char * prompt = NULL;

  int i;
  int len;

  if (text)
    {
      prompt = calloc (strlen (progname) + strlen (text) + 200, 1);
      sprintf (prompt, "%%S%s@%s %%!>%%s ", progname, text);
    }
  else
    {
      prompt = calloc (strlen (progname) + 200, 1);
      sprintf (prompt, "%%S%s %%!>%%s ", progname);
    }

  len = strlen (prompt);
  for (i = 0; i <= len; i ++)
    Prompt [i] = prompt [i];

  free (prompt);

  setcopy (STRprompt, Strsave (Prompt), VAR_READWRITE);
}
