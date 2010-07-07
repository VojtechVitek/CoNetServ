CoNetServ (Complex Network Services)
====================================
**Extension that integrates network tools into the browser.**


Quick links
-----------

* `Wiki pages <http://wiki.github.com/V-Teq/CoNetServ>`_
* `Product overview <http://www.fres-solutions.com/CoNetServ/>`_ and `screenshots <http://wiki.github.com/V-Teq/CoNetServ/screenshots>`_
* `Report bug <http://github.com/V-Teq/CoNetServ/issues/labels/Bug>`_ or `send us idea <http://github.com/V-Teq/CoNetServ/issues/labels/Feature>`_ (alternativelly `conetserv@fres-solutions.com <mailto:conetserv@fres-solutions.com>`_)
* `Contribute <http://pledgie.com/campaigns/10989>`_ (suggested donation: $3.00)
* Current `TODO list <http://github.com/V-Teq/CoNetServ/blob/master/TODO.txt>`_ and `future plans <http://github.com/V-Teq/CoNetServ/blob/master/TODO.txt>`_
* Current `CHANGELOG <http://github.com/V-Teq/CoNetServ/blob/master/CHANGELOG.txt>`_
* List of `AUTHORS <http://github.com/V-Teq/CoNetServ/blob/master/AUTHORS.txt>`_

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
1. Run CMake (download `CMake <http://www.cmake.org/cmake/resources/software.html>`_)
2. Set source code path
3. Set build path (eg. source code path + /build/Windows)
4. Select build type

   a) Debug mode (development)

      Default

   b) Release mode (exports optimized shared library without debug messages)

      Set CMAKE_BUILDTYPE variable to "Release" value

5. Set XULRunnerSDK_INCLUDE_DIR (download `XULRunner SDK <https://developer.mozilla.org/en/Gecko_SDK#Downloading>`_)
6. Click to Configure button
7. Select generator, eg. Visual Studio 2008 (64bit)
8. Click to Configure button again
9. Click to Generate button
10. Open generated project file and you are ready to develop
