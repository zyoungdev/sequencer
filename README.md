# Sequencer

A flexible sequencer built in Qt Qml.

## Features

- Selectable width and height
- Tempo
- Selectable root notes
- Selectable chord quality
- Random patterns

## Prerequisites

- C++20
- CMake
- Qt 5 (Core, Widgets, Quick, Multimedia)
- SDL2

## Build

``` bash
$ git clone --recurse-submodules https://github.com/zerovectorspace/sequencer
$ mkdir sequencer_build
$ cd sequencer_build
$ cmake ../sequencer
$ ./sequencer
```

## Samples

You must supply your own samples. They must be placed in the `./res/wav` directory. Here is a list of the filenames.

<details>
<summary>List of files</summary>
<pre>
A1.wav
A2.wav
A3.wav
A4.wav
A5.wav
Ab1.wav
Ab2.wav
Ab3.wav
Ab4.wav
Ab5.wav
B1.wav
B2.wav
B3.wav
B4.wav
B5.wav
Bb1.wav
Bb2.wav
Bb3.wav
Bb4.wav
Bb5.wav
C1.wav
C2.wav
C3.wav
C4.wav
C5.wav
D1.wav
D2.wav
D3.wav
D4.wav
D5.wav
Db1.wav
Db2.wav
Db3.wav
Db4.wav
Db5.wav
E1.wav
E2.wav
E3.wav
E4.wav
E5.wav
Eb1.wav
Eb2.wav
Eb3.wav
Eb4.wav
Eb5.wav
F1.wav
F2.wav
F3.wav
F4.wav
F5.wav
G1.wav
G2.wav
G3.wav
G4.wav
G5.wav
Gb1.wav
Gb2.wav
Gb3.wav
Gb4.wav
Gb5.wav
</pre>
</details>

