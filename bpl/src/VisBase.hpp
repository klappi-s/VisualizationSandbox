#pragma once
#include "VisBase.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <typeinfo>
#include <functional>
#include <cxxabi.h>

// Utility function to demangle C++ type names for readable output
inline std::string demangle_type_name(const char* mangled_name) {
    int status = 0;
    char* demangled = abi::__cxa_demangle(mangled_name, nullptr, nullptr, &status);
    std::string result = (status == 0) ? std::string(demangled) : std::string(mangled_name);
    free(demangled);
    return result;
}

// Template helper to get container size safely
template<typename Container>
std::size_t get_container_size(const Container& container) noexcept {
    try {
        if (container) {
            return container->size();
        }
        return 0;
    } catch (...) {
        return 0;
    }
}

// Implementation of print_state for VisRegistry (Verbose version)
template<typename T, unsigned Dim>
void VisRegistry<T, Dim>::print_state() const noexcept {
    try {
        std::cout << "\n" << std::string(80, '=') << "\n";
        std::cout << "VisRegistry VERBOSE State Report\n";
        std::cout << std::string(80, '=') << "\n";
        
        // Registry information
        std::cout << "Registry Type: " << demangle_type_name(typeid(*this).name()) << "\n";
        std::cout << "Base Type: " << demangle_type_name(typeid(T).name()) << "\n";
        std::cout << "Maximum Dimension: " << Dim << "\n";
        std::cout << "Registry Memory Address: " << static_cast<const void*>(this) << "\n";
        std::cout << "Registry Size: " << sizeof(*this) << " bytes\n\n";
        
        // DETAILED SUBREGISTRY ANALYSIS
        std::cout << "🔍 DETAILED SUBREGISTRY ANALYSIS:\n";
        std::cout << std::string(80, '-') << "\n";
        
        // Base case analysis: (1,1)
        std::cout << "📍 VisSubRegistry<" << demangle_type_name(typeid(T).name()) << ", 1, 1> (Base Case):\n";
        std::cout << "  - Scalar Field Container (Field<" << demangle_type_name(typeid(T).name()) << ",1>):\n";
        std::cout << "    * Address: " << static_cast<const void*>(VisSubRegistry<T, 1, 1>::sf_c.get()) << "\n";
        std::cout << "    * Container allocated: " << (VisSubRegistry<T, 1, 1>::sf_c ? "YES" : "NO") << "\n";
        std::cout << "    * Size: " << get_container_size(VisSubRegistry<T, 1, 1>::sf_c) << "\n";
        
        std::cout << "  - Vector Field Container (Field<vec<" << demangle_type_name(typeid(T).name()) << ",1>,1>):\n";
        std::cout << "    * Address: " << static_cast<const void*>(VisSubRegistry<T, 1, 1>::vf_c.get()) << "\n";
        std::cout << "    * Container allocated: " << (VisSubRegistry<T, 1, 1>::vf_c ? "YES" : "NO") << "\n";
        std::cout << "    * Size: " << get_container_size(VisSubRegistry<T, 1, 1>::vf_c) << "\n";
        
        std::cout << "  - Particle Container (ParticleBase<" << demangle_type_name(typeid(T).name()) << ",1>):\n";
        std::cout << "    * Address: " << static_cast<const void*>(VisSubRegistry<T, 1, 1>::pb_c.get()) << "\n";
        std::cout << "    * Container allocated: " << (VisSubRegistry<T, 1, 1>::pb_c ? "YES" : "NO") << "\n";
        std::cout << "    * Size: " << get_container_size(VisSubRegistry<T, 1, 1>::pb_c) << "\n\n";

        // Horizontal axis analysis: (1, VDim) where VDim > 1
        for (unsigned vd = 2; vd <= 3; ++vd) {
            std::cout << "📍 VisSubRegistry<" << demangle_type_name(typeid(T).name()) << ", 1, " << vd << "> (Horizontal Axis):\n";
            
            if (vd == 2) {
                std::cout << "  - Vector Field Container (Field<vec<" << demangle_type_name(typeid(T).name()) << ",2>,1>):\n";
                std::cout << "    * Address: " << static_cast<const void*>(VisSubRegistry<T, 1, 2>::vf_c.get()) << "\n";
                std::cout << "    * Container allocated: " << (VisSubRegistry<T, 1, 2>::vf_c ? "YES" : "NO") << "\n";
                std::cout << "    * Size: " << get_container_size(VisSubRegistry<T, 1, 2>::vf_c) << "\n";
            } else if (vd == 3) {
                std::cout << "  - Vector Field Container (Field<vec<" << demangle_type_name(typeid(T).name()) << ",3>,1>):\n";
                std::cout << "    * Address: " << static_cast<const void*>(VisSubRegistry<T, 1, 3>::vf_c.get()) << "\n";
                std::cout << "    * Container allocated: " << (VisSubRegistry<T, 1, 3>::vf_c ? "YES" : "NO") << "\n";
                std::cout << "    * Size: " << get_container_size(VisSubRegistry<T, 1, 3>::vf_c) << "\n";
            }
            std::cout << "\n";
        }

        // Vertical axis analysis: (Dim, 1) where Dim > 1
        for (unsigned d = 2; d <= Dim; ++d) {
            std::cout << "📍 VisSubRegistry<" << demangle_type_name(typeid(T).name()) << ", " << d << ", 1> (Vertical Axis):\n";
            
            if constexpr (Dim >= 2) {
                if (d == 2) {
                    std::cout << "  - Scalar Field Container (Field<" << demangle_type_name(typeid(T).name()) << ",2>):\n";
                    std::cout << "    * Address: " << static_cast<const void*>(VisSubRegistry<T, 2, 1>::sf_c.get()) << "\n";
                    std::cout << "    * Container allocated: " << (VisSubRegistry<T, 2, 1>::sf_c ? "YES" : "NO") << "\n";
                    std::cout << "    * Size: " << get_container_size(VisSubRegistry<T, 2, 1>::sf_c) << "\n";
                    
                    std::cout << "  - Vector Field Container (Field<vec<" << demangle_type_name(typeid(T).name()) << ",1>,2>):\n";
                    std::cout << "    * Address: " << static_cast<const void*>(VisSubRegistry<T, 2, 1>::vf_c.get()) << "\n";
                    std::cout << "    * Container allocated: " << (VisSubRegistry<T, 2, 1>::vf_c ? "YES" : "NO") << "\n";
                    std::cout << "    * Size: " << get_container_size(VisSubRegistry<T, 2, 1>::vf_c) << "\n";
                    
                    std::cout << "  - Particle Container (ParticleBase<" << demangle_type_name(typeid(T).name()) << ",2>):\n";
                    std::cout << "    * Address: " << static_cast<const void*>(VisSubRegistry<T, 2, 1>::pb_c.get()) << "\n";
                    std::cout << "    * Container allocated: " << (VisSubRegistry<T, 2, 1>::pb_c ? "YES" : "NO") << "\n";
                    std::cout << "    * Size: " << get_container_size(VisSubRegistry<T, 2, 1>::pb_c) << "\n";
                }
            }
            
            if constexpr (Dim >= 3) {
                if (d == 3) {
                    std::cout << "  - Scalar Field Container (Field<" << demangle_type_name(typeid(T).name()) << ",3>):\n";
                    std::cout << "    * Address: " << static_cast<const void*>(VisSubRegistry<T, 3, 1>::sf_c.get()) << "\n";
                    std::cout << "    * Container allocated: " << (VisSubRegistry<T, 3, 1>::sf_c ? "YES" : "NO") << "\n";
                    std::cout << "    * Size: " << get_container_size(VisSubRegistry<T, 3, 1>::sf_c) << "\n";
                    
                    std::cout << "  - Vector Field Container (Field<vec<" << demangle_type_name(typeid(T).name()) << ",1>,3>):\n";
                    std::cout << "    * Address: " << static_cast<const void*>(VisSubRegistry<T, 3, 1>::vf_c.get()) << "\n";
                    std::cout << "    * Container allocated: " << (VisSubRegistry<T, 3, 1>::vf_c ? "YES" : "NO") << "\n";
                    std::cout << "    * Size: " << get_container_size(VisSubRegistry<T, 3, 1>::vf_c) << "\n";
                    
                    std::cout << "  - Particle Container (ParticleBase<" << demangle_type_name(typeid(T).name()) << ",3>):\n";
                    std::cout << "    * Address: " << static_cast<const void*>(VisSubRegistry<T, 3, 1>::pb_c.get()) << "\n";
                    std::cout << "    * Container allocated: " << (VisSubRegistry<T, 3, 1>::pb_c ? "YES" : "NO") << "\n";
                    std::cout << "    * Size: " << get_container_size(VisSubRegistry<T, 3, 1>::pb_c) << "\n";
                }
            }
            std::cout << "\n";
        }

        // General case analysis: (Dim, VDim) where both > 1
        for (unsigned d = 2; d <= Dim; ++d) {
            for (unsigned vd = 2; vd <= 3; ++vd) {
                std::cout << "📍 VisSubRegistry<" << demangle_type_name(typeid(T).name()) << ", " << d << ", " << vd << "> (General Case):\n";
                
                if constexpr (Dim >= 2) {
                    if (d == 2 && vd == 2) {
                        std::cout << "  - Vector Field Container (Field<vec<" << demangle_type_name(typeid(T).name()) << ",2>,2>):\n";
                        std::cout << "    * Address: " << static_cast<const void*>(VisSubRegistry<T, 2, 2>::vf_c.get()) << "\n";
                        std::cout << "    * Container allocated: " << (VisSubRegistry<T, 2, 2>::vf_c ? "YES" : "NO") << "\n";
                        std::cout << "    * Size: " << get_container_size(VisSubRegistry<T, 2, 2>::vf_c) << "\n";
                    }
                    if (d == 2 && vd == 3) {
                        std::cout << "  - Vector Field Container (Field<vec<" << demangle_type_name(typeid(T).name()) << ",3>,2>):\n";
                        std::cout << "    * Address: " << static_cast<const void*>(VisSubRegistry<T, 2, 3>::vf_c.get()) << "\n";
                        std::cout << "    * Container allocated: " << (VisSubRegistry<T, 2, 3>::vf_c ? "YES" : "NO") << "\n";
                        std::cout << "    * Size: " << get_container_size(VisSubRegistry<T, 2, 3>::vf_c) << "\n";
                    }
                }
                
                if constexpr (Dim >= 3) {
                    if (d == 3 && vd == 2) {
                        std::cout << "  - Vector Field Container (Field<vec<" << demangle_type_name(typeid(T).name()) << ",2>,3>):\n";
                        std::cout << "    * Address: " << static_cast<const void*>(VisSubRegistry<T, 3, 2>::vf_c.get()) << "\n";
                        std::cout << "    * Container allocated: " << (VisSubRegistry<T, 3, 2>::vf_c ? "YES" : "NO") << "\n";
                        std::cout << "    * Size: " << get_container_size(VisSubRegistry<T, 3, 2>::vf_c) << "\n";
                    }
                    if (d == 3 && vd == 3) {
                        std::cout << "  - Vector Field Container (Field<vec<" << demangle_type_name(typeid(T).name()) << ",3>,3>):\n";
                        std::cout << "    * Address: " << static_cast<const void*>(VisSubRegistry<T, 3, 3>::vf_c.get()) << "\n";
                        std::cout << "    * Container allocated: " << (VisSubRegistry<T, 3, 3>::vf_c ? "YES" : "NO") << "\n";
                        std::cout << "    * Size: " << get_container_size(VisSubRegistry<T, 3, 3>::vf_c) << "\n";
                    }
                }
                std::cout << "\n";
            }
        }

        // INHERITANCE HIERARCHY ANALYSIS
        std::cout << "🏗️  COMPLETE INHERITANCE HIERARCHY:\n";
        std::cout << std::string(80, '-') << "\n";
        
        std::cout << "VisRegistry<" << demangle_type_name(typeid(T).name()) << ", " << Dim << ">\n";
        std::cout << "└── inherits from: VisSubRegistry<" << demangle_type_name(typeid(T).name()) << ", " << Dim << ", 3>\n";
        
        // Build the correct inheritance tree based on actual template specializations
        std::function<void(unsigned, unsigned, std::string, bool)> print_inheritance = 
            [&](unsigned d, unsigned vd, std::string prefix, bool is_last) {
                
            std::string type_str = "VisSubRegistry<" + demangle_type_name(typeid(T).name()) + ", " + std::to_string(d) + ", " + std::to_string(vd) + ">";
            
            if (d == 1 && vd == 1) {
                std::cout << prefix << "└── " << type_str << " (BASE CASE)\n";
                return;
            }
            
            if (d == 1 && vd > 1) {
                // Horizontal inheritance: VisSubRegistry<T, 1, VDim> inherits from VisSubRegistry<T, 1, VDim-1>
                std::cout << prefix << (is_last ? "└── " : "├── ") << type_str << " (Horizontal)\n";
                std::string new_prefix = prefix + (is_last ? "    " : "│   ");
                print_inheritance(1, vd - 1, new_prefix, true);
                
            } else if (d > 1 && vd == 1) {
                // Vertical inheritance: VisSubRegistry<T, Dim, 1> inherits from VisSubRegistry<T, Dim-1, 1>
                std::cout << prefix << (is_last ? "└── " : "├── ") << type_str << " (Vertical)\n";
                std::string new_prefix = prefix + (is_last ? "    " : "│   ");
                print_inheritance(d - 1, 1, new_prefix, true);
                
            } else if (d > 1 && vd > 1) {
                // General case: VisSubRegistry<T, Dim, VDim> inherits from BOTH parents
                std::cout << prefix << (is_last ? "└── " : "├── ") << type_str << " (General)\n";
                std::string new_prefix = prefix + (is_last ? "    " : "│   ");
                
                std::cout << new_prefix << "├── virtual inherits from:\n";
                print_inheritance(d - 1, vd, new_prefix + "│   ", false);
                
                std::cout << new_prefix << "└── virtual inherits from:\n";
                print_inheritance(d, vd - 1, new_prefix + "    ", true);
            }
        };
        
        // Start the recursion from the top-level registry
        print_inheritance(Dim, 3, "    ", true);
        
        std::cout << "\n🎯 INHERITANCE RULES:\n";
        std::cout << "  • Horizontal: VisSubRegistry<T, 1, VDim> → VisSubRegistry<T, 1, VDim-1>\n";
        std::cout << "  • Vertical: VisSubRegistry<T, Dim, 1> → VisSubRegistry<T, Dim-1, 1>\n";
        std::cout << "  • General: VisSubRegistry<T, Dim, VDim> → {VisSubRegistry<T, Dim-1, VDim>, VisSubRegistry<T, Dim, VDim-1>}\n";
        std::cout << "  • Base case: VisSubRegistry<T, 1, 1> (no further inheritance)\n";
        std::cout << "  • Virtual inheritance prevents diamond problem duplication\n";
        std::cout << "\n";

        // SUMMARY STATISTICS (same as before but with more detail)
        std::cout << "📊 SCALAR FIELDS SUMMARY:\n";
        std::cout << std::string(50, '-') << "\n";
        
        std::size_t total_scalar_fields = 0;
        for (unsigned d = 1; d <= Dim; ++d) {
            try {
                std::size_t count = 0;
                if constexpr (Dim >= 1) {
                    if (d == 1) count = get_container_size(VisSubRegistry<T, 1, 1>::sf_c);
                }
                if constexpr (Dim >= 2) {
                    if (d == 2) count = get_container_size(VisSubRegistry<T, 2, 1>::sf_c);
                }
                if constexpr (Dim >= 3) {
                    if (d == 3) count = get_container_size(VisSubRegistry<T, 3, 1>::sf_c);
                }
                
                if (count > 0) {
                    std::cout << "  Dimension " << d << ": " << count << " fields\n";
                    total_scalar_fields += count;
                }
            } catch (...) {
                std::cout << "  Dimension " << d << ": [Error accessing container]\n";
            }
        }
        
        if (total_scalar_fields == 0) {
            std::cout << "  No scalar fields registered.\n";
        }
        
        // Particle Bunches Section - just show counts
        std::cout << "\n🎯 PARTICLE BUNCHES SUMMARY:\n";
        std::cout << std::string(50, '-') << "\n";
        
        std::size_t total_particle_bunches = 0;
        for (unsigned d = 1; d <= Dim; ++d) {
            try {
                std::size_t count = 0;
                if constexpr (Dim >= 1) {
                    if (d == 1) count = get_container_size(VisSubRegistry<T, 1, 1>::pb_c);
                }
                if constexpr (Dim >= 2) {
                    if (d == 2) count = get_container_size(VisSubRegistry<T, 2, 1>::pb_c);
                }
                if constexpr (Dim >= 3) {
                    if (d == 3) count = get_container_size(VisSubRegistry<T, 3, 1>::pb_c);
                }
                
                if (count > 0) {
                    std::cout << "  Dimension " << d << ": " << count << " particle bunches\n";
                    total_particle_bunches += count;
                }
            } catch (...) {
                std::cout << "  Dimension " << d << ": [Error accessing container]\n";
            }
        }
        
        if (total_particle_bunches == 0) {
            std::cout << "  No particle bunches registered.\n";
        }
        
        // Vector Fields Section - just show counts
        std::cout << "\n🔢 VECTOR FIELDS SUMMARY:\n";
        std::cout << std::string(50, '-') << "\n";
        
        std::size_t total_vector_fields = 0;
        for (unsigned d = 1; d <= Dim; ++d) {
            for (unsigned vd = 2; vd <= 3; ++vd) {
                try {
                    std::size_t count = 0;
                    
                    if constexpr (Dim >= 1) {
                        if (d == 1 && vd == 2) count = get_container_size(VisSubRegistry<T, 1, 2>::vf_c);
                        if (d == 1 && vd == 3) count = get_container_size(VisSubRegistry<T, 1, 3>::vf_c);
                    }
                    if constexpr (Dim >= 2) {
                        if (d == 2 && vd == 2) count = get_container_size(VisSubRegistry<T, 2, 2>::vf_c);
                        if (d == 2 && vd == 3) count = get_container_size(VisSubRegistry<T, 2, 3>::vf_c);
                    }
                    if constexpr (Dim >= 3) {
                        if (d == 3 && vd == 2) count = get_container_size(VisSubRegistry<T, 3, 2>::vf_c);
                        if (d == 3 && vd == 3) count = get_container_size(VisSubRegistry<T, 3, 3>::vf_c);
                    }
                    
                    if (count > 0) {
                        std::cout << "  Dimension " << d << "D, Vector " << vd << "D: " << count << " fields\n";
                        total_vector_fields += count;
                    }
                } catch (...) {
                    std::cout << "  Dimension " << d << "D, Vector " << vd << "D: [Error accessing container]\n";
                }
            }
        }
        
        if (total_vector_fields == 0) {
            std::cout << "  No vector fields registered.\n";
        }
        
        // Summary
        std::cout << "\n💾 MEMORY & CONTAINER STATISTICS:\n";
        std::cout << std::string(50, '-') << "\n";
        
        // Count allocated containers vs total possible containers
        int allocated_containers = 0;
        int total_possible_containers = 0;
        
        // Base case containers (1,1)
        total_possible_containers += 3; // sf_c, vf_c, pb_c
        if (VisSubRegistry<T, 1, 1>::sf_c) allocated_containers++;
        if (VisSubRegistry<T, 1, 1>::vf_c) allocated_containers++;
        if (VisSubRegistry<T, 1, 1>::pb_c) allocated_containers++;
        
        // Horizontal axis (1, VDim>1)
        for (unsigned vd = 2; vd <= 3; ++vd) {
            total_possible_containers += 1; // vf_c only
            if (vd == 2 && VisSubRegistry<T, 1, 2>::vf_c) allocated_containers++;
            if (vd == 3 && VisSubRegistry<T, 1, 3>::vf_c) allocated_containers++;
        }
        
        // Vertical axis (Dim>1, 1)  
        for (unsigned d = 2; d <= Dim; ++d) {
            total_possible_containers += 3; // sf_c, vf_c, pb_c
            if constexpr (Dim >= 2) {
                if (d == 2) {
                    if (VisSubRegistry<T, 2, 1>::sf_c) allocated_containers++;
                    if (VisSubRegistry<T, 2, 1>::vf_c) allocated_containers++;
                    if (VisSubRegistry<T, 2, 1>::pb_c) allocated_containers++;
                }
            }
            if constexpr (Dim >= 3) {
                if (d == 3) {
                    if (VisSubRegistry<T, 3, 1>::sf_c) allocated_containers++;
                    if (VisSubRegistry<T, 3, 1>::vf_c) allocated_containers++;
                    if (VisSubRegistry<T, 3, 1>::pb_c) allocated_containers++;
                }
            }
        }
        
        // General case (Dim>1, VDim>1)
        for (unsigned d = 2; d <= Dim; ++d) {
            for (unsigned vd = 2; vd <= 3; ++vd) {
                total_possible_containers += 1; // vf_c only
                if constexpr (Dim >= 2) {
                    if (d == 2 && vd == 2 && VisSubRegistry<T, 2, 2>::vf_c) allocated_containers++;
                    if (d == 2 && vd == 3 && VisSubRegistry<T, 2, 3>::vf_c) allocated_containers++;
                }
                if constexpr (Dim >= 3) {
                    if (d == 3 && vd == 2 && VisSubRegistry<T, 3, 2>::vf_c) allocated_containers++;
                    if (d == 3 && vd == 3 && VisSubRegistry<T, 3, 3>::vf_c) allocated_containers++;
                }
            }
        }
        
        std::cout << "  Container Allocation Statistics:\n";
        std::cout << "    • Total Possible Containers: " << total_possible_containers << "\n";
        std::cout << "    • Currently Allocated: " << allocated_containers << "\n";
        std::cout << "    • Allocation Percentage: " << 
                     (total_possible_containers > 0 ? (100.0 * allocated_containers / total_possible_containers) : 0.0) 
                     << "%\n\n";
        
        std::cout << "  Field Count Summary:\n";
        std::cout << "    • Total Scalar Fields: " << total_scalar_fields << "\n";
        std::cout << "    • Total Particle Bunches: " << total_particle_bunches << "\n";
        std::cout << "    • Total Vector Fields: " << total_vector_fields << "\n";
        std::cout << "    • Grand Total Objects: " << (total_scalar_fields + total_particle_bunches + total_vector_fields) << "\n\n";
        
        std::cout << "  Memory Efficiency:\n";
        std::cout << "    • Estimated Registry Size: " << sizeof(*this) << " bytes\n";
        std::cout << "    • Estimated Container Overhead: " << (allocated_containers * sizeof(std::unique_ptr<std::vector<void*>>)) << " bytes\n";
        std::cout << "    • Objects per Allocated Container: " << 
                     (allocated_containers > 0 ? ((total_scalar_fields + total_particle_bunches + total_vector_fields) / allocated_containers) : 0) << "\n";
        
        std::cout << "\n" << std::string(80, '=') << "\n";
        std::cout << "End of VisRegistry VERBOSE State Report\n";
        std::cout << std::string(80, '=') << "\n\n";
        
    } catch (const std::exception& e) {
        std::cerr << "Error in print_state(): " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown error in print_state()" << std::endl;
    }
}


#include "VisBase.hpp"