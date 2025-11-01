// Minimal 3D uniform-mesh simulation with Catalyst 2.0 (multi-rank ready)
// - Initializes Catalyst with a Python pipeline (pipeline_trivial.py)
// - Each step updates a 3D uniform mesh channel named "uniform"
// - Field values change order every step; sleeps 1s to mimic a simulation

#include <catalyst.hpp>

#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <string>
#include <thread>

// Enable MPI unconditionally (CMake finds MPI and links include paths)
#include <mpi.h>
#ifdef MPI_VERSION
#  define MINI_HAVE_MPI 1
#else
#  define MINI_HAVE_MPI 0
#endif

// prefer conduit_cpp::Node (handle-like; behaves like ref semantics on subpaths)

static std::string get_env_or(const char* key, const std::string& defval)
{
    const char* v = std::getenv(key);
    return v ? std::string(v) : defval;
}

static void build_uniform3d_mesh(conduit_cpp::Node data,
                                 int nx, int ny, int nz,
                                 double ox, double oy, double oz,
                                 double dx, double dy, double dz,
                                 const int32_t* vals_ptr, int64_t ncell)
{
    // coordset
    data["coordsets/coords/type"].set("uniform");
    data["coordsets/coords/dims/i"].set(nx);
    data["coordsets/coords/dims/j"].set(ny);
    data["coordsets/coords/dims/k"].set(nz);
    data["coordsets/coords/origin/x"].set(ox);
    data["coordsets/coords/origin/y"].set(oy);
    data["coordsets/coords/origin/z"].set(oz);
    data["coordsets/coords/spacing/dx"].set(dx);
    data["coordsets/coords/spacing/dy"].set(dy);
    data["coordsets/coords/spacing/dz"].set(dz);

    // topology
    data["topologies/topo/type"].set("uniform");
    data["topologies/topo/coordset"].set("coords");

    // one element-associated int field (external array provided)
    const int ex = nx - 1;
    const int ey = ny - 1;
    const int ez = nz - 1;

    conduit_cpp::Node fld = data["fields/f/values"];
    fld.set_int32_ptr(const_cast<int32_t*>(vals_ptr), ncell);
    data["fields/f/association"].set("element");
    data["fields/f/topology"].set("topo");
}

int main(int argc, char** argv)
{

    
    // --- MPI init ---
    int rank = 0, size = 1;
#if MINI_HAVE_MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
#endif

    // --- Catalyst initialize ---
    conduit_cpp::Node init;

#if MINI_HAVE_MPI
    // Share MPI communicator with Catalyst (Fortran handle expected)
    const int fcomm = MPI_Comm_c2f(MPI_COMM_WORLD);
    const int64_t fcomm64 = static_cast<int64_t>(fcomm);
    init["catalyst/mpi_comm"].set(fcomm64);
#endif

    // Pipeline script path: from env CATALYST_PIPELINE_PATH or default
    const std::string default_pipeline = "src/mini_apps/pipeline_trivial.py";
    const std::string pipeline = get_env_or("CATALYST_PIPELINE_PATH", default_pipeline);
    init["catalyst/scripts/script/filename"].set(pipeline);
    // pass channel name for our trivial pipeline and turn on VTK extracts
    init["catalyst/scripts/script/args"].append().set_string("--channel_names");
    init["catalyst/scripts/script/args"].append().set_string("uniform");
    init["catalyst/scripts/script/args"].append().set_string("--VTKextract");
    init["catalyst/scripts/script/args"].append().set_string("ON");

        #if defined MINI_HAVE_MPI
        MPI_Barrier(MPI_COMM_WORLD);
        if (rank==0)        init.print();
        MPI_Barrier(MPI_COMM_WORLD);
        if (rank==1)        init.print();
        MPI_Barrier(MPI_COMM_WORLD);
        #endif

    catalyst_status ierr = catalyst_initialize(conduit_cpp::c_node(&init));
    if (ierr != catalyst_status_ok) {
        std::cerr << "catalyst_initialize failed with code " << static_cast<int>(ierr) << "\n";
#if MINI_HAVE_MPI
        MPI_Abort(MPI_COMM_WORLD, 2);
#else
        return 2;
#endif
    }



    

    // --- Simulation loop ---
    const int steps = 20;
    // Target: 4 elements (5 points) per dimension globally
    const int pts_total = 5;
    // Split X across 2 ranks: each gets 2 elements = 3 points; otherwise use full
    const int nx = (size == 2 ? 3 : pts_total); // points per rank in X
    const int ny = pts_total;                   // points per rank in Y
    const int nz = pts_total;                   // points per rank in Z
    const double dx = 1.0, dy = 1.0, dz = 1.0;
    const double ox0 = 0.0, oy0 = 0.0, oz0 = 0.0;

    const int ex = nx - 1, ey = ny - 1, ez = nz - 1;
    const int64_t ncell = static_cast<int64_t>(ex) * ey * ez;
    std::vector<int32_t> vals(static_cast<size_t>(ncell));

    for (int step = 0; step < steps; ++step) {
        conduit_cpp::Node exec;

        // state
        exec["catalyst/state/cycle"].set(step);
        exec["catalyst/state/time"].set(static_cast<double>(step));
        exec["catalyst/state/domain_id"].set(rank);

        // channel (mesh)
        conduit_cpp::Node ch = exec["catalyst/channels/uniform"];
        ch["type"].set("mesh");
        // ch["state/multiblock"].set(1); // let PV treat ranks as partitions // breaks vtpd extraction

        // Prepare field values (alternate order per step)
        if (step % 2 == 0) {
            for (int64_t i = 0; i < ncell; ++i) vals[static_cast<size_t>(i)] = static_cast<int32_t>(i + step);
        } else {
            for (int64_t i = 0; i < ncell; ++i) vals[static_cast<size_t>(i)] = static_cast<int32_t>((ncell - 1 - i) + step);
        }

    // tile ranks along X so each rank shows a distinct partition
    const double ox = ox0 + rank * (nx - 1) * dx;
        build_uniform3d_mesh(ch["data"], nx, ny, nz, ox, oy0, oz0, dx, dy, dz, vals.data(), ncell);

        if(step==0){
        #if defined MINI_HAVE_MPI
        MPI_Barrier(MPI_COMM_WORLD);
        if (rank==0)        exec.print();
        MPI_Barrier(MPI_COMM_WORLD);
        if (rank==1)        exec.print();
        MPI_Barrier(MPI_COMM_WORLD);
        #endif
        }

        ierr = catalyst_execute(conduit_cpp::c_node(&exec));
        if (ierr != catalyst_status_ok) {
            std::cerr << "catalyst_execute failed (step=" << step << ") code " << static_cast<int>(ierr) << "\n";
            break;
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    // --- Finalize ---
    conduit_cpp::Node fin;
    ierr = catalyst_finalize(conduit_cpp::c_node(&fin));
    if (ierr != catalyst_status_ok) {
        std::cerr << "catalyst_finalize failed with code " << static_cast<int>(ierr) << "\n";
    }

#if MINI_HAVE_MPI
    MPI_Finalize();
#endif
    return 0;
}