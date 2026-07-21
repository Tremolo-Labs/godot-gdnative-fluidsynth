godot-gdextension-fluidsynth
============================

Godot GDExtension fluidsynth library to allow playing music using fluidsynth.

## Contents
* godot-cpp as a submodule (`godot-cpp/`) — version 10.x (targeting Godot 4.7)
* GitHub CI/CD workflows to build and publish library packages (`.github/workflows/builds.yml`)
* preconfigured C++ GDExtension source files (`src/`)
* setup to automatically generate `.xml` files in a `doc_classes/` directory to be parsed by Godot as [GDExtension built-in documentation](https://docs.godotengine.org/en/stable/tutorials/scripting/gdextension/gdextension_docs_system.html)


### Configuring an IDE
You can develop your own extension with any text editor and by invoking scons on the command line, but if you want to work with an IDE (Integrated Development Environment), you can use a compilation database file called `compile_commands.json`. Most IDEs should automatically identify this file, and self-configure appropriately.
To generate the database file, you can run one of the following commands in the project root directory:
```shell
# Generate compile_commands.json while compiling
scons compiledb=yes

# Generate compile_commands.json without compiling
scons compiledb=yes compile_commands.json
```

## Building

Install system dependencies for Ubuntu:

    sudo apt install fluidsynth libfluidsynth-dev scons

Initialize git submodules:

    git submodule update --init --recursive

Build:

    scons api_version=4.7
