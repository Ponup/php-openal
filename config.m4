PHP_ARG_WITH(openal, whether to enable PHP-OpenAL,
[  --with-openal[=DIR]       Enable OpenAL Bindings.])

PHP_ARG_ENABLE(openal-debug, whether to enable PHP-OpenAL debug support,
[  --enable-openal-debug     Enable OpenAL debug support], no, no)

if test "$PHP_OPENAL" != "no"; then

  AC_DEFINE(WITH_OPENAL, 1, [Whether you want PHP-OpenAL])
  AC_MSG_CHECKING([for AL/al.h and AL/alc.h])
  for i in $PHP_OPENAL /usr/local /usr; do
    if test -r $i/include/AL/al.h; then
      OPENAL_DIR=$i
      AC_DEFINE(HAVE_AL_AL_H, 1, [Whether you have AL/al.h])
      PHP_ADD_INCLUDE($i/include)
    fi
    if test -r $i/include/AL/alc.h; then
      AC_DEFINE(HAVE_AL_ALC_H, 1, [Whether you have AL/alc.h])
      PHP_ADD_INCLUDE($i/include)
    fi
    if test -r $i/include/AL/alext.h; then
      AC_DEFINE(HAVE_AL_ALEXT_H, 1, [Whether you have AL/alext.h])
      PHP_ADD_INCLUDE($i/include)
    fi
  done

  dnl {{{ --enable-openal-debug
  if test "$PHP_OPENAL_DEBUG" != "no"; then
    CFLAGS="$CFLAGS -Wall -Wpedantic -g -ggdb -O0"
  fi
  dnl }}}

  if test -z "$OPENAL_DIR"; then
    AC_MSG_RESULT([not found])
    AC_MSG_ERROR([Please reinstall the OpenAL distribution including development files])
  else
    AC_MSG_RESULT([found])
  fi

  AC_CHECK_LIB(openal, alcOpenDevice, [
    PHP_ADD_LIBRARY(openal,, OPENAL_SHARED_LIBADD)
  ], [
    AC_MSG_ERROR([libopenal.so not found!])
  ])

  PHP_SUBST(OPENAL_SHARED_LIBADD)
  PHP_NEW_EXTENSION(openal, php_openal.c, $ext_shared, , )
fi
