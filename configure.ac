# require at least autoconf 2.61
AC_PREREQ(2.61)

# Process this file with autoconf to produce a configure script.
AC_INIT([fastsave], [0.0.1], [eduardo.bull@gmail.com])

## Set R_HOME, respecting an environment variable if set
: ${R_HOME=$(R RHOME)}
if test -z "${R_HOME}"; then
    AC_MSG_ERROR([Could not determine R_HOME.])
fi

# We are using C++
AC_LANG(C++)
AC_REQUIRE_CPP

## Check the C++ compiler using the CXX value set
AC_PROG_CXX

AC_CHECK_PROG(CMAKE_CHECK, cmake, yes)
if test x"$CMAKE_CHECK" != x"yes" ; then
    AC_MSG_ERROR([Could not find cmake.])
fi


## Get R compilers and flags
R_CC=$("${R_HOME}/bin/R" CMD config CC)
R_CFLAGS=$("${R_HOME}/bin/R" CMD config CFLAGS)
R_CXX=$("${R_HOME}/bin/R" CMD config CXX)
R_CXXFLAGS=$("${R_HOME}/bin/R" CMD config CXXFLAGS)
R_CPPFLAGS=`"${R_HOME}/bin/R" CMD config CPPFLAGS`

# Set default compilers and flags
export CXX=${R_CXX}
export CXXFLAGS="${R_CXXFLAGS} -fPIC"
export CC=${R_CC}
export CFLAGS="${R_CFLAGS} -fPIC"
export CPPFLAGS=${R_CPPFLAGS}

AC_SUBST([PKG_CFLAGS],["${PKG_CFLAGS} ${CFLAGS}"])
AC_SUBST([PKG_CXXFLAGS],["${PKG_CXXFLAGS} ${CXXFLAGS}"])

root_dir="$(pwd)"
third_party_dir="${root_dir}/src/third_party"


## --------------------------------------------
## ZSTD
AC_MSG_NOTICE([Building zstd library])

zstd_version="1.4.4"

zstd_src_dir="${third_party_dir}/zstd-${zstd_version}"
zstd_build_dir=$(mktemp -d -t zstd_build_XXXXXX)
# zstd_build_dir="${root_dir}/build"

ZSTD_INCL="-I'${zstd_src_dir}/lib'"
ZSTD_LIBS="${zstd_build_dir}/lib/libzstd.a"

AC_SUBST([PKG_CFLAGS],["${PKG_CFLAGS} ${ZSTD_INCL}"])
AC_SUBST([PKG_CXXFLAGS],["${PKG_CXXFLAGS} ${ZSTD_INCL}"])
AC_SUBST([PKG_LIBS],["${PKG_LIBS} ${ZSTD_LIBS}"])

AC_SUBST([zstd_src_dir],["${zstd_src_dir}"])
AC_SUBST([zstd_build_dir],["${zstd_build_dir}"])

AC_MSG_NOTICE([Finished building zstd library])

## --------------------------------------------

AC_CONFIG_FILES([src/Makevars])
AC_OUTPUT
