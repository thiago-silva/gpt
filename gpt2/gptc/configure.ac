AC_PREREQ(2.60)

AC_INIT(gptc, 2.0)
AC_CONFIG_HEADER([config.h])
AM_INIT_AUTOMAKE(gptc, 2.0)

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


#------------------------------
# Checks for ANTLR
#------------------------------

AC_PATH_PROG(ANTLR_BIN, antlr)

if test "x${ANTLR}" = "x"; then
  AC_PATH_PROG(ANTLR_BIN, runantlr)
fi

if test "x${ANTLR_BIN}" = "x"; then
  AC_MSG_ERROR(
  [
    O programa "antlr" (ou runantlr) nÃÂ£o foi encontrado no seu sistema (PATH).
    GPT precisa do ANTLR (versÃÂ£o >= 2.6) instalado.
    Baixe em: http://www.antlr.org
  ])
fi

AC_PATH_PROG(ANTLR_CFG, antlr-config)
if test "x${ANTLR_CFG}" = "x"; then
  AC_MSG_ERROR(
  [
    O programa "antlr-config" nÃÂ£o foi encontrado no seu sistema (PATH).
    GPT precisa do ANTLR (versÃÂ£o >= 2.6) instalado.
    Baixe em: http://www.antlr.org
  ])
fi

ANTLR_LIB=`${ANTLR_CFG} --libs`
ANTLR_INC=`${ANTLR_CFG} --cflags`

AC_SUBST(ANTLR_BIN)
AC_SUBST(ANTLR_INC)
AC_SUBST(ANTLR_LIB)


AC_CONFIG_FILES([Makefile
                 src/Makefile
                 src/common/Makefile
                 src/parser/Makefile
                 src/semantic/Makefile
                 src/symboltable/Makefile
                 src/gptasm_target/Makefile
                 test/Makefile])
AC_OUTPUT
