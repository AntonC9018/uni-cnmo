# CNMO (Calcul Numeric și Metodele de Optimizare)

This repository contains assignments, implemented as part of the course **Calcul Numeric și Metodele de Optimizare** (**Numerical Methods and Optimization**).

It is one monolithic project, including 2 assignments in one. For explanation documents, see the markdown files in the `doc` folder. The first document explains general setup of the project. Assignments are all in *Romanian*.

> The markdown files use [tex math formulas, which won't show on github](https://stackoverflow.com/questions/11256433/how-to-show-math-equations-in-general-githubs-markdownnot-githubs-blog). To see formulas, you will either have to convert markdown to html or pdf, with [`MathJax`](https://www.mathjax.org/) enabled, or find the compiled pdf's [on my google drive](https://drive.google.com/drive/folders/1Rs0-qy6ivSDuHh5JadrP4Ta4YDhuVRiC).

The compiled program can be downloaded right from github, see [releases](https://github.com/AntonC9018/uni-cnmo/releases). It is compiled for Windows.

PR's with grammar corrections, bug fixes, improvement suggestions or translations are very welcome.

Leave a star as a way to say "Thank you". Enjoy!

## Build requirements

- CMake 3.21 or newer
- A C/C++ toolchain with C++17 support
- Qt 5 or Qt 6 with the Widgets module (Qt 5 is preferred when both are installed)
- Qwt 6 or newer

Qt, Qwt, and the compiler must use the same architecture and ABI. On Windows,
for example, do not mix a MinGW Qt installation with an MSVC build of Qwt. A
standard Qwt installation should contain its headers under `include/qwt`, its
link library under `lib`, and, for a shared build, its DLL under `bin` or `lib`.

## Install the dependencies on Windows

The tested and simplest setup uses the 64-bit UCRT toolchain from
[MSYS2](https://www.msys2.org/). Install MSYS2, open the **MSYS2 UCRT64**
terminal, update its package database and installed packages, and then install
the complete build toolchain:

```sh
pacman -Syu
pacman -S --needed \
  mingw-w64-ucrt-x86_64-toolchain \
  mingw-w64-ucrt-x86_64-cmake \
  mingw-w64-ucrt-x86_64-ninja \
  mingw-w64-ucrt-x86_64-qt5-base \
  mingw-w64-ucrt-x86_64-qwt-qt5 \
  git
```

If the first update asks you to close the terminal, do so, reopen the UCRT64
terminal, and run `pacman -Syu` again before installing the packages. Keep all
of Qt, Qwt, and GCC in the UCRT64 environment; packages from the MINGW64,
CLANG64, or MSYS environments are not ABI-compatible with this build.

Clone the project and its bundled source dependencies from that same terminal:

```sh
git clone --recurse-submodules https://github.com/AntonC9018/uni-cnmo.git
cd uni-cnmo
```

Then configure, build, and package a release:

```sh
cmake -S . -B build -G Ninja \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_PREFIX_PATH=/ucrt64 \
  -DQwt_ROOT=/ucrt64
cmake --build build --parallel
cmake --build build --target package
```

These are the exact dependency and build commands used to validate the Windows
package. `windeployqt` is used automatically when the selected Qt installation
provides it; the MSYS2 Qt layout is also supported.

## Configure and build

Clone the repository together with its submodules:

```sh
git clone --recurse-submodules https://github.com/AntonC9018/uni-cnmo.git
cd uni-cnmo
```

For an existing clone, initialize the submodules with:

```sh
git submodule update --init --recursive
```

Run CMake from a shell configured for the compiler that matches the selected Qt
and Qwt installations:

```sh
cmake -S . -B build -DCMAKE_PREFIX_PATH="<path-to-Qt-kit>" -DQwt_ROOT="<path-to-Qwt-install>"
cmake --build build --config Release --parallel
```

`CMAKE_PREFIX_PATH` should point to the Qt kit itself, such as
`C:/Qt/5.15.2/mingw81_64`. `Qwt_ROOT` is only required when Qwt is outside the
toolchain's normal search paths.

With a single-configuration generator such as Ninja, select the release
configuration while configuring:

```sh
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH="<path-to-Qt-kit>" -DQwt_ROOT="<path-to-Qwt-install>"
cmake --build build --parallel
```

## Create the distributable ZIP

Build the CMake `package` target:

```sh
cmake --build build --config Release --target package
```

The archive and its SHA-256 checksum are written to `build/packages`, for
example:

```text
build/packages/cnmo-0.1.0-windows-x86_64.zip
build/packages/cnmo-0.1.0-windows-x86_64.zip.sha256
```

On Windows, packaging runs `windeployqt` and CMake's runtime dependency
scanner. The ZIP contains `cnmo.exe`, the Qwt runtime when Qwt is shared, the
required Qt and compiler DLLs, Qt platform plugins, and the license, so it can
be extracted and run on another machine without a Qt development installation.
