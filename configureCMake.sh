#!/bin/bash

UNAME=$(uname)

############
# OS detection
############
TRIPLET="x"
if [[ "$UNAME" == CYGWIN* || "$UNAME" == MINGW* ]] ; then
    TRIPLET="${TRIPLET}64-windows-static-release"
elif [ "$UNAME" == "Darwin" ] ; then
    # we use custom triplet (x64-osx-supported)
    # this make effect for openssl (via vcpkg)
    TRIPLET="${TRIPLET}64-osx-supported-release"
    # copy custom triplet file
    cp "${TRIPLET}.cmake" "submodule/vcpkg/triplets/${TRIPLET}.cmake"
elif [ "$UNAME" == "Linux" ] ; then
    TRIPLET="${TRIPLET}64-linux-release"
else
    echo "This OS is not supported..."
    exit 1
fi


############
# cmake
############
# shorten path to vcpkg
#   note: windows has shorter path length limit
if [[ "$UNAME" == CYGWIN* || "$UNAME" == MINGW* ]] ; then
    subst X: submodule
fi

# set path to vcpkg
if [[ "$UNAME" == CYGWIN* || "$UNAME" == MINGW* ]] ; then
    VCPKG_DIR="X:/vcpkg"
elif [ "$UNAME" == "Darwin" ] ; then
    VCPKG_DIR="submodule/vcpkg"
elif [ "$UNAME" == "Linux" ] ; then
    VCPKG_DIR="submodule/vcpkg"
fi

# configure
cmake -B build/Release -S . -DCMAKE_TOOLCHAIN_FILE="${VCPKG_DIR}/scripts/buildsystems/vcpkg.cmake" -DVCPKG_TARGET_TRIPLET="${TRIPLET}" -DCMAKE_BUILD_TYPE="Release" -DCMAKE_INSTALL_PREFIX="install" -DINSTALL_GTEST=OFF
cmake -B build/Debug -S . -DCMAKE_TOOLCHAIN_FILE="${VCPKG_DIR}/scripts/buildsystems/vcpkg.cmake" -DVCPKG_TARGET_TRIPLET="${TRIPLET}" -DCMAKE_BUILD_TYPE="Debug" -DCMAKE_INSTALL_PREFIX="install" -DINSTALL_GTEST=OFF

# revert subst command
if [[ "$UNAME" == CYGWIN* || "$UNAME" == MINGW* ]] ; then
    # "/" symbol was comprehended as separator for path in MINGW. Thus, we need to explicitly use "//"
    subst X: //D
fi
