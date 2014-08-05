PFA
===

Tek3 End of Year's Project

How to develop:

cmake --help
Look at the Generators section.

cmake . -G"Generator you choose"

For exemple:

cmake . -G"Unix Makefiles"

cmake . -G"Unix Makefiles" -DCMAKE_BUILD_TYPE:STRING=Debug

Also, this might usefull:
-DCMAKE_VERBOSE_MAKEFILE=On
