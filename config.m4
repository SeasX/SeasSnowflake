dnl $Id$
dnl config.m4 for extension SeasSnowflake

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(SeasSnowflake, for SeasSnowflake support,
dnl Make sure that the comment is aligned:
dnl [  --with-SeasSnowflake             Include SeasSnowflake support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(SeasSnowflake, whether to enable SeasSnowflake support,
Make sure that the comment is aligned:
[  --enable-SeasSnowflake           Enable SeasSnowflake support])

if test "$PHP_SEASSNOWFLAKE" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-SeasSnowflake -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/SeasSnowflake.h"  # you most likely want to change this
  dnl if test -r $PHP_SEASSNOWFLAKE/$SEARCH_FOR; then # path given as parameter
  dnl   SEASSNOWFLAKE_DIR=$PHP_SEASSNOWFLAKE
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for SeasSnowflake files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       SEASSNOWFLAKE_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$SEASSNOWFLAKE_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the SeasSnowflake distribution])
  dnl fi

  dnl # --with-SeasSnowflake -> add include path
  dnl PHP_ADD_INCLUDE($SEASSNOWFLAKE_DIR/include)

  dnl # --with-SeasSnowflake -> check for lib and symbol presence
  dnl LIBNAME=SeasSnowflake # you may want to change this
  dnl LIBSYMBOL=SeasSnowflake # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $SEASSNOWFLAKE_DIR/$PHP_LIBDIR, SEASSNOWFLAKE_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_SEASSNOWFLAKELIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong SeasSnowflake lib version or lib not found])
  dnl ],[
  dnl   -L$SEASSNOWFLAKE_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(SEASSNOWFLAKE_SHARED_LIBADD)
  PHP_REQUIRE_CXX()
  PHP_SUBST(SEASSNOWFLAKE_SHARED_LIBADD)
  PHP_ADD_LIBRARY(stdc++, 1, SEASSNOWFLAKE_SHARED_LIBADD)
  CXXFLAGS="$CXXFLAGS -Wall -Wno-unused-function -Wno-deprecated -Wno-deprecated-declarations -std=c++11"
  SeasSnowflake_source_file="SeasSnowflake.cpp"

  THIS_DIR=`dirname $0`
  PHP_NEW_EXTENSION(SeasSnowflake, $SeasSnowflake_source_file, $ext_shared,,-DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
fi
