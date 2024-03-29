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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"

#if WITH_OPENAL

#include "php_openal.h"
#include "php_globals.h"
#include "ext/standard/info.h"

/* Resource globals */
static int le_openal_device;
static int le_openal_context;
static int le_openal_buffer;
static int le_openal_source;

/* Userspace Exposure */

/* Devices */

ZEND_BEGIN_ARG_INFO_EX(arginfo_openal_device_open, 0, 0, 0)
ZEND_ARG_INFO(0, device_desc)
ZEND_END_ARG_INFO()

/* {{{ proto resource openal_device_open([string device_desc])
   Initialize the OpenAL audio layer */
PHP_FUNCTION(openal_device_open)
{
	ALCdevice *dev;
	char *device = NULL;
	size_t device_len = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "|s", &device, &device_len) == FAILURE)
	{
		RETURN_FALSE;
	}

	dev = alcOpenDevice((const ALCchar *)device);
	if (!dev)
	{
		php_error_docref(NULL, E_WARNING, "Unable to open audio device.");
		RETURN_FALSE;
	}

	RETURN_RES(zend_register_resource(dev, le_openal_device));
}
/* }}} */

ZEND_BEGIN_ARG_INFO_EX(arginfo_openal_device_close, 0, 0, 1)
ZEND_ARG_INFO(0, device)
ZEND_END_ARG_INFO()

/* {{{ proto bool openal_device_close(resource device)
   Close OpenAL device */
PHP_FUNCTION(openal_device_close)
{
	zval *zdevice;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "r", &zdevice) == FAILURE)
	{
		RETURN_FALSE;
	}

	/* Verify the resource is actually an le_openal_device */
	zend_fetch_resource(Z_RES_P(zdevice), PHP_OPENAL_RES_DEVICE, le_openal_device);
	zend_list_close(Z_RES_P(zdevice));

	RETURN_BOOL(SUCCESS);
}
/* }}} */

/* Contexts */

ZEND_BEGIN_ARG_INFO_EX(arginfo_openal_context_create, 0, 0, 1)
ZEND_ARG_INFO(0, device)
ZEND_END_ARG_INFO()

/* {{{ proto resource openal_context_create(resource device)
   Create a context using default attributes
   TODO: Allow attribute override */
PHP_FUNCTION(openal_context_create)
{
	zval *zdevice;
	ALCdevice *dev;
	ALCcontext *context;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "r", &zdevice) == FAILURE)
	{
		RETURN_FALSE;
	}

	dev = (ALCdevice *)zend_fetch_resource(Z_RES_P(zdevice), PHP_OPENAL_RES_DEVICE, le_openal_device);

	context = alcCreateContext(dev, NULL);
	if (!context)
	{
		php_error_docref(NULL, E_WARNING, "Unable to create context.");
		RETURN_FALSE;
	}

	RETURN_RES(zend_register_resource(context, le_openal_context));
}
/* }}} */

ZEND_BEGIN_ARG_INFO_EX(arginfo_openal_context_current, 0, 0, 1)
ZEND_ARG_INFO(0, context)
ZEND_END_ARG_INFO()

/* {{{ proto bool openal_context_current(resource context)
   Make the specified context current */
PHP_FUNCTION(openal_context_current)
{
	zval *zcontext;
	ALCcontext *context;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "r", &zcontext) == FAILURE)
	{
		RETURN_FALSE;
	}

	context = (ALCcontext *)zend_fetch_resource(Z_RES_P(zcontext), PHP_OPENAL_RES_CONTEXT, le_openal_context);

	if (!context)
	{
		php_error_docref(NULL, E_WARNING, "Invalid context.");
		RETURN_FALSE;
	}

	alcMakeContextCurrent(context);

	RETURN_TRUE;
}
/* }}} */

ZEND_BEGIN_ARG_INFO_EX(arginfo_openal_context_process, 0, 0, 1)
ZEND_ARG_INFO(0, context)
ZEND_END_ARG_INFO()

