#!/bin/bash

# Cross-Compiler Toolchain for ${PLATFORM}
#  by Martin Decky <martin@decky.cz>
#
#  GPL'ed, copyleft
#


check_error() {
    if [ "$1" -ne "0" ]; then
	echo
	echo "Script failed: $2"
	exit
    fi
}

OPTIONDOWNLOAD=0
OPTIONUNPACK=0
OPTIONPATCH=0

BINUTILS_VERSION="2.20.1"
GCC_VERSION="4.5.1"
NEWLIB_VERSION="1.18.0"

MYTOOLSVERSION="0.2"

CROSS_PREFIX=/home/eugen/work/rtemssparc64/rtems/compilers/rtems11-sparc64-newlib-${NEWLIB_VERSION}-binutils-${BINUTILS_VERSION}-gcc-${GCC_VERSION}_${MYTOOLSVERSION}


if [ -z "${CROSS_PREFIX}" ] ; then
    CROSS_PREFIX="/opt/rtems-4.11"
fi

BINUTILS="binutils-${BINUTILS_VERSION}.tar.gz"
GCC_CORE="gcc-core-${GCC_VERSION}.tar.bz2"
GCC_OBJC="gcc-objc-${GCC_VERSION}.tar.bz2"
GCC_CPP="gcc-g++-${GCC_VERSION}.tar.bz2"
NEWLIB="newlib-${NEWLIB_VERSION}.tar.gz"

BINUTILS_SOURCE="ftp://ftp.gnu.org/gnu/binutils/"
GCC_SOURCE="ftp://ftp.gnu.org/gnu/gcc/gcc-${GCC_VERSION}/"
NEWLIB_SOURCE="ftp://sources.redhat.com/pub/newlib/"

PLATFORM="sparc64"
WORKDIR=`pwd`
TARGET="${PLATFORM}-rtems4.11"
PREFIX="${CROSS_PREFIX}"
PROGRAMPREFIX="sparc-rtems4.11"
BINUTILSDIR="${WORKDIR}/binutils-${BINUTILS_VERSION}"
GCCDIR="${WORKDIR}/gcc-${GCC_VERSION}"
OBJDIR="${WORKDIR}/b-gcc"
BINBUILDDIR="${WORKDIR}/b-binutils"

rm ${OBJDIR} -rf
rm ${BINBUILDDIR} -rf

echo ">>> Downloading tarballs"

if [ ${OPTIONDOWNLOAD} = '0' ]
then
	echo ">>>>>> Skipping download"
else
	if [ ! -f "${BINUTILS}" ]; then
	    wget -c "${BINUTILS_SOURCE}${BINUTILS}"
	    check_error $? "Error downloading binutils."
	fi
	if [ ! -f "${GCC_CORE}" ]; then
	    wget -c "${GCC_SOURCE}${GCC_CORE}"
	    check_error $? "Error downloading GCC Core."
	fi
	if [ ! -f "${GCC_OBJC}" ]; then
	    wget -c "${GCC_SOURCE}${GCC_OBJC}"
	    check_error $? "Error downloading GCC Objective C."
	fi
	if [ ! -f "${GCC_CPP}" ]; then
	    wget -c "${GCC_SOURCE}${GCC_CPP}"
	    check_error $? "Error downloading GCC C++."
	fi

	if [ ! -f "${NEWLIB}" ]; then
	    wget -c "${NEWLIB_SOURCE}${NEWLIB}"
	    check_error $? "Error downloading newlib."
	fi
fi

echo ">>> Creating destionation directory"
if [ ! -d "${PREFIX}" ]; then
    mkdir -p "${PREFIX}" 
    test -d "${PREFIX}"
    check_error $? "Unable to create ${PREFIX}."
fi

echo ">>> Creating GCC work directory"
if [ ! -d "${OBJDIR}" ]; then
    mkdir -p "${OBJDIR}"
    test -d "${OBJDIR}"
    check_error $? "Unable to create ${OBJDIR}."
fi

echo ">>> Creating BINUTILS build directory"
if [ ! -d "${BINBUILDDIR}" ]; then
    mkdir -p "${BINBUILDDIR}"
    test -d "${BINBUILDDIR}"
    check_error $? "Unable to create ${BINBUILDDIR}."
fi

echo ">>> Unpacking tarballs"
if [ "${OPTIONUNPACK}" = "0" ]
then
	echo ">>>>>> Skipping Unpacking tarballs"
else
	rm ${BINUTILSDIR} -rf 
	tar -xvzf "${BINUTILS}"
	check_error $? "Error unpacking binutils."
	rm ${GCCDIR} -rf
	tar -xvjf "${GCC_CORE}"
	check_error $? "Error unpacking GCC Core."
	tar -xvjf "${GCC_OBJC}"
	check_error $? "Error unpacking GCC Objective C."
	tar -xvjf "${GCC_CPP}"
	check_error $? "Error unpacking GCC C++."
	rm newlib-${NEWLIB_VERSION} -rf 
	tar -xvzf "${NEWLIB}"
	check_error $? "Error unpacking newlib."
fi

echo ">>> Applying patches"
if [ "${OPTIONPATCH}" = "0" ]
then
	echo ">>>>>> Skipping Applying patches"
else
	cd ${BINUTILSDIR}
	cat ../rtemspatches/binutils-${BINUTILS_VERSION}.diff | patch -p1
	cd ${GCCDIR}
	cat ../rtemspatches/gcc-core-${GCC_VERSION}.diff | patch -p1
	cd ../newlib-${NEWLIB_VERSION}
	cat ../rtemspatches/newlib-${NEWLIB_VERSION}.diff | patch -p1
fi

echo ">>> Compiling and installing binutils"
cd "${BINBUILDDIR}" 
check_error $? "Change directory failed."
#${BINUTILSDIR}/configure "--target=${TARGET}" "--prefix=${PREFIX}" "--program-prefix=${PROGRAMPREFIX}-"
#${BINUTILSDIR}/configure "--target=${TARGET}" "--prefix=${PREFIX}" "--disable-nls"
${BINUTILSDIR}/configure "--target=${TARGET}" "--prefix=${PREFIX}"
check_error $? "Error configuring binutils."
make all install
check_error $? "Error compiling/installing binutils."

echo ">>> Compiling and installing GCC"
cd ${GCCDIR}
ln -s ../newlib-${NEWLIB_VERSION}/newlib .
ln -s ../mpfr-3.0.0 mpfr
ln -s ../mpc-0.8.2 mpc
ln -s ../gmp-5.0.1 gmp

cd "${OBJDIR}"
check_error $? "Change directory failed."
#"${GCCDIR}/configure" "--target=${TARGET}" "--prefix=${PREFIX}" --with-gnu-as --with-gnu-ld --with-newlib --disable-nls --disable-threads --enable-languages=c,c++ --disable-multilib --disable-libgcj --without-headers --disable-shared 
#"${GCCDIR}/configure" "--target=${TARGET}" "--prefix=${PREFIX}" --with-gnu-as --with-gnu-ld --with-newlib --enable-threads --enable-languages=c --disable-libgcj --without-headers --disable-shared "--program-prefix=${PROGRAMPREFIX}-"
"${GCCDIR}/configure" "--target=${TARGET}" "--prefix=${PREFIX}" --with-newlib --enable-threads --enable-languages=c,c++ 
check_error $? "Error configuring GCC."
PATH="${PATH}:${PREFIX}/bin" 
#make all-gcc install-gcc
make all
make install
check_error $? "Error compiling/installing GCC."

echo
echo ">>> Cross-compiler for ${TARGET} installed."
