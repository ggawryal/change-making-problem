# Change-making-problem
Collection of algorithms for [change-making problem](https://en.wikipedia.org/wiki/Change-making_problem), written in C++ using [FFTW](http://www.fftw.org) library.

## Algorithm descriptions
-  T. M. Chan and Q. He: **On the Change-Making Problem** https://tmc.web.engr.illinois.edu/coin_sosa.pdf
-  T. M. Chan and Q. He: **More on Change-Making and Related Problems** https://drops.dagstuhl.de/opus/volltexte/2020/12895/

## Running
Just launch ```run.py``` script with Python3. At first run you will be asked if you want to download FFTW library. If  you confirm, the script will download, configure and install it in local directory. Then, script will compile and run ```test_correctness.cpp```, which run tests, and ```benchmark.cpp``` to measure times for each solution on different test generators. When it finishes, ```run.py``` will plot graphs for each solution and save it in ```data/``` directory.

Alternatively, you don't have to use FFTW library - when script asks, if you want to install it, all you have to do is to press (n) key. In this case solutions will use own fast Fourier transform implementation, which is a few times slower that this from the library.

There are sample graphs and files with informations about each solution's times in ```data``` directory, generated on computer with Intel(R) Core(TM) i5-7300HQ CPU  @ 2.50GHz, averaged on 10 runs. You can increase default run number by passing ``--runs=`` parameter, for example ```run.py --runs=10```.

## Requirements
- g++ with c++11 support,
- python3 with matplotlib and pandas libraries (you can run ```pip install -r requirements.txt```)

Additionaly, ```run.py``` script can install fftw library only on \*nix systems, for other systems, like Windows, you have to [do it manually](http://www.fftw.org/fftw3_doc/Installation-on-non_002dUnix-systems.html#Installation-on-non_002dUnix-systems) (when configuring, you have to pass --enable-float flag, see ```fftw-install.sh``` script).
