#!/usr/bin/env bash
set -euo pipefail

BUILD_DIR="build"
if [ ! -d "${BUILD_DIR}" ]; then
  echo "Build directory not found. Run ./cmake.sh first." >&2
  exit 1
fi

cmake --build "${BUILD_DIR}" -j

echo "\nLinked libs of built executables (showing catalyst/mpi/python if present):"
for exe in $(find "${BUILD_DIR}" -maxdepth 1 -type f -executable); do
  echo "--- $(basename "$exe") ---"
  ldd "$exe" | grep -E "libmpi|libmpicxx|libcatalyst|libpython" || true
  echo
done
