#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.60)
AC_INIT(services, 0.1, rydencillo@gmail.com)
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_CONFIG_SRCDIR([Nucleo/Nucleo.cpp])
AC_CONFIG_HEADER([config.h])
AC_CONFIG_FILES([Makefile Nucleo/Makefile])

# Checks for programs.
AC_PROG_CC
AC_PROG_CXX

# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T

# Checks for library functions.

cd Nucleo/google/sparsehash
./configure
cd ../../..

AC_OUTPUT

