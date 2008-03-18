AC_PREREQ(2.60)

AC_INIT(gpt, 1.0)
AM_INIT_AUTOMAKE(gpt, 1.0)
AC_CONFIG_HEADER([config.h])
AC_LANG(C++)
AC_PROG_CXX
AM_PROG_LIBTOOL


dnl------------------------------
dnl debug options
dnl------------------------------

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


dnl------------------------------
dnl Check SO
dnl------------------------------

if test $version_type = "windows"; then
   SO_WINDOWS="yes"
else
   SO_WINDOWS="no"
fi

AM_CONDITIONAL(BUILD_DEBUGGER, test x$SO_WINDOWS = xno)

dnl------------------------------
dnl Checks for ANTLR
dnl------------------------------

AC_PATH_PROG(ANTLR_BIN, antlr)

if test "x${ANTLR}" = "x"; then
  AC_PATH_PROG(ANTLR_BIN, runantlr)
fi

if test "x${ANTLR_BIN}" = "x"; then
  AC_MSG_ERROR(
  [
    O programa "antlr" (ou runantlr) nao foi encontrado no seu sistema (PATH).
    GPT precisa do ANTLR (versao 2.7.x) instalado.
    Baixe em: http://www.antlr.org
  ])
fi

AC_PATH_PROG(ANTLR_CFG, antlr-config)
if test "x${ANTLR_CFG}" = "x"; then
  AC_MSG_ERROR(
  [
    O programa "antlr-config" nao foi encontrado no seu sistema (PATH).
    GPT precisa do ANTLR (versao 2.7.x) instalado.
    Baixe em: http://www.antlr.org
  ])
fi

ANTLR_LIB=`${ANTLR_CFG} --libs`
ANTLR_INC=`${ANTLR_CFG} --cflags`

AC_SUBST(ANTLR_BIN)
AC_SUBST(ANTLR_INC)
AC_SUBST(ANTLR_LIB)


dnl------------------------------
dnl checks for PCRE
dnl------------------------------

# AC_MSG_CHECKING(for pcre)

AC_CHECK_PROG(has_pcre, pcre-config, yes)

if test "x$has_pcre" = "xyes"; then
  PCRE_CONFIG="pcre-config"
else
  AC_MSG_ERROR(
  [
    GPT precisa da biblioteca PCRE instalada
    Baixe em: http://www.pcre.org/
  ])
fi

#pcrecpp

PCRE_INC=`${PCRE_CONFIG} --cglags`
PCRE_LIB="-L`${PCRE_CONFIG} --prefix`/lib -lpcrecpp"
AC_SUBST(PCRE_INC)
AC_SUBST(PCRE_LIB)


dnl------------------------------
dnl checks if we want to install 
dnl the libs and headers
dnl------------------------------

AC_ARG_ENABLE([install-devel],
    [AC_HELP_STRING([--enable-install-devel],
                    [instala bibliotecas e headers])], 
    [INSTALL_DEVEL="yes"],
    [INSTALL_DEVEL="no"]
)

AM_CONDITIONAL(INSTALL_DEVEL, test x$INSTALL_DEVEL = xyes)


dnl------------------------------
dnl the end
dnl------------------------------

AC_OUTPUT(Makefile        
        src/Makefile 
        doc/Makefile 
        doc/man/Makefile
        doc/man/pt_BR/Makefile  
        exemplos/Makefile
        lib/Makefile
        src/modules/Makefile 
        src/modules/parser/Makefile 
        src/modules/interpreter/Makefile 
        src/modules/c_translator/Makefile
        src/modules/x86/Makefile
      )

AC_MSG_NOTICE([
Sumario:
	antlr           : $ANTLR_BIN
	antlr includes  : $ANTLR_INC
	antr lib        : $ANTLR_LIB
	
	pcre includes   : $PCRE_INC
	pcre lib        : $PCRE_LIB

	CXXFLAGS        : $CXXFLAGS
	MS Windows      : $SO_WINDOWS
])
