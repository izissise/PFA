#ifndef __UNUSED_HPP__
# define __UNUSED_HPP__

# if defined(__GNUC__)
#  define UNUSED __attribute__((unused))
# elif defined(_WIN32)
#  define UNUSED __pragma(warning(suppress: 4100 4101))
# elif defined(_WIN64)
#  define UNUSED __pragma(warning(suppress: 4100 4101))
# elif defined(__APPLE__)
#  define UNUSED __attribute__((unused))
# endif

#endif