/* {{{ proto bool openal_context_process(resource context)
   Process the specified context */
PHP_FUNCTION(openal_context_process)
{
	zval *zcontext;
	ALCcontext *context;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "r", &zcontext) == FAILURE)
	{
		RETURN_FALSE;
	}

	context = (ALCcontext *)zend_fetch_resource(Z_RES_P(zcontext), PHP_OPENAL_RES_CONTEXT, le_openal_context);

	if (!context)
	{
		php_error_docref(NULL, E_WARNING, "Invalid context.");
		RETURN_FALSE;
	}

	alcProcessContext(context);

	RETURN_TRUE;
}
/* }}} */

ZEND_BEGIN_ARG_INFO_EX(arginfo_openal_context_suspend, 0, 0, 1)
ZEND_ARG_INFO(0, context)
ZEND_END_ARG_INFO()

/* {{{ proto bool openal_context_suspend(resource context)
   Suspend the specified context */
PHP_FUNCTION(openal_context_suspend)
{
	zval *zcontext;
	ALCcontext *context;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "r", &zcontext) == FAILURE)
	{
		RETURN_FALSE;
	}

	context = (ALCcontext *)zend_fetch_resource(Z_RES_P(zcontext), PHP_OPENAL_RES_CONTEXT, le_openal_context);

	if (!context)
	{
		php_error_docref(NULL, E_WARNING, "Invalid context.");
		RETURN_FALSE;
	}

	alcSuspendContext(context);

	RETURN_TRUE;
}
/* }}} */

ZEND_BEGIN_ARG_INFO_EX(arginfo_openal_context_destroy, 0, 0, 1)
ZEND_ARG_INFO(0, context)
ZEND_END_ARG_INFO()

/* {{{ proto bool openal_context_destroy(resource context)
   Destroys a context */
PHP_FUNCTION(openal_context_destroy)
{
	zval *zcontext;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "r", &zcontext) == FAILURE)
	{
		RETURN_FALSE;
	}

	/* Verify the resource is actually an le_openal_context */
	zend_fetch_resource(Z_RES_P(zcontext), PHP_OPENAL_RES_CONTEXT, le_openal_context);
	zend_list_close(Z_RES_P(zcontext));

	RETURN_BOOL(SUCCESS);
}
/* }}} */

/* Buffers */

ZEND_BEGIN_ARG_INFO_EX(arginfo_openal_buffer_create, 0, 0, 0)
ZEND_END_ARG_INFO()

/* {{{ proto resource openal_buffer_create(void)
   Generate OpenAL buffer */
PHP_FUNCTION(openal_buffer_create)
{
	ALuint *buffer = NULL;

	if (ZEND_NUM_ARGS() != 0)
	{
		WRONG_PARAM_COUNT;
	}

	buffer = emalloc(sizeof(ALuint));

	alGenBuffers(1, buffer);

	RETURN_RES(zend_register_resource(buffer, le_openal_buffer));
}
/* }}} */

ZEND_BEGIN_ARG_INFO_EX(arginfo_openal_buffer_data, 0, 0, 4)
ZEND_ARG_INFO(0, buffer)
ZEND_ARG_INFO(0, format)
ZEND_ARG_INFO(0, data)
ZEND_ARG_INFO(0, freq)
ZEND_END_ARG_INFO()

/* {{{ proto bool openal_buffer_data(resource buffer, int format, string data, int freq)
   Load a buffer with data */
