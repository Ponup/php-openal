<?php declare(strict_types=1);

dl('openal.so');

require 'WaveFileReader.php';

// https://freesound.org/people/Ironi%20Alef/sounds/32226/
$wave = WaveFileReader::ReadFile('sound.wav', true);
$waveData = $wave['subchunk2']['data'];
$waveSampleRate = $wave['subchunk1']['samplerate'];
$waveLength = ceil(strlen($waveData) / ($waveSampleRate * $wave['subchunk1']['numchannels'] * $wave['subchunk1']['bitspersample'] /8));

$device = openal_device_open();
$context = openal_context_create($device);
openal_context_current($context);

$source = openal_source_create();
$buffer = openal_buffer_create();
openal_buffer_data($buffer, AL_FORMAT_STEREO16, $waveData, $waveSampleRate);

openal_source_set($source, AL_BUFFER, $buffer);
openal_source_set($source, AL_LOOPING, true);

openal_source_play($source);
sleep((int)$waveLength);
openal_source_stop($source);

openal_context_destroy($context);
openal_device_close($device);

