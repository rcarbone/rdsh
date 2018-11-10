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


#pragma once

/* System headers */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


/* Array length (not NULL terminated) */
#define alen(x)    (sizeof x / sizeof x [0])

/* GNU Option Array length */
#define optlen(x)  ((sizeof x / sizeof x [0]) - 1)

/* Return # of items in a NULL terminated array of any type */
#define arrlen(x)                      valen ((void **) x)
#define arrmore(argv, item, type)      (type **) vamore ((void **) argv, (void *) item)
#define arrless(argv, item, type, rm)  (type **) valess ((void **) argv, (void *) item, rm)
#define arrclear(argv, func)           vaclear ((void **) argv, func)
#define arrsort(argv, func, type)      (type **) vasort ((void **) argv, func)
#define arrrev(argv, type)             (type **) varev ((void **) argv)


/* Helper functions */
typedef void varm_f (void *);
typedef int vasort_f (const void * a, const void * b);


#ifdef __cplusplus
extern "C" {
#endif


/* Public functions */
unsigned valen (void * argv []);
void ** vamore (void * argv [], void * item);
void ** valess (void * argv [], void * item, varm_f rm);
int vahas (void * argv [], void * item);
void ** vaclear (void * argv [], varm_f rm);
void ** vadup (void * argv []);
void ** varev (void * argv []);
void ** vacat (void * a [], void * b []);
void ** vasort (void * argv [], vasort_f cmp);
void ** vashuffle (void * argv []);
unsigned vadigits (unsigned n);
char * vafmt (void * argv []);
char * valeft (unsigned n);
void ** varnd (unsigned argc, void * argv []);


#ifdef __cplusplus
}
#endif
