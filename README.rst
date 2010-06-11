CoNetServ (Complex Network Services)
====================================
**Extension which integrates network services into the browser.**

.. image:: http://github.com/V-Teq/CoNetServ/raw/master/extension/images/icon128.png

Directory structure
-------------------
==================== =======================================
CMakePlugins/        CMake plugins
extension/           Extension source code
extension/chrome/    Google Chrome (Chromium) specific files
extension/firefox/   Mozilla Firefox specific files
extension/opera/     Opera specific files
doc/                 Documentation (Doxygen)
plugin/              NPAPI-based plugin source code
plugin/apple/        Apple (Darwin) specific files
plugin/unix/         Unix-like specific files
plugin/win/          Windows specific files
==================== =======================================

Building on Unix-like systems
-----------------------------
a) Debug mode

::

  $ mkdir -p build/$(uname)/ && cd build/$(uname)
  $ cmake ../../
  $ make

b) Release mode

::

  $ mkdir -p build/$(uname)/ && cd build/$(uname)
  $ cmake -DCMAKE_BUILD_TYPE=Release ../../
  $ make

  NOTE: (building 32bit plugin on 64bit system)
  $ cmake -DCMAKE_C_FLAGS=-m32 -DARCH=x86 -DCMAKE_BUILD_TYPE=Release ../../

Building on Windows
-------------------
1. Run CMake (download `CMake`_)
2. Set source code path
3. Set build path (eg. source code path + /build/Windows)
4. Select build type

   a) Debug mode (development)

      Default

   b) Release mode (exports optimized shared library without debug messages)

      Set CMAKE_BUILDTYPE variable to "Release" value

5. Set XULRunnerSDK_INCLUDE_DIR (download `XULRunner SDK`_)
6. Click Configure
7. Select generator (Visual Studio, CodeBlocks, Eclipse CDT, Borland, MinGW etc.)
8. Click Generate
9. Open generated project file and you are ready to develop

.. _CMake: http://www.cmake.org/cmake/resources/software.html
.. _XULRunner SDK: https://developer.mozilla.org/en/Gecko_SDK#Downloading
