// Explicit-coordinate 3D mesh mini app using Catalyst 2.0 (multi-rank ready)
// Mirrors uniform_mini.cpp but builds an explicit coordset + unstructured hex topology.

#include <catalyst.hpp>

#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include <mpi.h>
#ifdef MPI_VERSION
#  define MINI_HAVE_MPI 1
#else
#  define MINI_HAVE_MPI 0
#endif

static std::string get_env_or(const char* key, const std::string& defval)
{
  const char* v = std::getenv(key);
  return v ? std::string(v) : defval;
}

static inline int64_t vtx_index(int i, int j, int k, int nx, int ny)
{
  return static_cast<int64_t>(i) + static_cast<int64_t>(j) * nx + static_cast<int64_t>(k) * (static_cast<int64_t>(nx) * ny);
}





static void build_explicit3d_hex_mesh(conduit_cpp::Node data,
                                      int nx, int ny, int nz,
                                      std::vector<double>& xs,
                                      std::vector<double>& ys,
                                      std::vector<double>& zs,
                                      std::vector<int64_t>& conn,
                                      const int32_t* vals_ptr, int64_t ncell)
{
  // Points (explicit) and topology pointers
  const int64_t np = static_cast<int64_t>(nx) * ny * nz;
  data["coordsets/coords/type"].set("explicit");
  data["coordsets/coords/values/x"].set_float64_ptr(xs.data(), np);
  data["coordsets/coords/values/y"].set_float64_ptr(ys.data(), np);
  data["coordsets/coords/values/z"].set_float64_ptr(zs.data(), np);

  conduit_cpp::Node topo = data["topologies/topo"];
  topo["type"].set("unstructured");
  topo["coordset"].set("coords");
  // Conduit blueprint expects singular shape names: "hex"
  topo["elements/shape"].set("hex");
  topo["elements/connectivity"].set_int64_ptr(conn.data(), static_cast<int64_t>(conn.size()));

  // Field on elements
  conduit_cpp::Node fld = data["fields/f/values"];
  fld.set_int32_ptr(const_cast<int32_t*>(vals_ptr), ncell);
  data["fields/f/association"].set("element");
  data["fields/f/topology"].set("topo");
}














int main(int argc, char** argv)
{


    /* MPI SETUP */
  int rank = 0, size = 1;
#if MINI_HAVE_MPI
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
#endif

  conduit_cpp::Node init;
#if MINI_HAVE_MPI
  const int fcomm = MPI_Comm_c2f(MPI_COMM_WORLD);
  const int64_t fcomm64 = static_cast<int64_t>(fcomm);
  init["catalyst/mpi_comm"].set(fcomm64);
#endif


    /* CATALYST INITIALIZATION */
  const std::string default_pipeline = "src/mini_apps/pipeline_trivial.py";
  const std::string pipeline = get_env_or("CATALYST_PIPELINE_PATH", default_pipeline);
  init["catalyst/scripts/script/filename"].set(pipeline);
  init["catalyst/scripts/script/args"].append().set_string("--channel_names");
  init["catalyst/scripts/script/args"].append().set_string("explicit");
  init["catalyst/scripts/script/args"].append().set_string("--VTKextracts");
  init["catalyst/scripts/script/args"].append().set_string("ON");

  catalyst_status ierr = catalyst_initialize(conduit_cpp::c_node(&init));
  if (ierr != catalyst_status_ok) {
    std::cerr << "catalyst_initialize failed with code " << static_cast<int>(ierr) << "\n";
#if MINI_HAVE_MPI
    MPI_Abort(MPI_COMM_WORLD, 2);
#else
    return 2;
#endif
  }

  // Simulation parameters: 4 elements per dimension globally
  const int steps = 20;
  const int pts_total = 5;
  const int nx = (size == 2 ? 3 : pts_total);
  const int ny = pts_total;
  const int nz = pts_total;
  const double dx = 1.0, dy = 1.0, dz = 1.0;
  const double ox0 = 0.0, oy0 = 0.0, oz0 = 0.0;

  const int ex = nx - 1, ey = ny - 1, ez = nz - 1;
  const int64_t ncell = static_cast<int64_t>(ex) * ey * ez;
  std::vector<int32_t> vals(static_cast<size_t>(ncell));

  // Allocate coordinates and connectivity once; coordinates are static per-rank
  const int64_t np = static_cast<int64_t>(nx) * ny * nz;
  std::vector<double> xs(static_cast<size_t>(np));
  std::vector<double> ys(static_cast<size_t>(np));
  std::vector<double> zs(static_cast<size_t>(np));
  std::vector<int64_t> conn(static_cast<size_t>(ncell) * 8);



  // Fill coordinates once (local domain)
  const double ox = ox0 + rank * (nx - 1) * dx;
  for (int k = 0; k < nz; ++k)
    for (int j = 0; j < ny; ++j)
      for (int i = 0; i < nx; ++i) {
        const int64_t id = vtx_index(i, j, k, nx, ny);
        xs[static_cast<size_t>(id)] = ox + i * dx;
        ys[static_cast<size_t>(id)] = oy0 + j * dy;
        zs[static_cast<size_t>(id)] = oz0 + k * dz;
      }
  {
    // Fill connectivity once (local domain)
    size_t c = 0;
    for (int k = 0; k < ez; ++k)
      for (int j = 0; j < ey; ++j)
        for (int i = 0; i < ex; ++i) {
          const int64_t v0 = vtx_index(i,   j,   k,   nx, ny);
          const int64_t v1 = vtx_index(i+1, j,   k,   nx, ny);
          const int64_t v2 = vtx_index(i+1, j+1, k,   nx, ny);
          const int64_t v3 = vtx_index(i,   j+1, k,   nx, ny);
          const int64_t v4 = vtx_index(i,   j,   k+1, nx, ny);
          const int64_t v5 = vtx_index(i+1, j,   k+1, nx, ny);
          const int64_t v6 = vtx_index(i+1, j+1, k+1, nx, ny);
          const int64_t v7 = vtx_index(i,   j+1, k+1, nx, ny);
          conn[c++] = v0; conn[c++] = v1; conn[c++] = v2; conn[c++] = v3;
          conn[c++] = v4; conn[c++] = v5; conn[c++] = v6; conn[c++] = v7;
        }
  }


  /* "SIMULATION" */
  for (int step = 0; step < steps; ++step) {
    conduit_cpp::Node exec;
    exec["catalyst/state/cycle"].set(step);
    exec["catalyst/state/time"].set(static_cast<double>(step));
    exec["catalyst/state/domain_id"].set(rank);

    conduit_cpp::Node ch = exec["catalyst/channels/explicit"];
    ch["type"].set("mesh");

    // Update field values
    if ((step % 2) == 0) {
      for (int64_t i = 0; i < ncell; ++i) vals[static_cast<size_t>(i)] = static_cast<int32_t>(i + step);
    } else {
      for (int64_t i = 0; i < ncell; ++i) vals[static_cast<size_t>(i)] = static_cast<int32_t>((ncell - 1 - i) + step);
    }

  build_explicit3d_hex_mesh(ch["data"], nx, ny, nz,
                xs, ys, zs, conn, vals.data(), ncell);

    /* CATALYST EXECUTE */
    ierr = catalyst_execute(conduit_cpp::c_node(&exec));
    if (ierr != catalyst_status_ok) {
      std::cerr << "catalyst_execute failed (step=" << step << ") code " << static_cast<int>(ierr) << "\n";
      break;
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));
  }

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
