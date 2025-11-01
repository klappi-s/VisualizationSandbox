# Catalyst 2.0 Mini Apps

This repo contains minimal multi-rank in situ examples using ParaView Catalyst 2.0. It builds small executables for sources under `src/mini_apps/` and wires them into a tiny Catalyst Python pipeline that exposes `PVTrivialProducer`(s) for live visualization.

Mini apps available:
- `uniform_mini`: 3D uniform mesh; updates a single integer cell field each step (1s sleep per step)
- `explicit_mini`: 3D explicit coordinates + unstructured hex topology; same evolving cell field and timing

## Requirements
- MPI toolchain (to match the ParaView client build)
- ParaView 5.12 (MPI build) – path set via `PV_PREFIX`
- Catalyst 2.0 SDK built with MPI – path set via `CATALYST_HINT_PATH`

i use MPICH since official linux paraview binaries are built with this.

## Configure and build

```bash
# might need to properly set <PATH_TO_Paraview> and <PATH_TO_CATALYST>/ in cmake bash
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

# start paraview catalyst live session
paraview --live 22222

# Launch the uniform mini app (default NP=2; override with NP)
NP=1 ./run_uniform.sh
./run_uniform.sh


# Launch the explicit-coordinates mini app (default NP=2 in the script)
# !!!! Note: set PV_PREFIX in run_explicit.sh or export it before running  !!!!
./run_explicit.sh
```

Then, in ParaView (built with MPICH), use "Catalyst Live" to connect to the running simulation; a source named `uniform` (for `uniform_mini`) or `explicit` (for `explicit_mini`) will appear and update every second.

## Notes
- The pipeline script is at `src/mini_apps/pipeline_trivial.py`. You can override it by setting `CATALYST_PIPELINE_PATH` before running.
- `compile.sh` prints a quick linkage check. Ensure only single MPI libraries are present.
- `run_explicit.sh` ships with a placeholder `PV_PREFIX`. Point it to your ParaView MPI build (same MPICH as used to build Catalyst/this app).

## Problem


Both succeed in vtk file generation but with the uniform mesh, live visualisation in the paraview client only displays data from rank 0
