#!/usr/bin/env bash
set -euo pipefail

# Enforce MPICH toolchain to match ParaView's build (avoid OpenMPI mismatch)
: "${MPICH_PREFIX:=/opt/mpich}"
MPICC="${MPICH_PREFIX}/bin/mpicc"
MPICXX="${MPICH_PREFIX}/bin/mpicxx"

# Use mpiexec bundled with the ParaView MPICH build for runtime
: "${PV_PREFIX:=/<PATH_TO_Paraview>/ParaView-5.12.0-MPI-Linux-Python3.10-x86_64}"
MPIEXEC_EXECUTABLE="${PV_PREFIX}/lib/mpiexec"

# Catalyst CMake hint (MPICH-built Catalyst preferred)
: "${CATALYST_HINT_PATH:=/home/klappi/AddApp/catalyst/install-mpich/lib/cmake/catalyst-2.0}"

# Build directory
BUILD_DIR="build"
rm -rf "${BUILD_DIR}"
mkdir -p "${BUILD_DIR}"

pushd "${BUILD_DIR}" >/dev/null

cmake \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_CXX_STANDARD=17 \
  -DCMAKE_FIND_PACKAGE_PREFER_CONFIG=ON \
  -DMPI_C_COMPILER="${MPICC}" \
  -DMPI_CXX_COMPILER="${MPICXX}" \
  -DMPIEXEC_EXECUTABLE="${MPIEXEC_EXECUTABLE}" \
  -DCATALYST_HINT_PATH="${CATALYST_HINT_PATH}" \
  ..

popd >/dev/null

echo "\nConfigured. Next: ./compile.sh"
