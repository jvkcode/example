
AUTOMAKE_OPTIONS = gnu
ACLOCAL_AMFLAGS = -I build-aux
AUTOCOMF_AMFLAGS = -o configure -I build-aux
AUTOMAKE_AMFLAGS = --add-missing --libdir=build-aux

# libtool -version-info current[:revision[:age]]
MY_DLOPEN_SO_CURRENT = 5
MY_DLOPEN_SO_REVISION = 15
MY_DLOPEN_SO_AGE = 1

AM_CFLAGS = -I$(srcdir)/src -DJULIA=@TARGET_BITS@

EXTRA_DIST = \
src/libs/my_dlopen_so_libs.c 

include_HEADERS =

bin_PROGRAMS = my_dlopen_so
my_dlopen_so_SOURCES = src/my_dlopen_so_main.c
my_dlopen_so_LDFLAGS = @LDFLAGS@ -L$(top_builddir)/.libs -ldl 
#-Wl,-soname -Wl,libmy_dlopen_so.so 

lib_LTLIBRARIES = libmy_dlopen_so.la
# noinst_LTLIBRARIES

libmy_dlopen_so_la_SOURCES = $(include_HEADERS) $(EXTRA_DIST)

libmy_dlopen_so_la_LIBADD = @LIBOBJS@

libmy_dlopen_so_la_LDFLAGS = @LDFLAGS@ -version-info $(MY_DLOPEN_SO_CURRENT):$(MY_DLOPEN_SO_REVISION):$(MY_DLOPEN_SO_AGE) -module

