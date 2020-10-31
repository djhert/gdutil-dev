# gdutil-dev

Development and working example for GDUtil

## Using

Download this git repository and all submodules:
````bash
$ git clone --recursive https://github.com/hlfstr/gdutil-dev.git
````

Issue a build:
````bash
$ cd gdutil-dev/source
$ mkdir build
$ cd build
$ cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON  ..
$ make -j$(nproc)
````

Alternatively, you can issue a build on the `source` folder from your IDE.

Load Godot from the `gdutil-dev` folder to edit, or import the `gdutil-dev` folder into Godot