PHP_FUNCTION(openal_buffer_data)
{
	zval *zbuffer;
	ALuint *buffer;
	zend_long format, freq;
	size_t data_len;
	char *data;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "rlsl", &zbuffer, &format, &data, &data_len, &freq) == FAILURE)
	{
		RETURN_FALSE;
	}

	if (format != AL_FORMAT_MONO8 &&
		format != AL_FORMAT_MONO16 &&
		format != AL_FORMAT_STEREO8 &&
		format != AL_FORMAT_STEREO16)
	{
		php_error_docref(NULL, E_WARNING, "Invalid format.");
		RETURN_FALSE;
	}

	if (freq < 8000 || freq > 48000)
	{
		php_error_docref(NULL, E_WARNING, "Invalid frequency.");
		RETURN_FALSE;
	}

	if (data_len <= 0)
	{
		php_error_docref(NULL, E_WARNING, "Invalid data length.");
		RETURN_FALSE;
	}

	buffer = (ALuint *)zend_fetch_resource(Z_RES_P(zbuffer), PHP_OPENAL_RES_BUFFER, le_openal_buffer);

	alBufferData(*buffer, (ALenum)format, (ALvoid *)data, (ALsizei)data_len, (ALsizei)freq);

	RETURN_TRUE;
}
/* }}} */

ZEND_BEGIN_ARG_INFO_EX(arginfo_openal_buffer_get, 0, 0, 2)
ZEND_ARG_INFO(0, buffer)
ZEND_ARG_INFO(0, property)
ZEND_END_ARG_INFO()

/* {{{ proto mixed openal_buffer_get(resource buffer, int property)
   Retrieve an OpenAL buffer property */
PHP_FUNCTION(openal_buffer_get)
{
	zval *zbuffer;
	ALuint *buffer;
	zend_long property;
	ALint value;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "rl", &zbuffer, &property) == FAILURE)
	{
		RETURN_FALSE;
	}

	buffer = (ALuint *)zend_fetch_resource(Z_RES_P(zbuffer), PHP_OPENAL_RES_BUFFER, le_openal_buffer);

	if (property == AL_FREQUENCY ||
		property == AL_BITS ||
		property == AL_CHANNELS ||
		property == AL_SIZE)
	{
		alGetBufferi(*buffer, (ALenum)property, &value);
		RETURN_LONG((long)value);
	}
	/* If buffers ever gain float properties
	   we'll want to retrieve them with alGetBufferf */

	RETURN_FALSE;
}
/* }}} */

ZEND_BEGIN_ARG_INFO_EX(arginfo_openal_buffer_destroy, 0, 0, 1)
ZEND_ARG_INFO(0, buffer)
ZEND_END_ARG_INFO()

/* {{{ proto bool openal_buffer_destroy(resource buffer)
   Destroys an OpenAL buffer */
PHP_FUNCTION(openal_buffer_destroy)
{
	zval *zbuffer;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "r", &zbuffer) == FAILURE)
	{
		RETURN_FALSE;
	}

	zend_fetch_resource(Z_RES_P(zbuffer), PHP_OPENAL_RES_BUFFER, le_openal_buffer);
	zend_list_close(Z_RES_P(zbuffer));

	RETURN_BOOL(SUCCESS);
}
/* }}} */

/* Sources */

ZEND_BEGIN_ARG_INFO_EX(arginfo_openal_source_create, 0, 0, 0)
ZEND_END_ARG_INFO()

/* {{{ proto resource openal_source_create()
   Generate a source resource */
PHP_FUNCTION(openal_source_create)
{
	ALuint *source;

	if (ZEND_NUM_ARGS() != 0)
	{
		WRONG_PARAM_COUNT;
	}

	source = emalloc(sizeof(ALuint));

	alGenSources(1, source);

	RETURN_RES(zend_register_resource(source, le_openal_source));
}
/* }}} */

ZEND_BEGIN_ARG_INFO_EX(arginfo_openal_source_get, 0, 0, 2)
ZEND_ARG_INFO(0, source)
ZEND_ARG_INFO(0, property)
ZEND_END_ARG_INFO()

/* {{{ proto mixed openal_source_get(resource source, int property)
   Retrieve an OpenAL source property */
