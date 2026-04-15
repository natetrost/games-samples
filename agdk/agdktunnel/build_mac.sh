#!/bin/bash
# Copyright 2026 Google LLC
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     https://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

set -e

# Directory containing this script
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

BUILD_DIR="$DIR/build_mac"
mkdir -p "$BUILD_DIR"

# Run CMake
cd "$BUILD_DIR"
cmake "$DIR/app/src/main/cpp"

# Build
cmake --build .

# Copy MoltenVK dylib
cp "$DIR/../third_party/MoltenVK/dynamic/dylib/macOS/libMoltenVK.dylib" "$BUILD_DIR/"

# Copy assets
mkdir -p "$BUILD_DIR/agdktunnel.app/Contents/Resources/assets/textures"
cp "$DIR/install_time_assets/src/main/assets/textures/"* "$BUILD_DIR/agdktunnel.app/Contents/Resources/assets/textures/"
cp "$DIR/on_demand_assets/src/main/assets/textures/"* "$BUILD_DIR/agdktunnel.app/Contents/Resources/assets/textures/"

# Compile shaders
SHADERS_SRC="$DIR/app/src/main/shaders"
SHADERS_DST="$BUILD_DIR/agdktunnel.app/Contents/Resources/assets/shaders"
mkdir -p "$SHADERS_DST"
glslc "$SHADERS_SRC/our.vert" -o "$SHADERS_DST/our.vert.spv"
glslc "$SHADERS_SRC/our.frag" -o "$SHADERS_DST/our.frag.spv"
glslc "$SHADERS_SRC/trivial.vert" -o "$SHADERS_DST/trivial.vert.spv"
glslc "$SHADERS_SRC/trivial.frag" -o "$SHADERS_DST/trivial.frag.spv"

echo "Build completed successfully in $BUILD_DIR"
