#include <iostream>
#include <vector>
#include <string>
#include <any>
#include <functional>
#include <memory>
#include <algorithm> // Required for std::remove_if
#include <typeindex> // Required for std::type_index

/**
 * @class any_dog
 * @brief A type-erasing wrapper that holds a reference to a dog object.
 *
 * This version is updated to also store the memory address of the original
 * dog object, which allows us to find and remove it from the kennel later.
 */
class any_dog {
public:
    // The constructor now also captures the address and type info of the object.
    template<typename DogType>
    any_dog(const DogType& d) 
        : dog_instance(std::cref(d)), 
          referenced_object_ptr(&d),
          value_type_index(typeid(typename DogType::value_type))
    {
        print_details_caller = [](const std::any& instance){
            std::any_cast<std::reference_wrapper<const DogType>>(instance).get().print_details();
        };
        get_id_caller = [](const std::any& instance) -> const std::string& {
            return std::any_cast<std::reference_wrapper<const DogType>>(instance).get().get_id();
        };
        // This lambda erases the return type of get_value, returning it as a const void*
        get_value_ptr_caller = [](const std::any& instance) -> const void* {
            return &std::any_cast<std::reference_wrapper<const DogType>>(instance).get().get_value();
        };
    }

    void print_details() const {
        print_details_caller(dog_instance);
    }

    // A public getter to retrieve the stored address for comparison.
    const void* get_address() const {
        return referenced_object_ptr;
    }
    
    // Getter for the ID (type is known).
    const std::string& get_id() const {
        return get_id_caller(dog_instance);
    }

    // Templated getter for the value (type is unknown).
    // Returns a pointer to the value if the requested type T is correct, otherwise nullptr.
    template<typename T>
    const T* get_value() const {
        if (typeid(T) == value_type_index) {
            return static_cast<const T*>(get_value_ptr_caller(dog_instance));
        }
        return nullptr;
    }

private:
    std::any dog_instance;
    std::function<void(const std::any&)> print_details_caller;
    std::function<const std::string&(const std::any&)> get_id_caller;
    std::function<const void*(const std::any&)> get_value_ptr_caller;
    const void* referenced_object_ptr; // Stores the address of the original dog.
    std::type_index value_type_index; // Stores the type of the value.
};













/**
 * @class playground
 * @brief A static utility class that holds the global collection of all dog instances.
 */
class playground {
public:
    // The kennel is now a unique_ptr, which manages the vector's memory automatically.
    static std::unique_ptr<std::vector<std::unique_ptr<any_dog>>> kennel;

private:
    // A private constructor prevents anyone from creating an instance of `playground`.
    playground() = default;
};

// The unique_ptr is default-initialized to nullptr.
std::unique_ptr<std::vector<std::unique_ptr<any_dog>>> playground::kennel;







/**
 * @class dog
 * @brief A class template for dogs with a std::string ID and a generic value.
 *
 * The destructor is implemented to find and remove this dog's entry from
 * the global kennel, preventing dangling references.
 */
template<typename T>
class dog {
public:
    using value_type = T; // Type alias for any_dog to query the value type.
    
    std::string id;
    T value;

    // Constructor registers the dog instance in the global kennel.
    dog(std::string identifier, T val) : id(std::move(identifier)), value(val) {
        if (playground::kennel) {
            playground::kennel->push_back(std::make_unique<any_dog>(*this));
        }
    }

    // The destructor now cleans up this dog's entry from the kennel.
    ~dog() {
        std::cout << "  -> Destructor for dog with ID: " << id 
                  << " is running. Removing from kennel.\n";
        
        if (playground::kennel) {
            auto& kennel_ref = *playground::kennel;
            const void* self_address = this;
            kennel_ref.erase(
                std::remove_if(kennel_ref.begin(), kennel_ref.end(),
                    [self_address](const std::unique_ptr<any_dog>& dog_ptr) {
                        return dog_ptr->get_address() == self_address;
                    }),
                kennel_ref.end()
            );
        }
    }

    void print_details() const {
        std::cout << "Dog ID: " << id << ", Value: " << value << "\n";
    }

    // Getter for ID
    const std::string& get_id() const {
        return id;
    }

    // Getter for value
    const T& get_value() const {
        return value;
    }
};










int main() {
    playground::kennel = std::make_unique<std::vector<std::unique_ptr<any_dog>>>();

    std::cout << "--- Creating dogs ---\n";
    dog<int> dog1("Bravo", 85);       // Value is an int
    dog<double> dog2("Charlie", 65.5); // Value is a double
    dog<const char*> dog3("Delta", "Featherweight"); // Value is a string literal

    std::cout << "\n--- Using getters to retrieve dog info from kennel ---\n";
    for (const auto& dog_ptr : *playground::kennel) {
        // 1. Get the ID directly
        std::cout << "Retrieved ID: " << dog_ptr->get_id() << ". ";

        // 2. Try to get the value as different types
        if (const int* value = dog_ptr->get_value<int>()) {
            std::cout << "Retrieved value as int: " << *value << "\n";
        } else if (const double* value = dog_ptr->get_value<double>()) {
            std::cout << "Retrieved value as double: " << *value << "\n";
        } else if (const char** value = dog_ptr->get_value<std::string>()) {
            std::cout << "Retrieved value as string: " << *value << "\n";
        } else {
            std::cout << "Could not retrieve value with known types.\n";
        }
    }
    
    std::cout << "\n--- Program ending ---\n";

    return 0; // dog1, dog2, and dog3 destructors are called here.
}
