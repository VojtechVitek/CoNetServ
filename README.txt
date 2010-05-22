====================================
CoNetServ - Complex Network Services
====================================
  Extension which integrates
  network tools into the browser.

Directory structure:
--------------------
  plugin/ ...... NPAPI-based plugin source code
  extension/ ... Extension source code & browsers-specific files
  doc/ ......... Documentation (doxygen)

Building NPAPI plugin:
----------------------
* Unix-like systems:
  ------------------
  $ cd plugin
  $ # edit config.mk (set ARCH variable)
  $ make

* Windows:
  --------
  In plugin directory, there are MSVC project files.
  For further information see plugin/READMEWIN.txt.

Steps above shall produce one of these files:
  npconetserv.so ...... GNU/Linux
  npconetserv.dylib ... MacOSX (Darwin)
  npconetserv.dll ..... Windows

Now you can send the final library file to
the packager (someone who makes final extension
packages).
Or in case you want to generate final packages
yourself, you need to rename the library conforming
to the library-naming-convetion (see generating below)
and copy it into build/ directory in CoNetServ
root directory.


Generating extension packages:
------------------------------
You need to build plugin for every supported
operating system and architecture and copy it
into build/ directory (keep library-naming-convention):
  build/npCoNetServ_Linux_x86_64-gcc3.so
  build/npCoNetServ_Linux_x86-gcc3.so
  build/npCoNetServ_Darwin_x86-gcc3.dylib
  build/npCoNetServ_WINNT_x86-MSVC.dll
Afterwards you should be able to simply run
  $ make
in CoNetServ root directory and it shall
generate these final packages itself:
  build/CoNetServ-*.xpi ... Mozilla Firefox add-on
  build/CoNetServ-*.crx ... Google Chrome extension
  build/CoNetServ-*.wgt ... Opera widget

