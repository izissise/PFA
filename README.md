PFA
===

Tek3 End of Year's Project

How to develop:

cmake --help
Look at the Generators section.

cmake . -G"Generator you choose"

For example:

cmake . -G"Unix Makefiles"

cmake . -G"Unix Makefiles" -DCMAKE_BUILD_TYPE:STRING=Debug

Also, this might be useful:
-DCMAKE_VERBOSE_MAKEFILE=On


For the server:

On Windows, you have to get the boost dll or compile it.
To compile it, you have to download the sources, open cmd.exe, go into the boost folder with the command cd.

When you are in the folder,
Type "bootstrap.sh"
Wait
Open the file "boost/config/user.hpp"
Add the following define
	#define BOOST_ALL_DYN_LINK
	#define BOOST_ALL_NO_LIB
Close the file
Type "b2.exe variant=debug,release link=shared runtime-link=shared --with-program_options --with-system"
Wait again
Now you have copy the boost lib on the server's folder, the librairies are in stages/lib