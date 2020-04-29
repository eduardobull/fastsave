# require at least autoconf 2.61
AC_PREREQ(2.61)

# Process this file with autoconf to produce a configure script.
AC_INIT([genser], [0.0.1], [eduardo.bull@gmail.com])

## Set R_HOME, respecting an environment variable if set
: ${R_HOME=$(R RHOME)}
if test -z "${R_HOME}"; then
    AC_MSG_ERROR([Could not determine R_HOME.])
fi

## Get R compilers and flags
R_CC=$("${R_HOME}/bin/R" CMD config CC)
R_CFLAGS=$("${R_HOME}/bin/R" CMD config CFLAGS)
R_CXX=$("${R_HOME}/bin/R" CMD config CXX)
R_CXXFLAGS=$("${R_HOME}/bin/R" CMD config CXXFLAGS)
R_CPPFLAGS=`"${R_HOME}/bin/R" CMD config CPPFLAGS`

# Set default compilers and flags
export CXX=${R_CXX}
export CXXFLAGS=${R_CXXFLAGS}
export CC=${R_CC}
export CFLAGS=${R_CFLAGS}
export CPPFLAGS=${R_CPPFLAGS}

# We are using C++
AC_LANG(C++)
AC_REQUIRE_CPP

## Check the C++ compiler using the CXX value set
AC_PROG_CXX

AC_CHECK_PROG(CMAKE_CHECK, cmake, yes)
if test x"$CMAKE_CHECK" != x"yes" ; then
    AC_MSG_ERROR([Could not find cmake.])
fi

AC_SUBST([PKG_CFLAGS],["${PKG_CFLAGS} ${R_CFLAGS}"])
AC_SUBST([PKG_CXXFLAGS],["${PKG_CXXFLAGS} ${R_CXXFLAGS}"])

root_dir="$(pwd)"
third_party_dir="${root_dir}/src/third_party"


## --------------------------------------------
## LZ4
lz4_version="1.9.2"

lz4_src_dir="${third_party_dir}/lz4"
lz4_build_dir="${root_dir}/build/lz4"
# lz4_build_dir=$(mktemp -d -t lz4_build_XXXXXX)

AC_MSG_NOTICE([Building lz4 library])

$(cd ${lz4_src_dir} &&\
	find . -type d -exec mkdir -p -- ${lz4_build_dir}/{} \; &&\
	find . -type f \
		\( -regex '.*\.c\(pp\)?' -o \
		   -regex '.*\.h\(pp\)?' -o \
		   -name 'Makefile*' \) \
		-exec cp --parents {} ${lz4_build_dir} \; &&\
	cd ${lz4_build_dir}/lib &&\
	make liblz4.a > /dev/null 2>&1)

AC_MSG_NOTICE([Finished building lz4 library])

LZ4_INCL="-I${lz4_build_dir}/include"
LZ4_LIBS="${lz4_build_dir}/lib/liblz4.a"

AC_SUBST([PKG_CFLAGS],["${PKG_CFLAGS} ${LZ4_INCL}"])
AC_SUBST([PKG_CXXFLAGS],["${PKG_CXXFLAGS} ${LZ4_INCL}"])
AC_SUBST([PKG_LIBS],["${PKG_LIBS} ${LZ4_LIBS}"])


## --------------------------------------------
## ZSTD
zstd_version="1.4.4"

zstd_src_dir="${third_party_dir}/zstd"
zstd_build_dir="${root_dir}/build/zstd"
# zstd_build_dir=$(mktemp -d -t zstd_build_XXXXXX)

AC_MSG_NOTICE([Building zstd library])

$(mkdir -p ${zstd_build_dir} &&\
	cd ${zstd_build_dir} &&\
	cmake "${zstd_src_dir}/build/cmake/" \
		-DCMAKE_CXX_FLAGS="${R_CXXFLAGS}" \
		-DCMAKE_C_FLAGS="${R_CFLAGS}" \
		-DCMAKE_POSITION_INDEPENDENT_CODE=ON \
		-DCMAKE_BUILD_TYPE=Release \
		-DZSTD_BUILD_STATIC=ON \
		-DZSTD_BUILD_SHARED=OFF \
		-DZSTD_BUILD_PROGRAMS=OFF \
		-DZSTD_BUILD_TESTS=OFF > /dev/null 2>&1 &&\
	make > /dev/null 2>&1)

AC_MSG_NOTICE([Finished building zstd library])

ZSTD_INCL="-I${zstd_build_dir}/include"
ZSTD_LIBS="${zstd_build_dir}/lib/libzstd.a"

AC_SUBST([PKG_CFLAGS],["${PKG_CFLAGS} ${ZSTD_INCL}"])
AC_SUBST([PKG_CXXFLAGS],["${PKG_CXXFLAGS} ${ZSTD_INCL}"])
AC_SUBST([PKG_LIBS],["${PKG_LIBS} ${ZSTD_LIBS}"])


## --------------------------------------------
## SNAPPY
snappy_version="1.1.8"

snappy_src_dir="${third_party_dir}/snappy"
snappy_build_dir="${root_dir}/build/snappy"
# snappy_build_dir=$(mktemp -d -t snappy_build_XXXXXX)

AC_MSG_NOTICE([Building snappy library])

$(mkdir -p ${snappy_build_dir} &&\
	cd ${snappy_build_dir} &&\
	cmake "${snappy_src_dir}" \
		-DCMAKE_CXX_FLAGS="${R_CXXFLAGS}" \
		-DCMAKE_C_FLAGS="${R_CFLAGS}" \
		-DCMAKE_POSITION_INDEPENDENT_CODE=ON \
		-DCMAKE_BUILD_TYPE=Release \
		-DSNAPPY_BUILD_TESTS=OFF > /dev/null 2>&1 &&\
	make > /dev/null 2>&1)

AC_MSG_NOTICE([Finished building snappy library])

SNAPPY_INCL="-I${snappy_build_dir}/include"
SNAPPY_LIBS="${snappy_build_dir}/libsnappy.a"

AC_SUBST([PKG_CFLAGS],["${PKG_CFLAGS} ${SNAPPY_INCL}"])
AC_SUBST([PKG_CXXFLAGS],["${PKG_CXXFLAGS} ${SNAPPY_INCL}"])
AC_SUBST([PKG_LIBS],["${PKG_LIBS} ${SNAPPY_LIBS}"])


## --------------------------------------------

AC_CONFIG_FILES([src/Makevars])
AC_OUTPUT
