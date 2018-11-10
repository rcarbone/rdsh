#
# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#                   _ _ _          
#              _ __| (_) |__   ___ 
#             | '__| | | '_ \ / __|
#             | |  | | | |_) | (__ 
#             |_|  |_|_|_.__/ \___|
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
MAINSRCS +=
LIBSRCS  += safe.c
LIBSRCS  += replace.c
LIBSRCS  += vargv.c
LIBSRCS  += sargv.c
LIBSRCS  += rctype.c
LIBSRCS  += rstring.c
LIBSRCS  += roptions.c
LIBSRCS  += support.c
LIBSRCS  += rwall.c
LIBSRCS  += interval.c
LIBSRCS  += elapsed.c

# The name of the games
LIBNAME   = rlibc
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
