AM_CFLAGS = -I$(top_srcdir)/lib @AM_CFLAGS@
bin_PROGRAMS = writev
writev_SOURCES = writev.c -g
writev_LDADD = $(top_builddir)/lib/libbenchmark.a $(AM_LDFLAGS)

GRAPHS = writev.csv write.csv
PROG   = writev
TMP    = tmp.dat

include ../lib/lib.mk
