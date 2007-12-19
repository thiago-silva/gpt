AC_PREREQ(2.60)

AC_INIT(gptcommon, 1.0)
AC_CONFIG_HEADER([config.h])
AM_INIT_AUTOMAKE(gptcommon, 1.0)

AC_LANG(C++)
AC_PROG_CXX
AM_PROG_LIBTOOL

#------------------------------
# debug options
#------------------------------

AC_ARG_ENABLE(debug,
        AC_HELP_STRING([--enable-debug=ARG],[enables debug symbols (yes|no|full) [default=no]]),
[
  case $enableval in
    yes)
      use_debug_code="yes"
      use_debug_define=yes
      ;;
    full)
      use_debug_code="full"
      use_debug_define=yes
      ;;
    *)
      use_debug_code="no"
      use_debug_define=no
      ;;
  esac
],
  [use_debug_code="no"
    use_debug_define=no
])

CXXFLAGS=
if test "$use_debug_code" != "no"; then
  if test $use_debug_code = "full"; then
    CXXFLAGS="-g3 $CXXFLAGS"
  else
    CXXFLAGS="-g -O2 $CXXFLAGS"
  fi
else
  CXXFLAGS="-O2 $CXXFLAGS"
fi

if test "$use_debug_define" = "yes"; then
  CXXFLAGS="-DDEBUG $CXXFLAGS"
fi


AC_CONFIG_FILES([Makefile
                 src/Makefile])
AC_OUTPUT
