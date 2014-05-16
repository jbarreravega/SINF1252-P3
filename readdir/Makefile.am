AM_CFLAGS = -I$(top_srcdir)/lib @AM_CFLAGS@
bin_PROGRAMS = readdir
readdir_SOURCES = readdir.c
readdir_LDADD = $(top_builddir)/lib/libbenchmark.a $(AM_LDFLAGS) 	

GRAPHS = readdir1.csv readdir2.csv readdir3.csv
PROG   = readdir
TMP = tmp1 tmp2 tmp3

include ../lib/lib.mk
