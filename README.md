# PC Lights
[![Build Status](https://travis-ci.org/nnarain/pclights.svg?branch=develop)](https://travis-ci.org/nnarain/pclights)
[![Build status](https://ci.appveyor.com/api/projects/status/7cebq1phenlqcya7/branch/develop?svg=true)](https://ci.appveyor.com/project/nnarain/pclights/branch/master)

Stream animations to a Atmega328p microcontroller to drive a WS2812 led strip in my PC.

Hardware
--------

Build
-----

**PC Lights command line program**

* Install Boost C++
* Set appropriate Boost environment variables like `BOOST_ROOT`

Also be sure to set the correct library type using the FindBoost module's variables `Boost_USE_STATIC_LIBS` and `BOOST_USE_MULTITHREADED`

For example:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.sh}
cd /path/to/project
mkdir build && cd build
cmake .. -DBoost_USE_STATIC_LIBS=ON -DBoost_USE_MULTITHREADED=ON
make
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

**PC Lights firmware**

Builds with PlatformIO
