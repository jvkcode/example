## Demo how to set own m4
AC_DEFUN([JULIA_SET_IT],
[
dnl Set macro for gcc to use in .c file
TARGET_BITS=unknown
case x"${target_cpu}" in #(
  *i386*) :
    TARGET_BITS=32
    ;;
  *x86_64*)
    TARGET_BITS=64
    ;;
esac
AC_SUBST(TARGET_BITS)dnl 
])
