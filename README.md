# CNMO (Calcul Numeric și Metodele de Optimizare)

This repository contains assignments, implemented as part of the course **Calcul Numeric și Metodele de Optimizare** (**Numerical Methods and Optimization**).

It is one monolithic project, including 2 assignments in one. For explanation documents, see the markdown files in the `doc` folder. The first document explains general setup of the project. Assignments are all in *Romanian*.

> The markdown files use [tex math formulas, which won't show on github](https://stackoverflow.com/questions/11256433/how-to-show-math-equations-in-general-githubs-markdownnot-githubs-blog). To see formulas, you will either have to convert markdown to html or pdf, with [`MathJax`](https://www.mathjax.org/) enabled, or find the compiled pdf's [on my google drive](https://drive.google.com/drive/folders/1Rs0-qy6ivSDuHh5JadrP4Ta4YDhuVRiC).

The compiled program can be downloaded right from github, see [releases](https://github.com/AntonC9018/uni-cnmo/releases). It is compiled for Windows.

PR's with grammar corrections, bug fixes, improvement suggestions or translations are very welcome.

Leave a star as a way to say "Thank you". Enjoy!

## Building and installing

### Requirements

- A C++17 compiler
- CMake 3.16 or newer
- Ninja (recommended; another CMake generator also works)
- Qt 5 or Qt 6 with the Widgets module
- Qwt built against the same Qt major version

The bundled `strlib`, `tinyexpr`, and `mydefines` dependencies are Git
submodules and do not need to be installed separately.

On Ubuntu/Debian with Qt 5, the prerequisites can typically be installed with:

```sh
sudo apt install build-essential cmake ninja-build qtbase5-dev libqwt-qt5-dev
```

On macOS with Homebrew:

```sh
brew install cmake ninja qt qwt
```

On Windows, use an MSYS2 MinGW shell and install the matching compiler, CMake,
Ninja, Qt, and Qwt packages. Do not mix MSVC-built and MinGW-built libraries in
one build.

### Configure and build

Clone the repository together with its submodules:

```sh
git clone --recursive https://github.com/AntonC9018/uni-cnmo.git
cd uni-cnmo
```

If the repository was cloned without `--recursive`, initialize the submodules:

```sh
git submodule update --init --recursive
```

The included presets use Ninja:

```sh
cmake --preset default
cmake --build --preset default
```

For an optimized build:

```sh
cmake --preset release
cmake --build --preset release
```

Without presets, any generator can be used:

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

If Qwt is installed in a non-standard prefix, pass it explicitly:

```sh
cmake -S . -B build -DQwt_ROOT=/path/to/qwt
```

As a last resort, set `QWT_INCLUDE_DIR` and `QWT_LIBRARY` to the exact include
directory and library file. If Qt itself is in a non-standard prefix, add that
prefix to `CMAKE_PREFIX_PATH`.

To install the built application under a chosen prefix:

```sh
cmake --install build-release --prefix ./install
```

The executable is installed into `bin` on Linux and Windows, or as an
application bundle on macOS. Packaging the Qt and Qwt runtime libraries is
platform-specific and is not performed automatically.

### Headless/core-only build

The bundled libraries can be built without Qt or Qwt:

```sh
cmake -S . -B build-core -G Ninja -DCNMO_BUILD_APP=OFF
cmake --build build-core
```