PHP_FUNCTION(openal_source_get)
{
	zval *zsource;
	ALuint *source;
	zend_long property;
	ALint lvalue;
	ALfloat dvalue;
	ALfloat vvalues[3];

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "rl", &zsource, &property) == FAILURE)
	{
		RETURN_FALSE;
	}

	source = (ALuint *)zend_fetch_resource(Z_RES_P(zsource), PHP_OPENAL_RES_SOURCE, le_openal_source);

	if (property == AL_SOURCE_RELATIVE ||
		property == AL_SOURCE_STATE)
	{
		alGetSourcei(*source, (ALenum)property, &lvalue);
		RETURN_LONG((long)lvalue);
	}
	if (property == AL_PITCH ||
		property == AL_GAIN ||
		property == AL_MIN_GAIN ||
		property == AL_MAX_GAIN ||
		property == AL_MAX_DISTANCE ||
		property == AL_ROLLOFF_FACTOR ||
		property == AL_CONE_OUTER_GAIN ||
		property == AL_CONE_INNER_ANGLE ||
		property == AL_CONE_OUTER_ANGLE ||
		property == AL_REFERENCE_DISTANCE)
	{
		alGetSourcef(*source, (ALenum)property, &dvalue);
		RETURN_DOUBLE((double)dvalue);
	}
	if (property == AL_POSITION ||
		property == AL_VELOCITY ||
		property == AL_DIRECTION)
	{
		alGetSourcefv(*source, (ALenum)property, vvalues);
		array_init(return_value);
		add_next_index_double(return_value, vvalues[0]);
		add_next_index_double(return_value, vvalues[1]);
		add_next_index_double(return_value, vvalues[2]);
		return;
	}

	php_error_docref(NULL, E_WARNING, "Invalid property.");
	RETURN_FALSE;
}
/* }}} */

ZEND_BEGIN_ARG_INFO_EX(arginfo_openal_source_set, 0, 0, 3)
ZEND_ARG_INFO(0, source)
ZEND_ARG_INFO(0, property)
ZEND_ARG_INFO(0, setting)
ZEND_END_ARG_INFO()

/* {{{ proto bool openal_source_set(resource source, int property, mixed setting)
   Set source property */
PHP_FUNCTION(openal_source_set)
{
	zval *zsource, *zsetting, *zvalue;
	ALuint *source, *buffer;
	long i;
	zend_long property;
	ALfloat vvalues[3];

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "rlz", &zsource, &property, &zsetting) == FAILURE)
	{
		RETURN_FALSE;
	}

	source = (ALuint *)zend_fetch_resource(Z_RES_P(zsource), PHP_OPENAL_RES_SOURCE, le_openal_source);

	if (property == AL_BUFFER)
	{
		if (Z_TYPE_P(zsetting) != IS_RESOURCE)
		{
			php_error_docref(NULL, E_WARNING, "Non resource passed while setting buffer.");
			RETURN_FALSE;
		}
		buffer = (ALuint *)zend_fetch_resource(Z_RES_P(zsetting), PHP_OPENAL_RES_BUFFER, le_openal_buffer);
		alSourcei(*source, (ALenum)property, (ALint)(*buffer));
		RETURN_TRUE;
	}
	if (property == AL_LOOPING)
	{
		if (Z_TYPE_P(zsetting) == IS_TRUE || Z_TYPE_P(zsetting) == IS_FALSE)
		{
			/* Let PHP's true/false act like OpenAL's AL_TRUE/AL_FALSE */
			alSourcei(*source, (ALenum)property, Z_LVAL_P(zsetting) == IS_TRUE ? AL_TRUE : AL_FALSE);
		}
		else
		{
			convert_to_long_ex(zsetting);
			alSourcei(*source, (ALenum)property, (ALint)Z_LVAL_P(zsetting));
		}
		RETURN_TRUE;
	}
	if (property == AL_SOURCE_RELATIVE ||
		property == AL_SOURCE_STATE)
	{
		convert_to_long_ex(zsetting);
		alSourcei(*source, (ALenum)property, (ALint)Z_LVAL_P(zsetting));
		RETURN_TRUE;
	}
	if (property == AL_PITCH ||
		property == AL_GAIN ||
		property == AL_MIN_GAIN ||
		property == AL_MAX_GAIN ||
		property == AL_MAX_DISTANCE ||
		property == AL_ROLLOFF_FACTOR ||
		property == AL_CONE_OUTER_GAIN ||
		property == AL_CONE_INNER_ANGLE ||
		property == AL_CONE_OUTER_ANGLE ||
		property == AL_REFERENCE_DISTANCE)
	{
		convert_to_double_ex(zsetting);
		alSourcef(*source, (ALenum)property, (ALfloat)Z_DVAL_P(zsetting));
		RETURN_TRUE;
	}
	if (property == AL_POSITION ||
		property == AL_VELOCITY ||
		property == AL_DIRECTION)
	{
		if (Z_TYPE_P(zsetting) != IS_ARRAY)
		{
			php_error_docref(NULL, E_WARNING, "Non array setting passed for array property.");
			RETURN_FALSE;
		}
		for (i = 0; i < 3; i++)
		{
			if ((zvalue = zend_hash_index_find(HASH_OF(zsetting), i)) == NULL)
			{
				php_error_docref(NULL, E_WARNING, "setting[%ld] not found.", i);
				RETURN_FALSE;
			}
			convert_to_double_ex(zvalue);
			vvalues[i] = (ALfloat)Z_DVAL_P(zvalue);
		}
		alSourcefv(*source, (ALenum)property, vvalues);
		RETURN_TRUE;
	}

	php_error_docref(NULL, E_WARNING, "Invalid property.");
	RETURN_FALSE;
}
/* }}} */

