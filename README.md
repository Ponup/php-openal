# PHP-OpenAL

**OpenAL** bindings for PHP.

```php
$device = openal_device_open(null);
$context = openal_context_create($device);
openal_context_current($context);

$source = openal_source_create();
$buffer = openal_buffer_create();
openal_buffer_data($buffer, AL_FORMAT_STEREO16, $wavData, $wavSampleRate);

openal_source_set($source, AL_BUFFER, $buffer);
openal_source_set($source, AL_LOOPING, true);

openal_source_play($source);
sleep($wavLength);
openal_source_stop($source);

openal_context_destroy($context);
openal_device_close($device);
```

## Requirements

- PHP7
- OpenAL library and headers

## Documentation

https://www.php.net/manual/en/book.openal.php

## Building

```sh
$ phpize
$ ./configure --with-openal=/usr/local/Cellar/openal-soft/1.19.1 # Adjust path accordingly
$ make
$ sudo make install
```

## Packaging

```sh
$ pear package # This will generate an openal-X.X.X.tgz to push to pecl.php.net
```

