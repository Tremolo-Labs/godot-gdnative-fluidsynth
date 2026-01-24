godot-gdnative-fluidsynth
=========================

Godot gdnative fluidsynth library to allow playing music using fluidsynth.

## Contents
* An empty Godot project (`demo/`)
* godot-cpp as a submodule (`godot-cpp/`)
* GitHub Issues template (`.github/ISSUE_TEMPLATE.yml`)
* GitHub CI/CD workflows to publish your library packages when creating a release (`.github/workflows/builds.yml`)
* preconfigured source files for C++ development of the GDExtension (`src/`)
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

## Usage - Actions

This repository comes with a GitHub action that builds the GDExtension for cross-platform use. It triggers automatically for each pushed change. You can find and edit it in [builds.yml](.github/workflows/builds.yml).
After a workflow run is complete, you can find the file `godot-cpp-template.zip` on the `Actions` tab on GitHub.

## Old README


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
