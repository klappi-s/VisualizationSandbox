#!/usr/bin/env bash
set -euo pipefail

# Adjust ParaView install prefix for your machine
PV_PREFIX=/<PATH TO>/paraview_versions/ParaView-5.12.0-MPI-Linux-Python3.10-x86_64

# Catalyst implementation (ParaView)
export CATALYST_IMPLEMENTATION_PATHS="${PV_PREFIX}/lib/catalyst"
export CATALYST_IMPLEMENTATION_NAME="paraview"

# Use ParaView's mpiexec to avoid MPI mismatches
MPIEXEC="${PV_PREFIX}/lib/mpiexec"

# Ranks (override with: NP=4 ./run_uniform.sh)
: "${NP:=1}"

# Build dir and executable
BUILD_DIR="build"
EXE="${BUILD_DIR}/uniform_mini"

if [ ! -x "${EXE}" ]; then
  echo "Executable ${EXE} not found. Run ./compile.sh first." >&2
  exit 1
fi

# Optional: point to our pipeline (can be overridden via CATALYST_PIPELINE_PATH)
: "${CATALYST_PIPELINE_PATH:=$(pwd)/src/mini_apps/pipeline_trivial.py}"
export CATALYST_PIPELINE_PATH

echo "Using PV at: ${PV_PREFIX}"
echo "Using pipeline: ${CATALYST_PIPELINE_PATH}"
echo "Launching ${NP} ranks..."

"${MPIEXEC}" -np "${NP}" "${EXE}"
