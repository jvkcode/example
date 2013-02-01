## Prelude, basic settings Autoconf
# PACKAGE: my_dlopen_so
# VERSION: 1.0.0
AC_PREREQ([2.69])
AC_INIT(my_dlopen_so, 1.0.0,[-bug],[my_dlopen_so-1.0.0])

VERSION=AC_PACKAGE_VERSION
AC_SUBST(VERSION)

AC_MSG_NOTICE([configuring $PACKAGE_NAME-$PACKAGE_VERSION])
AC_CONFIG_SRCDIR([src/my_dlopen_so_main.c])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([build-aux])

AC_CANONICAL_HOST
AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE([-Wall])


echo "                             Testing for a C compiler"
AC_PROG_CC
if test x"$GCC" = xyes 
then
    CFLAGS="$CFLAGS -W -Wall"
    AC_MSG_NOTICE([Adding gcc options: $CFLAGS])
fi
CFLAGS="$CFLAGS"

if test x"${target_cpu}" = x"i386" && test x"${host_cpu}" = x"x86_64" ; then
CFLAGS="$CFLAGS -m32"
fi

echo "                             Testing for libtool"
AC_CHECK_TOOL(AR, ar)
dnl AC_CHECK_TOOL(RANLIB, ranlib, :)
dnl AM_PROG_LIBTOOL
LT_INIT([dlopen])

AM_PROG_CC_C_O
AC_PROG_MAKE_SET
AC_PROG_LN_S

echo "                             Setting JULIA tag"
JULIA_SET_IT
dnl TARGET_BITS=unknown
dnl case x"${target_cpu}" in #(
dnl   *i386*) :
dnl     TARGET_BITS=32
dnl     ;;
dnl   *x86_64*)
dnl     TARGET_BITS=64
dnl     ;;
dnl esac

AC_SUBST(TARGET_BITS)

echo "                             Testing for headerfiles"
AC_CHECK_HEADERS([sys/errno.h dlfcn.h],[],
[AC_MSG_FAILURE("Cannot find header file")])
AC_CHECK_HEADERS([stdio.h stdint.h limits.h],[],
[AC_MSG_FAILURE("Cannot find header file")])
AC_CHECK_HEADERS([time.h dirent.h],[],
[AC_MSG_FAILURE("Cannot find header file")])

echo "                             Testing for functions"
AC_CHECK_DECLS(
[printf],
[],
[AC_MSG_FAILURE("Function is not found")])
dnl AC_CHECK_DECLS(
dnl [dlopen, dlcose],
dnl [],
dnl [AC_MSG_FAILURE("Function is not found")])

dnl AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([Makefile])

echo "                             Creating config.status"
AC_OUTPUT

