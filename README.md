godot-gdnative-fluidsynth
=========================

Godot gdnative fluidsynth library to allow playing music using fluidsynth.

How to Install
--------------

Install system dependencies for Ubuntu:

    apt install fluidsynth libfluidsynth-dev abcmidi scons

Build
-----

Initialize git submodules:

    git submodule update --init --recursive

Create assets:

    make assets

Compile godot-cpp library:

    make godot-cpp

Compile gdnative library:

    make
