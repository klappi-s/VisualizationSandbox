#include <iostream>
#include <string>
#include <unordered_map>
#include <any>
#include <stdexcept>
#include <dyna
#include <typeinfo>



// // A dynamic registry with compile-time name-only API via nested mappings
// class Registry {
// private:
//     std::unordered_map<std::string, void*> m_storage;  // string→void* storage
//     std::unordered_map<std::string, size_t> m_type;   // string→type hash

// public:
//     bool empty() const noexcept { return m_storage.empty(); }

//     // Nested mapping: default unknown names to void
//     // template<fixed_string Name>
//     // struct NameToType { using type = void; };



//     // Runtime string API
//     template<typename T>
//     void add(const std::string& name, T& object) {
//         m_storage[name] = const_cast<void*>(static_cast<const void*>(&object));
//         m_type[name] = typeid(object).hash_code();
//     }

//     // Print the stored type (demo)
//     void print_type(const std::string& name) const {
//         auto tIt = m_type.find(name);
//         if (tIt == m_type.end()) {
//             std::cout << name << ": <unknown>" << std::endl;
//             return;
//         }
//         const auto h = tIt->second;
//         if (h == typeid(double).hash_code()) {
//             std::cout << name << ": double" << std::endl;
//         } else if (h == typeid(int).hash_code()) {
//             std::cout << name << ": int" << std::endl;
//         } else {
//             std::cout << name << ": <unmapped type> (hash=" << h << ")" << std::endl;
//         }
//     }





//     template<typename T>
//     auto* get(const std::string& name) const {
//         auto tIt = m_type.find(name);
//         auto sIt = m_storage.find(name);


//         const auto h = tIt->second;
//         void* hi= sIt->second;
        
//         if (h == typeid(double).hash_code()) {
//             std::cout << name << ": double" << std::endl;
//             return static_cast<double*>(hi);
//         }
        
//         else if (h == typeid(int).hash_code()) {
//             std::cout << name << ": int" << std::endl;
//             return static_cast<int*>(hi);
//         } 
        
//         else {
//             std::cout << name << ": <unmapped type> (hash=" << h << ")" << std::endl;
//             return 0;
//         }
//     }


//     template<>
//     auto* get(const std::string& name) const {
//         auto tIt = m_type.find(name);
//         auto sIt = m_storage.find(name);


//         const auto h = tIt->second;
//         void* hi= sIt->second;
        
//         if (h == typeid(double).hash_code()) {
//             std::cout << name << ": double" << std::endl;
//             return static_cast<double*>(hi);
//         }
        
//         else if (h == typeid(int).hash_code()) {
//             std::cout << name << ": int" << std::endl;
//             return static_cast<int*>(hi);
//         } 
        
//         else {
//             std::cout << name << ": <unmapped type> (hash=" << h << ")" << std::endl;
//             return 0;
//         }
//     }



//     // Typed getter that verifies the stored type before casting
//     template<typename T>
//     T* get_typed(const std::string& name) const {
//         auto sIt = m_storage.find(name);
//         if (sIt == m_storage.end()) return nullptr;
//         auto tIt = m_type.find(name);
//         if (tIt == m_type.end()) return nullptr;
//         if (tIt->second != typeid(T).hash_code()) return nullptr; // type mismatch
//         return static_cast<T*>(sIt->second);
//     }



//     bool contains_named(const std::string& name) const {
//         return m_storage.find(name) != m_storage.end();
//     }

//     bool unset_named(const std::string& name) {
//         m_type.erase(name);
//         return m_storage.erase(name) > 0;
//     }
// };

        // else {
        //     std::cout << name << ": <unmapped type> (hash=" << h << ")" << std::endl;
        //     return 0;
        // }




// class field_base{
//     int i;
// };
// class field_A : public field_base{
//     double v;
// };
// class field_B : public field_base{
//     double v;
// };
// class Registry {
//     std::unordered_map<std::string, field_base> m_storage;  // string→void* storage
//     std::unordered_map<std::string, size_t> m_type;   // string→type hash
//     public:

//     template<typename T>
//     void add(const std::string& name, T& object) {
//         m_storage[name] = dynamic_cast<field_base*>(dynamic_cast<const field_base>(object));
//         m_type[name] = typeid(object).hash_code();
//     }


    
//     // template<typename T>
//     auto get(const std::string& name) const {
//         auto tIt = m_type.find(name);
//         auto sIt = m_storage.find(name);

//         const size_t hash = tIt->second;
//         field_base bas = sIt->second;
        
//         if (hash == typeid(field_A).hash_code()) {
//             std::cout << name << "A" << std::endl;
//             return static_cast<field_A>(bas);
//         }
//         else // (h == typeid(field_B).hash_code()) 
//         {
//             std::cout << name << "B" << std::endl;
//             return static_cast<field_B>(bas);
//         }

//     }
// };








class Registry {
    std::unordered_map<std::string, void*>  m_storage;  // string→void* storage
    std::unordered_map<std::string, size_t> m_type;   // string→type hash
    
    public:
    template<typename T>
    void add(const std::string& name, T& object) {
        m_storage[name] = const_cast<void*>(static_cast<const void*>(object));
        m_type[name] = typeid(object).hash_code();
    }
    
    // template<typename T> ???
    auto get(const std::string& name) const {
        size_t hash = m_type.find(name)->second;
        auto sIt = m_storage.find(name);
        void* bas = sIt->second;
        
        if (hash == typeid(double).hash_code()) {
            std::cout << name << "A" << std::endl;
            return static_cast<double*>(bas);
        }
        else // (h == typeid(float).hash_code()) 
        {
            std::cout << name << "B" << std::endl;
            return static_cast<float*>(bas);
        }
    }
};


int main() {
    Registry r;
    double a;
    float b;

    r.add("a", a);
    r.add("b", b);

    /* assume no type knowledge at retrieval */
    auto A = r.get("a");
    auto B = r.get("b");
    
    return 0;
}