ZEND_BEGIN_ARG_INFO_EX(arginfo_openal_source_play, 0, 0, 1)
ZEND_ARG_INFO(0, source)
ZEND_END_ARG_INFO()

/* {{{ proto bool openal_source_play(resource source)
   Start playing the source */
PHP_FUNCTION(openal_source_play)
{
	zval *zsource;
	ALuint *source;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "r", &zsource) == FAILURE)
	{
		RETURN_FALSE;
	}

	source = (ALuint *)zend_fetch_resource(Z_RES_P(zsource), PHP_OPENAL_RES_SOURCE, le_openal_source);

	alSourcePlay(*source);

	RETURN_TRUE;
}
/* }}} */

ZEND_BEGIN_ARG_INFO_EX(arginfo_openal_source_pause, 0, 0, 1)
ZEND_ARG_INFO(0, source)
ZEND_END_ARG_INFO()

/* {{{ proto bool openal_source_pause(resource source)
   Pause the source */
PHP_FUNCTION(openal_source_pause)
{
	zval *zsource;
	ALuint *source;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "r", &zsource) == FAILURE)
	{
		RETURN_FALSE;
	}

	source = (ALuint *)zend_fetch_resource(Z_RES_P(zsource), PHP_OPENAL_RES_SOURCE, le_openal_source);

	alSourcePause(*source);

	RETURN_TRUE;
}
/* }}} */

ZEND_BEGIN_ARG_INFO_EX(arginfo_openal_source_stop, 0, 0, 1)
ZEND_ARG_INFO(0, source)
ZEND_END_ARG_INFO()

/* {{{ proto bool openal_source_stop(resource source)
   Stop playing the source */
PHP_FUNCTION(openal_source_stop)
{
	zval *zsource;
	ALuint *source;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "r", &zsource) == FAILURE)
	{
		RETURN_FALSE;
	}

	source = (ALuint *)zend_fetch_resource(Z_RES_P(zsource), PHP_OPENAL_RES_SOURCE, le_openal_source);

	alSourceStop(*source);

	RETURN_TRUE;
}
/* }}} */

ZEND_BEGIN_ARG_INFO_EX(arginfo_openal_source_rewind, 0, 0, 1)
ZEND_ARG_INFO(0, source)
ZEND_END_ARG_INFO()

/* {{{ proto bool openal_source_rewind(resource source)
   Rewind the source */
PHP_FUNCTION(openal_source_rewind)
{
	zval *zsource;
	ALuint *source;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "r", &zsource) == FAILURE)
	{
		RETURN_FALSE;
	}

	source = (ALuint *)zend_fetch_resource(Z_RES_P(zsource), PHP_OPENAL_RES_SOURCE, le_openal_source);

	alSourceRewind(*source);

	RETURN_TRUE;
}
/* }}} */

