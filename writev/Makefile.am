AM_CFLAGS = -I$(top_srcdir)/lib @AM_CFLAGS@
bin_PROGRAMS = writev
writev_SOURCES = writev.c
writev_LDADD = $(top_builddir)/lib/libbenchmark.a $(AM_LDFLAGS)

PROG   = writev
GRAPHS = writev.csv

include ../lib/lib.mk
