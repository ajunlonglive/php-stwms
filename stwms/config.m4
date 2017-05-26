dnl $Id$
dnl config.m4 for extension stwms

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(stwms, for stwms support,
dnl Make sure that the comment is aligned:
dnl [  --with-stwms             Include stwms support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(stwms, whether to enable stwms support,
[  --enable-stwms           Enable stwms support])

if test "$PHP_STWMS" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-stwms -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/stwms.h"  # you most likely want to change this
  dnl if test -r $PHP_STWMS/$SEARCH_FOR; then # path given as parameter
  dnl   STWMS_DIR=$PHP_STWMS
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for stwms files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       STWMS_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$STWMS_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the stwms distribution])
  dnl fi

  dnl # --with-stwms -> add include path
  dnl PHP_ADD_INCLUDE($STWMS_DIR/include)

  dnl # --with-stwms -> check for lib and symbol presence
  dnl LIBNAME=stwms # you may want to change this
  dnl LIBSYMBOL=stwms # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $STWMS_DIR/$PHP_LIBDIR, STWMS_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_STWMSLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong stwms lib version or lib not found])
  dnl ],[
  dnl   -L$STWMS_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(STWMS_SHARED_LIBADD)

  PHP_NEW_EXTENSION(stwms, stwms.c stwms_funcs.c, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
fi


if test -z "$PHP_DEBUG" ; then 
    AC_ARG_ENABLE(debug, [--enable-debug compile with debugging system], [PHP_DEBUG=$enableval],[PHP_DEBUG=no] ) 
fi 
