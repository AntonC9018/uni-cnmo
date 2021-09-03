# CNMO (Calcul Numeric și Metodele de Optimizare)

This repository contains assignments, implemented as part of the course **Calcul Numeric și Metodele de Optimizare** (**Numerical Methods and Optimization**).

It is one monolithic project, including 2 assignments in one. For explanation documents, see the markdown files in the `doc` folder. The first document explains general setup of the project. Assignments are all in *Romanian*.

> The markdown files use [tex math formulas, which won't show on github](https://stackoverflow.com/questions/11256433/how-to-show-math-equations-in-general-githubs-markdownnot-githubs-blog). To see formulas, you will either have to convert markdown to html or pdf, with [`MathJax`](https://www.mathjax.org/) enabled, or find the compiled pdf's [on my google drive](https://drive.google.com/drive/folders/1Rs0-qy6ivSDuHh5JadrP4Ta4YDhuVRiC).

The compiled program can be downloaded right from github, see [releases](https://github.com/AntonC9018/uni-cnmo/releases). It is compiled for Windows.

PR's with grammar corrections, bug fixes, improvement suggestions or translations are very welcome.

Leave a star as a way to say "Thank you". Enjoy!

## Approximate build instructions

1. `git clone --recursive https://github.com/AntonC9018/uni-cnmo`. `--recursive` is needed to load submodules.
2. Install `Qt` version 5 (I think). Newer versions will likely cause errors in `Qwt`. You will need to also install `CMake` and `MinGW` or `Cygwin`. They come with `Qt`, if you don't have them already installed.
3. Download `Qwt` source code and build it. 
4. Adjust the command in `run.bat` with your paths to compilers and the libraries.
5. Adjust the path to `libs` in `CMakeLists.txt`.
6. Build & Run with `run`.