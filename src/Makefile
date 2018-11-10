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


# The macro SUBDIRS defines the ordered list of all the sub-directories that are recursively tried
SUBDIRS += rlibc
SUBDIRS += extensions
SUBDIRS += rdsh

# The main target is responsible to recursively scan subdirectories and make all the defined targets
all: Makefile
	@for dir in ${SUBDIRS} ; do \
           if [ -d $$dir ] ; then \
             echo "Making $$dir ..." ; \
             make -C $$dir -s --no-print-directory ; \
           else \
             echo "Warning: missing sub-directory $$dir" ; \
           fi \
         done

# Cleanup rules
clean distclean:
	@rm -f *~
	@for dir in ${SUBDIRS} ; do \
           if [ -d $$dir ] ; then \
             make -C $$dir -s --no-print-directory $@ ; \
           fi \
         done

# Fake targets
.PHONY: all clean distclean ${SUBDIRS}
