# PC Lights
[![Build Status](https://travis-ci.org/nnarain/pclights.svg?branch=develop)](https://travis-ci.org/nnarain/pclights)
[![Build status](https://ci.appveyor.com/api/projects/status/7cebq1phenlqcya7/branch/develop?svg=true)](https://ci.appveyor.com/project/nnarain/pclights/branch/master)

Stream animations to a Atmega328p microcontroller to drive a WS2812 led strip in my PC.

Hardware
--------

Build
-----

* Install Boost C++
* Install avr toolchain
* Set appropriate Boost environment variables like `BOOST_ROOT`

Also be sure to set the correct library type using the FindBoost module's variables `Boost_USE_STATIC_LIBS` and `BOOST_USE_MULTITHREADED`

For example:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.sh}
cd /path/to/project
mkdir build && cd build
cmake .. -DBoost_USE_STATIC_LIBS=ON -DBoost_USE_MULTITHREADED=ON
make
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

**Linux**

Then run cmake and make

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.sh}
cd /path/to/project
mkdir build && cd build
cmake ..
make
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

**Windows**

The cmake External Project build for the firmware doesn't work well when generating Visual Studio project files (work with Unix Makefiles generator).

For this reason building this project on Windows has to options.

* Use "Unix Makefiles" generator

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.sh}
cd /path/to/project
mkdir build && cd build
cmake -G "Unix Makefiles" ..
make
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Must have an appropriate GNU compiler installed.

* Use "Visual Studio <version>" generator

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.sh}
cd /path/to/project
mkdir build && cd build
cmake -G "Visual Studio 14 2015" ..
cd ../firmware
mkdir build && cd build
cmake -G "Unix Makefiles" ..
make
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
