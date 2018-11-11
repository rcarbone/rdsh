#
# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#                      _     _
#              _ __ __| |___| |__
#             | '__/ _` / __| '_ \
#             | | | (_| \__ \ | | |
#             |_|  \__,_|___/_| |_|
#
#             rdsh, the Redis shell
#
# R. Carbone (rocco@tecsiel.it)
#   4Q 2018
#
# SPDX-License-Identifier: BSD-3-Clause
#
# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#


# C source files
MAINSRCS += try-link.c
MAINSRCS += try-connect.c

# tcsh-specific
LIBSRCS  += init.c
LIBSRCS  += tcsh-wrap.c
LIBSRCS  += prompt.c
LIBSRCS  += hiredis-wrap.c

LIBSRCS  += stupid.c

# Sessions
LIBSRCS  += usage.c
LIBSRCS  += help.c
LIBSRCS  += sessions.c
LIBSRCS  += connect.c
LIBSRCS  += disconnect.c
LIBSRCS  += change.c

# General
LIBSRCS  += redis.c

# Server
LIBSRCS  += server.c

# Connections
LIBSRCS  += connections.c

# Data Structures
LIBSRCS  += data-structures.c

# Strings
LIBSRCS  += strings.c

# Hashes
LIBSRCS  += hashes.c

# Lists
LIBSRCS  += lists.c

# Sets
LIBSRCS  += sets.c

# Sorted Sets
LIBSRCS  += sorted-sets.c

#
# External software
#

# rlibc - C library of useful functions
RLIBCDIR  = ../rlibc
INCLUDE  += -I${RLIBCDIR}
LIBRLIBC  = ${RLIBCDIR}/librlibc.a

# Hiredis is a minimalistic C client library for Redis 
HIREDISDIR = ../../3rdparty/hiredis
INCLUDE  += -I${HIREDISDIR}
HIREDIS    = ${HIREDISDIR}/libhiredis.a

# rdsh - tcsh hack with extensions for Redis-client functionalities
RDSHDIR    = ../rdsh
INCLUDE  += -I${RDSHDIR}

# The name of the games
LIBNAME   = rdsh
PROGRAMS += ${MAINSRCS:%.c=%}

# All C source files
SRCS      = ${LIBSRCS} ${MAINSRCS}

# All libraries
STLIB     = lib${LIBNAME}.a
# SHLIB     = ${STLIB:%.a=%.so}

# All targets
TARGETS   = ${STLIB} ${SHLIB} ${PROGRAMS}

# Object and depend files
LIBOBJS   = ${LIBSRCS:%.c=%.o}
MAINOBJS  = ${MAINSRCS:%.c=%.o}
OBJS      = ${LIBOBJS} ${MAINOBJS}
DEPS      = ${SRCS:%.c=%.M}

# C/C++ Compilers and flags
INCLUDES  = -I./
INCLUDES += ${INCLUDE}
CC        = gcc
AR        = ar
LD        = gcc
CFLAGS    = -g -Wall
CFLAGS   += -fPIC
CFLAGS   += -fno-diagnostics-color
CFLAGS   += ${INCLUDES}
ARFLAGS   = rv
LDFLAGS   =

# User and System Libraries
USRLIBS  += ${STLIB}
USRLIBS  += ${HIREDIS}
USRLIBS  += ${LIBRLIBC}
SYSLIBS  +=

# The main target is responsible to make all the targets
all: ${TARGETS}

# Static library
${STLIB}: ${LIBOBJS}
	@echo "=*= making static library $@ =*="
	@${AR} ${ARFLAGS} $@ $^ 1> /dev/null 2>& 1

# Shared library
${SHLIB}: ${LIBOBJS}
	@echo "=*= making shared library $@ =*="
	@${CC} -shared $^ ${USRLIBS} ${SYSLIBS} -o $@

# Binary programs
%: %.o ${USRLIBS}
	@echo "=*= making program $@ =*="
	@${LD} ${LDFLAGS} $^ ${SYSLIBS} -o $@

# Cleanup rules
clean:
	@rm -f ${TARGETS}
	@rm -f ${OBJS}
	@rm -f *~

distclean: clean
	@rm -f ${DEPS}

# How to make an object file from a C source
%.o: %.c
	@echo "=*= making object $@ =*="
	@${CC} -c ${CFLAGS} $<

# How to make a depend file from a C source
%.M: %.c
	@-${CC} -MM ${INCLUDES} $< -o $@

-include ${DEPS}

${LIBRLIBC}:
	@(cd ${RLIBCDIR}; make -s)

# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
# Check for errors and memory leaks
VFLAGS   += --error-exitcode=1
VFLAGS   += --leak-check=full
VFLAGS   += --show-leak-kinds=all
SUPPRESS =

# Run valgrind (using raw suppression file)
leaks-%: %
	@echo "=*= ckecking $^ (suppression file ${SUPPRESS}) =*="
	@echo
	@valgrind ${VFLAGS} --suppressions=${SUPPRESS} $^

# Run valgrind (no suppression file)
checks-%: %
	@echo "=*= ckecking $^ =*="
	@echo
	@valgrind ${VFLAGS} $^

# All errors and memory leaks
leaks: ${PROGRAMS}
	@for bin in ${PROGRAMS} ; do \
           echo "=*= checking program $$bin for errors =*=" ; \
           make leaks-$$bin ; \
         done

checks: ${PROGRAMS}
	@for bin in ${PROGRAMS} ; do \
           echo "=*= checking program $$bin for errors =*=" ; \
           make checks-$$bin ; \
         done

# The help target
help:
	@echo "Usage:"
	@echo "  make all     --> compile only (just to check for C program syntax)"
	@echo "  make X       --> make suppression files [*.X]"
	@echo "  make leaks   --> ckeck for memory leaks using suppression file ${SUPPRESS}"
	@echo "  make ckecks  --> ckeck for memory leaks (no suppression files)"
	@echo
	@echo "  make leaks-X --> ckeck for memory leaks just in module X"
	@echo
	@echo "  The following modules have been currently defined:"
	@echo -n "   "
	@for p in ${PROGRAMS} ; do \
           echo -n "$$p " ; \
         done
	@echo

.DEFAULT:
	@echo "No rule has been defined to make: $@"