ZEND_BEGIN_ARG_INFO_EX(arginfo_openal_source_destroy, 0, 0, 1)
ZEND_ARG_INFO(0, source)
ZEND_END_ARG_INFO()

/* {{{ proto resource openal_source_destroy(resource source)
   Destroy a source resource */
PHP_FUNCTION(openal_source_destroy)
{
	zval *zsource;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "r", &zsource) == FAILURE)
	{
		RETURN_FALSE;
	}

	zend_fetch_resource(Z_RES_P(zsource), PHP_OPENAL_RES_SOURCE, le_openal_source);
	zend_list_close(Z_RES_P(zsource));

	RETURN_BOOL(SUCCESS);
}
/* }}} */

/* Listener */

ZEND_BEGIN_ARG_INFO_EX(arginfo_openal_listener_set, 0, 0, 1)
ZEND_ARG_INFO(0, property)
ZEND_ARG_INFO(0, setting)
ZEND_END_ARG_INFO()

/* {{{ proto bool openal_listener_set(int property, mixed setting)
   Set a listener property */
PHP_FUNCTION(openal_listener_set)
{
	zend_long property;
	long i;
	zval *zsetting, *zvalue;
	ALfloat vvalues[6];

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "lz", &property, &zsetting) == FAILURE)
	{
		RETURN_FALSE;
	}

	if (property == AL_GAIN)
	{
		convert_to_double_ex(zsetting);
		alListenerf((ALenum)property, Z_DVAL_P(zsetting));
		RETURN_TRUE;
	}
	if (property == AL_POSITION ||
		property == AL_VELOCITY ||
		property == AL_ORIENTATION)
	{
		if (Z_TYPE_P(zsetting) != IS_ARRAY)
		{
			php_error_docref(NULL, E_WARNING, "Non array setting passed for array property.");
			RETURN_FALSE;
		}
		for (i = 0; i < (property == AL_ORIENTATION ? 6 : 3); i++)
		{
			if ((zvalue = zend_hash_index_find(HASH_OF(zsetting), i)) == NULL)
			{
				php_error_docref(NULL, E_WARNING, "setting[%ld] not found.", i);
				RETURN_FALSE;
			}
			convert_to_double_ex(zvalue);
			vvalues[i] = (ALfloat)Z_DVAL_P(zvalue);
		}
		alListenerfv((ALenum)property, vvalues);
		RETURN_TRUE;
	}

	php_error_docref(NULL, E_WARNING, "Invalid property.");
	RETURN_FALSE;
}
/* }}} */

ZEND_BEGIN_ARG_INFO_EX(arginfo_openal_listener_get, 0, 0, 1)
ZEND_ARG_INFO(0, property)
ZEND_END_ARG_INFO()

/* {{{ proto mixed openal_listener_get(int property)
   Retrieve a listener property */
PHP_FUNCTION(openal_listener_get)
{
	zend_long property;
	ALfloat fval;
	ALfloat vval[6];

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &property) == FAILURE)
	{
		RETURN_FALSE;
	}

	/* If listeners ever support integer properties
	   we'll have to handle them here */
	if (property == AL_GAIN)
	{
		alGetListenerf(property, &fval);
		RETURN_DOUBLE((double)fval);
	}
	if (property == AL_POSITION ||
		property == AL_VELOCITY ||
		property == AL_ORIENTATION)
	{
		alGetListenerfv(property, vval);
		array_init(return_value);
		add_next_index_double(return_value, vval[0]);
		add_next_index_double(return_value, vval[1]);
		add_next_index_double(return_value, vval[2]);
		if (property == AL_ORIENTATION)
		{
			add_next_index_double(return_value, vval[3]);
			add_next_index_double(return_value, vval[4]);
			add_next_index_double(return_value, vval[5]);
		}
		return;
	}

	php_error_docref(NULL, E_WARNING, "Invalid property.");
	RETURN_FALSE;
}
/* }}} */

