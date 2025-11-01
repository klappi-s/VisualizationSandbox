# Catalyst 2.0 Mini Apps (MPICH)

This repo contains a minimal multi-rank in situ example using ParaView Catalyst 2.0. It builds small executables for sources under `src/mini_apps/`. The primary example is `uniform_mini`, a 3D uniform mesh simulation that updates a single integer cell field each step and sleeps 1s per step. It wires into a tiny Catalyst Python pipeline that exposes a `PVTrivialProducer` for live visualization.

## Requirements
- MPI toolchain (to match the ParaView client build)
- ParaView 5.12 (MPI build) – path set via `PV_PREFIX`
- Catalyst 2.0 SDK built with MPI – path set via `CATALYST_HINT_PATH`

i use MPICH since official binaries are built with this.

## Configure and build

```bash
./cmake.sh
./compile.sh
```

`cmake.sh` enforces MPICH compilers (`/opt/mpich/bin/mpicc/mpicxx`) and uses ParaView's `mpiexec` from `${PV_PREFIX}/lib/mpiexec` to avoid MPI mismatches.

## Run (live visualization)

```bash
# Optional: override paths
export PV_PREFIX=/<PATH TO>/paraview_versions/ParaView-5.12.0-MPI-Linux-Python3.10-x86_64
export CATALYST_IMPLEMENTATION_PATHS="${PV_PREFIX}/lib/catalyst"
export CATALYST_IMPLEMENTATION_NAME=paraview

# Launch the mini app (2 ranks by default)
./run_uniform.sh
```

Then, in ParaView (built with MPICH), use "Catalyst Live" to connect to the running simulation; a source named `uniform` will appear and update every second.

## Notes
- The pipeline script is at `src/mini_apps/pipeline_trivial.py`. You can override it by setting `CATALYST_PIPELINE_PATH` before running.
- `compile.sh` prints a quick linkage check. Ensure only single MPI libraries are present.
