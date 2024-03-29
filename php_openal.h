/*
   +----------------------------------------------------------------------+
   | Copyright (c) The PHP Group                                |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available at through the world-wide-web at                           |
   | http://www.php.net/license/3_01.txt.                                 |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
   | Authors: Sara Golemon <pollita@php.net>                              |
   |          Santiago Lizardo <santiagolizardo@php.net>                  |
   +----------------------------------------------------------------------+
*/

#ifndef PHP_OPENAL_H
#define PHP_OPENAL_H

/* We never have one without the other, but it's a simple sanity check */
#if !defined(HAVE_AL_AL_H) || !defined(HAVE_AL_ALC_H)
#undef WITH_OPENAL
#endif

#if WITH_OPENAL

#include <AL/al.h>
#include <AL/alc.h>

#ifdef HAVE_AL_ALEXT_H
#include <AL/alext.h>
#endif

#define PHP_OPENAL_VERSION "0.10.0"

#define PHP_OPENAL_RES_DEVICE "Open AL(Device)"
#define PHP_OPENAL_RES_CONTEXT "Open AL(Context)"
#define PHP_OPENAL_RES_BUFFER "Open AL(Buffer)"
#define PHP_OPENAL_RES_SOURCE "Open AL(Source)"

#define PHP_OPENAL_DEF_FREQ 44100
#define PHP_OPENAL_DEF_SYNC AL_TRUE

extern zend_module_entry openal_module_entry;
#define phpext_openal_ptr &openal_module_entry

PHP_MINIT_FUNCTION(openal);
PHP_MINFO_FUNCTION(openal);

PHP_FUNCTION(openal_device_open);
PHP_FUNCTION(openal_device_close);
PHP_FUNCTION(openal_context_create);
PHP_FUNCTION(openal_context_current);
PHP_FUNCTION(openal_context_process);
PHP_FUNCTION(openal_context_suspend);
PHP_FUNCTION(openal_context_destroy);
PHP_FUNCTION(openal_buffer_create);
PHP_FUNCTION(openal_buffer_get);
PHP_FUNCTION(openal_buffer_data);
PHP_FUNCTION(openal_buffer_destroy);
PHP_FUNCTION(openal_source_create);
PHP_FUNCTION(openal_source_get);
PHP_FUNCTION(openal_source_set);
PHP_FUNCTION(openal_source_play);
PHP_FUNCTION(openal_source_pause);
PHP_FUNCTION(openal_source_stop);
PHP_FUNCTION(openal_source_rewind);
PHP_FUNCTION(openal_source_destroy);
PHP_FUNCTION(openal_listener_get);
PHP_FUNCTION(openal_listener_set);

#else

#define phpext_openal_ptr NULL

#endif /* WITH_OPENAL */

#endif /* PHP_OPENAL_H */