/* End of userspace exposure */

/* Module Housekeeping */

static void php_openal_device_dtor(zend_resource *rsrc)
{
	ALCdevice *dev = (ALCdevice *)rsrc->ptr;

	if (dev)
	{
		alcCloseDevice(dev);
	}
}

static void php_openal_context_dtor(zend_resource *rsrc)
{
	ALCcontext *context = (ALvoid *)rsrc->ptr;

	if (context)
	{
		alcDestroyContext(context);
	}
}

static void php_openal_buffer_dtor(zend_resource *rsrc)
{
	ALuint *buffer = (ALuint *)rsrc->ptr;

	if (buffer)
	{
		alDeleteBuffers(1, buffer);
		efree(buffer);
	}
}

static void php_openal_source_dtor(zend_resource *rsrc)
{
	ALuint *source = (ALuint *)rsrc->ptr;

	if (source)
	{
		alDeleteSources(1, source);
		efree(source);
	}
}

zend_function_entry openal_functions[] = {
	PHP_FE(openal_device_open, arginfo_openal_device_open)
		PHP_FE(openal_device_close, arginfo_openal_device_close)
			PHP_FE(openal_context_create, arginfo_openal_context_create)
				PHP_FE(openal_context_current, arginfo_openal_context_current)
					PHP_FE(openal_context_process, arginfo_openal_context_process)
						PHP_FE(openal_context_suspend, arginfo_openal_context_suspend)
							PHP_FE(openal_context_destroy, arginfo_openal_context_destroy)
								PHP_FE(openal_buffer_create, arginfo_openal_buffer_create)
									PHP_FE(openal_buffer_get, arginfo_openal_buffer_get)
										PHP_FE(openal_buffer_data, arginfo_openal_buffer_data)
											PHP_FE(openal_buffer_destroy, arginfo_openal_buffer_destroy)
												PHP_FE(openal_source_create, arginfo_openal_source_create)
													PHP_FE(openal_source_get, arginfo_openal_source_get)
														PHP_FE(openal_source_set, arginfo_openal_source_set)
															PHP_FE(openal_source_play, arginfo_openal_source_play)
																PHP_FE(openal_source_pause, arginfo_openal_source_pause)
																	PHP_FE(openal_source_stop, arginfo_openal_source_stop)
																		PHP_FE(openal_source_rewind, arginfo_openal_source_rewind)
																			PHP_FE(openal_source_destroy, arginfo_openal_source_destroy)
																				PHP_FE(openal_listener_get, arginfo_openal_listener_get)
																					PHP_FE(openal_listener_set, arginfo_openal_listener_set)
#ifdef HAVE_OPENAL_STREAM
																						PHP_FE(openal_stream, NULL)
#endif
																							{NULL, NULL, NULL}};

zend_module_entry openal_module_entry = {
	STANDARD_MODULE_HEADER,
	"openal",
	openal_functions,
	PHP_MINIT(openal),
	NULL, /* MSHUTOWN */
	NULL, /* RINIT */
	NULL, /* RSHUTDOWN */
	PHP_MINFO(openal),
	PHP_OPENAL_VERSION,
	STANDARD_MODULE_PROPERTIES};

#ifdef COMPILE_DL_OPENAL
ZEND_GET_MODULE(openal)
#endif

