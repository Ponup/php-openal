[![Build Status](https://travis-ci.org/Ponup/php-openal.svg?branch=master)](https://travis-ci.org/Ponup/php-openal)

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

- PHP8.1
- OpenAL library and headers

## Documentation

https://www.php.net/manual/en/book.openal.php

## Docker

```sh
docker build -t php-openal .
docker run -it --device /dev/snd -e PULSE_SERVER=/tmp/pulse-native -e PULSE_COOKIE=/tmp/pulse-cookie -v $XDG_RUNTIME_DIR/pulse/native:/tmp/pulse-native -v ~/.config/pulse/cookie:/tmp/pulse-cookie --group-add audio --entrypoint php php-openal examples/example.php
```

## Building

```sh
phpize
./configure --with-openal=/usr/local/Cellar/openal-soft/1.19.1 # Adjust path accordingly
make
sudo make install
```

## Packaging

```sh
pear package # This will generate an openal-X.X.X.tgz to push to pecl.php.net
```

