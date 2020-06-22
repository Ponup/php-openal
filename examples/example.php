<?php declare(strict_types=1);

require __DIR__ . '/vendor/autoload.php';

use Mammoth\Audio\WaveFileReader;

// https://freesound.org/people/Ironi%20Alef/sounds/32226/
$waveFileReader = new WaveFileReader(__DIR__ . '/sound.wav');
$wave = $waveFileReader->read(true);

echo 'Wave loaded. Format is ' . $wave->getAudioFormat(), PHP_EOL;

$waveData = $wave->getData();
$waveSampleRate = $wave->getSampleRate();
$waveLength = ceil(strlen($waveData) / ($waveSampleRate * $wave->getNumChannels() * $wave->getBitsPerSample() / 8));

$device = openal_device_open();
$context = openal_context_create($device);
openal_context_current($context);

$source = openal_source_create();
$buffer = openal_buffer_create();
openal_buffer_data($buffer, AL_FORMAT_STEREO16, $waveData, $waveSampleRate);

openal_source_set($source, AL_BUFFER, $buffer);
openal_source_set($source, AL_LOOPING, false);

openal_source_play($source);
sleep((int)$waveLength);
openal_source_stop($source);

openal_context_destroy($context);
openal_device_close($device);