PHP_MINIT_FUNCTION(openal)
{
	/* Context Attributes */
	REGISTER_LONG_CONSTANT("ALC_FREQUENCY", ALC_FREQUENCY, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("ALC_REFRESH", ALC_REFRESH, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("ALC_SYNC", ALC_SYNC, CONST_CS | CONST_PERSISTENT);

	/* Buffer Settings */
	REGISTER_LONG_CONSTANT("AL_FREQUENCY", AL_FREQUENCY, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("AL_BITS", AL_BITS, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("AL_CHANNELS", AL_CHANNELS, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("AL_SIZE", AL_SIZE, CONST_CS | CONST_PERSISTENT);

	/* Source & Listener Settings */
	/*   Buffer (Integer) */
	REGISTER_LONG_CONSTANT("AL_BUFFER", AL_BUFFER, CONST_CS | CONST_PERSISTENT);
	/*   Integer settings */
	REGISTER_LONG_CONSTANT("AL_SOURCE_RELATIVE", AL_SOURCE_RELATIVE, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("AL_SOURCE_STATE", AL_SOURCE_STATE, CONST_CS | CONST_PERSISTENT);
	/*   Float settings */
	REGISTER_LONG_CONSTANT("AL_PITCH", AL_PITCH, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("AL_GAIN", AL_GAIN, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("AL_MIN_GAIN", AL_MIN_GAIN, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("AL_MAX_GAIN", AL_MAX_GAIN, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("AL_MAX_DISTANCE", AL_MAX_DISTANCE, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("AL_ROLLOFF_FACTOR", AL_ROLLOFF_FACTOR, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("AL_CONE_OUTER_GAIN", AL_CONE_OUTER_GAIN, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("AL_CONE_INNER_ANGLE", AL_CONE_INNER_ANGLE, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("AL_CONE_OUTER_ANGLE", AL_CONE_OUTER_ANGLE, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("AL_REFERENCE_DISTANCE", AL_REFERENCE_DISTANCE, CONST_CS | CONST_PERSISTENT);
	/*   Float Vector Settings */
	REGISTER_LONG_CONSTANT("AL_POSITION", AL_POSITION, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("AL_VELOCITY", AL_VELOCITY, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("AL_DIRECTION", AL_DIRECTION, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("AL_ORIENTATION", AL_ORIENTATION, CONST_CS | CONST_PERSISTENT);

	/* PCM Formats */
	REGISTER_LONG_CONSTANT("AL_FORMAT_MONO8", AL_FORMAT_MONO8, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("AL_FORMAT_MONO16", AL_FORMAT_MONO16, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("AL_FORMAT_STEREO8", AL_FORMAT_STEREO8, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("AL_FORMAT_STEREO16", AL_FORMAT_STEREO16, CONST_CS | CONST_PERSISTENT);

	REGISTER_LONG_CONSTANT("AL_INITIAL", AL_INITIAL, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("AL_PLAYING", AL_PLAYING, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("AL_PAUSED", AL_PAUSED, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("AL_STOPPED", AL_STOPPED, CONST_CS | CONST_PERSISTENT);

	REGISTER_LONG_CONSTANT("AL_TRUE", AL_TRUE, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("AL_FALSE", AL_FALSE, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("AL_LOOPING", AL_LOOPING, CONST_CS | CONST_PERSISTENT);

	if ((le_openal_device = zend_register_list_destructors_ex(php_openal_device_dtor, NULL, PHP_OPENAL_RES_DEVICE, module_number)) == FAILURE)
	{
		return FAILURE;
	}
	if ((le_openal_context = zend_register_list_destructors_ex(php_openal_context_dtor, NULL, PHP_OPENAL_RES_CONTEXT, module_number)) == FAILURE)
	{
		return FAILURE;
	}
	if ((le_openal_buffer = zend_register_list_destructors_ex(php_openal_buffer_dtor, NULL, PHP_OPENAL_RES_BUFFER, module_number)) == FAILURE)
	{
		return FAILURE;
	}
	if ((le_openal_source = zend_register_list_destructors_ex(php_openal_source_dtor, NULL, PHP_OPENAL_RES_SOURCE, module_number)) == FAILURE)
	{
		return FAILURE;
	}

	return SUCCESS;
}

PHP_MINFO_FUNCTION(openal)
{
	php_info_print_table_start();
	php_info_print_table_row(2, "OpenAL Extension", "enabled");
	php_info_print_table_row(2, "Version", PHP_OPENAL_VERSION);
	php_info_print_table_end();
}

#endif /* WITH_OPENAL */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 fdm=marker
 * vim<600: sw=4 ts=4
 */
