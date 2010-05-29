CoNetServ (Complex Network Services)
====================================
  Extension which integrates
  network services into the browser.

Directory structure
-------------------
  plugin/ ...... NPAPI-based plugin source code
  extension/ ... Extension source code & browsers-specific files
  doc/ ......... Documentation (doxygen)

Unix-like
------------------------------
  $ mkdir -p build/$(uname)/ && cd build/$(uname)
  a) Debug mode
     $ cmake ../../
  b) Release mode
     $ cmake -D CMAKE_BUILD_TYPE="Release" ../../
  $ make

Windows
-------
  1. Run CMake (download `CMake`_)
  2. Set source code path
  3. Set build path (eg. source code path + /build/Windows)
  4. Build type
     a) Debug mode (development)
        Default (Skip to 5)
     b) Release mode (exports optimized shared library without debug messages)
        Set CMAKE_BUILDTYPE variable to "Release" value
  5. Set XULRunnerSDK_INCLUDE_DIR (download `XULRunner SDK`_)
  6. Click Configure
  7. Select generator (Visual Studio, CodeBlocks, Eclipse CDT, Borland, MinGW etc.)
  8. Click Generate
  9. Open generated project file and you are ready to develope

.. _CMake: http://www.cmake.org/cmake/resources/software.html
.. _XULRunner SDK: https://developer.mozilla.org/en/Gecko_SDK#Downloading