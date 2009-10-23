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

BINUTILS_VERSION="2.19"
GCC_VERSION="4.4.1"
NEWLIBVERSION="1.17.0"

CROSS_PREFIX=/home/eugen/work/simics/rtems/compilers/sparc64-newlib-elf-${GCC_VERSION}.gcc

if [ -z "${CROSS_PREFIX}" ] ; then
    CROSS_PREFIX="/usr/local"
fi


BINUTILS="binutils-${BINUTILS_VERSION}.tar.gz"
GCC_CORE="gcc-core-${GCC_VERSION}.tar.bz2"
GCC_OBJC="gcc-objc-${GCC_VERSION}.tar.bz2"
GCC_CPP="gcc-g++-${GCC_VERSION}.tar.bz2"
NEWLIB="newlib-${NEWLIBVERSION}.tar.gz"

BINUTILS_SOURCE="ftp://ftp.gnu.org/gnu/binutils/"
GCC_SOURCE="ftp://ftp.gnu.org/gnu/gcc/gcc-${GCC_VERSION}/"
NEWLIB_SOURCE="ftp://sources.redhat.com/pub/newlib/"

PLATFORM="sparc64"
WORKDIR=`pwd`
TARGET="${PLATFORM}-elf"
#TARGET="${PLATFORM}-linux"
PREFIX="${CROSS_PREFIX}/${PLATFORM}"
#PROGRAMPREFIX="sparc64-rtems4.10"
BINUTILSDIR="${WORKDIR}/binutils-${BINUTILS_VERSION}"
GCCDIR="${WORKDIR}/gcc-${GCC_VERSION}"
OBJDIR="${WORKDIR}/gcc-obj"

rm ${BINUTILSDIR} -rf 
rm newlib-${NEWLIBVERSION} -rf 
rm ${GCCDIR} -rf
rm gcc-obj -rf

echo ">>> Downloading tarballs"

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

echo ">>> Unpacking tarballs"
tar -xvzf "${BINUTILS}"
check_error $? "Error unpacking binutils."
tar -xvjf "${GCC_CORE}"
check_error $? "Error unpacking GCC Core."
tar -xvjf "${GCC_OBJC}"
check_error $? "Error unpacking GCC Objective C."
tar -xvjf "${GCC_CPP}"
check_error $? "Error unpacking GCC C++."
tar -xvzf "${NEWLIB}"
check_error $? "Error unpacking newlib."


echo ">>> Compiling and installing binutils"
cd "${BINUTILSDIR}"
check_error $? "Change directory failed."
#./configure "--target=${TARGET}" "--prefix=${PREFIX}" "--program-prefix=${PROGRAMPREFIX}-"
./configure "--target=${TARGET}" "--prefix=${PREFIX}" "--disable-nls"
check_error $? "Error configuring binutils."
make all install
check_error $? "Error compiling/installing binutils."

echo ">>> Compiling and installing GCC"
cd ${GCCDIR}
ln -s ../newlib-${NEWLIBVERSION}/newlib .
cd "${OBJDIR}"
check_error $? "Change directory failed."
#"${GCCDIR}/configure" "--target=${TARGET}" "--prefix=${PREFIX}" --with-gnu-as --with-gnu-ld --with-newlib --disable-nls --disable-threads --enable-languages=c,c++ --disable-multilib --disable-libgcj --without-headers --disable-shared 
"${GCCDIR}/configure" "--target=${TARGET}" "--prefix=${PREFIX}" --with-gnu-as --with-gnu-ld --with-newlib --enable-threads --enable-languages=c --disable-libgcj --without-headers --disable-shared
check_error $? "Error configuring GCC."
PATH="${PATH}:${PREFIX}/bin" 
#make all-gcc install-gcc
make all
make install
check_error $? "Error compiling/installing GCC."

echo
echo ">>> Cross-compiler for ${TARGET} installed."
