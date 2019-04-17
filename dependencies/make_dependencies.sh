#!/bin/sh

# Requires depot_tools and git: 
#   https://skia.org/user/download
# Build notes:
#   https://skia.org/user/build
# GLFW requires CMake

SKIA_REPO=https://github.com/google/skia
GLFW_REPO=https://github.com/glfw/glfw

# -----------------------------
# Get & Build Skia
# -----------------------------
if [ ! -d skia ]; then
	echo "Cloning Skia."
    git clone $SKIA_REPO
else
    echo "Already have Skia, update it."
    cd skia && git fetch && git merge master
    cd ..
fi

cd skia
python tools/git-sync-deps
bin/gn gen out/Static --args=" \
    is_official_build=true \
    skia_use_angle=false \
    skia_use_dng_sdk=false \
    skia_use_egl=false \
    skia_use_expat=false \
    skia_use_fontconfig=false \
    skia_use_freetype=true \
    skia_use_system_freetype2=false \
    skia_use_icu=false \
    skia_use_libheif=false \
    skia_use_libjpeg_turbo=false \
    skia_use_libpng=true \
    skia_use_system_libpng=false \
    skia_use_libwebp=false \
    skia_use_lua=false \
    skia_use_piex=false \
    skia_use_vulkan=false \
    skia_use_metal=false \
    skia_use_zlib=true \
    skia_use_system_zlib=false \
    skia_enable_ccpr=true \
    skia_enable_gpu=true \
    skia_enable_fontmgr_empty=false \
    skia_enable_spirv_validation=false \
    skia_enable_pdf=false
    "
ninja -C out/Static
cd ..

# -----------------------------
# Get & Build GLFW
# -----------------------------
if [ ! -d glfw ]; then
	echo "Cloning GLFW."
    git clone $GLFW_REPO
else
    echo "Already have GLFW, update it."
    cd glfw && git fetch && git merge master
    cd ..
fi

mkdir glfw_build
cd glfw_build
cmake ../glfw -DBUILD_SHARED_LIBS=OFF
make