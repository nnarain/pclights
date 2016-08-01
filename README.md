# PC Lights
[![Build Status](https://travis-ci.org/nnarain/pclights.svg?branch=develop)](https://travis-ci.org/nnarain/pclights)
[![Build status](https://ci.appveyor.com/api/projects/status/7cebq1phenlqcya7/branch/develop?svg=true)](https://ci.appveyor.com/project/nnarain/pclights/branch/master)

Stream animations to a Atmega328p microcontroller to drive a WS2812 led strip in my PC.

Hardware
--------

Build
-----

* Install Boost C++

**Linux**

* Install avr compiler and libraries

Then run cmake and make

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.sh}
cd /path/to/project
mkdir build && cd build
cmake ..
make
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

**Windows**

* Install avr compiler and library for windows

The cmake External Project build for the firmware doesn't work well when generating Visual Studio project files (work with Unix Makefiles generator).

For this reason building this project on Windows has to options.

* Use "Unix Makefiles" generator

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.sh}
cd /path/to/project
mkdir build && cd build
cmake -G "Unix Makefiles" ..
make
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

* Use "Visual Studio <version>" generator

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.sh}
cd /path/to/project
mkdir build && cd build
cmake -G "Visual Studio 14 2015" ..
cd ../firmware
mkdir build && cd build
cmake ..
make
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Usage
-----
