#############################################################################
#                         Standard build tool stuff                         #
#############################################################################
# all this file is created manually
#change the version here
m4_define([solitario_VERSION], [2.0])
#this settings will ba also available in config.h
AC_INIT([solitario], 
        m4_defn([solitario_VERSION]), 
        [6colpiunbucosolo@gmx.net],
        [solitario-]m4_defn([solitario_VERSION]),
        [https://invido.it])
#some settings for make and autoconf
AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects])
# The value of the Variable DATA_PREFIX used in compile is define here below
AC_SUBST(DATA_PREFIX,["data/"]) 
AC_PROG_CXX
# static library stuff
AC_PROG_RANLIB
AM_PROG_AR
# generate Config.h
AC_CONFIG_HEADERS([src/Config.h])
#include make file list
AC_CONFIG_FILES([
  Makefile
  libini/Makefile
])
#generate files
AC_OUTPUT