cmake_minimum_required(VERSION 3.18)
project(catalyst_mini_apps LANGUAGES CXX)

# Options
option(ENABLE_MPI "Build with MPI" ON)

# Prefer config packages (for catalyst)
set(CMAKE_FIND_PACKAGE_PREFER_CONFIG ON)

if(ENABLE_MPI)
  find_package(MPI REQUIRED)
endif()

# Catalyst 2.0
# Allow user to pass -DCATALYST_HINT_PATH=... or rely on default search paths
if(DEFINED CATALYST_HINT_PATH AND EXISTS "${CATALYST_HINT_PATH}")
  message(STATUS "Using user-provided Catalyst hint path: ${CATALYST_HINT_PATH}")
  list(APPEND CATALYST_HINTS "${CATALYST_HINT_PATH}")
endif()
find_package(catalyst REQUIRED
  HINTS ${CATALYST_HINTS}
)

message(STATUS "Found catalyst_DIR: ${catalyst_DIR}")

# Common compile features
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Gather all mini-app .cpp files and create executables per file
file(GLOB MINI_SOURCES CONFIGURE_DEPENDS
  ${CMAKE_CURRENT_SOURCE_DIR}/src/mini_apps/*.cpp)

if(NOT MINI_SOURCES)
  message(WARNING "No sources found under src/mini_apps. Nothing to build.")
endif()

foreach(src ${MINI_SOURCES})
  get_filename_component(name ${src} NAME_WE)
  add_executable(${name} ${src})
  target_link_libraries(${name}
    PRIVATE
      catalyst::catalyst
      catalyst::catalyst_headers
      $<$<BOOL:${ENABLE_MPI}>:MPI::MPI_CXX>
  )
  target_include_directories(${name}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
  )
  message(STATUS "Added mini-app executable: ${name}")
endforeach()
