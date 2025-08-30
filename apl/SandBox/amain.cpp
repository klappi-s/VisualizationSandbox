#include<tuple>
#include<string>
#include<iostream>

#include <iostream>
#include <string>
#include <unordered_map>
#include <typeindex>
#include <type_traits>
#include <array>
#include <string_view>


#include <typeinfo>



// =================================================================================
// FixedString for Compile-Time String Tags
// =================================================================================

template<size_t N>
struct FixedString {
    std::array<char, N> data{};
    
    constexpr FixedString(const char* str) {
        for (size_t i = 0; i < N; ++i) data[i] = str[i];
    }
    
    constexpr std::string_view to_view() const {
        return {data.data(), N-1}; // Exclude null terminator
    }
    
    constexpr bool operator==(const FixedString& other) const {
        return data == other.data;
    }
    
    constexpr size_t size() const { return N - 1; }
};

// Deduction guide for FixedString
template<size_t N>
FixedString(const char (&)[N]) -> FixedString<N>;



// /================================================================
// /================================================================
// /================================================================
// /================================================================
// /================================================================



template<FixedString IdTag>
struct TagType {using type = void; }; 

template<FixedString IdTag>
using tag_type_t = typename TagType<IdTag>::type;




/* connection can only be made in global scope .... */
#define CONNECT_TAG(ID, TYP) template<> struct TagType<ID> { using type = TYP; }
template<> struct TagType<"Id_A">    { using type = int;    };
CONNECT_TAG("Id_B", bool);
CONNECT_TAG("Id_C", double);




// /================================================================
// /================================================================
// /================================================================
// /================================================================
// /================================================================
// /================================================================


// 1. Connection does't make sens since any connection could be also specialized
template<FixedString Name, typename T>
struct Connector {
    using type = T;
    static constexpr auto name = Name;
    static constexpr std::string_view get_name() { return Name.to_view(); }
};



template<FixedString Name, typename T>
inline constexpr Connector<Name, T> string_tag{};


// =================================================================================
// Convenience Macros for Creating String Tags on the Spot
// =================================================================================

// Macro to create a string tag with type

#define CONNECT_T(name, TYP) Connector<FixedString{name}, TYP>

#define CONNECT(name, TYP) Connector<FixedString{name}, TYP>{}

// Macro to create a string tag variable
#define GET_CONST_CONNECT(var_name, name, TYP) \
    constexpr auto var_name = Connector<FixedString{name}, TYP>{}


// /================================================================


template<class T>
void printt(){
    std::cout << typeid(T).name() << std::endl;
}


template<class T>
void printt_type(){
    std::cout << typeid(typename T::type).name() << std::endl;
}



template<class T>
void printt_type2(T o){
    std::cout << typeid(typename decltype(o)::type).name() << std::endl;
}










void test2(){

    printt<CONNECT_T("useles", double)>();
    printt<CONNECT_T("useles", double)::type>();
    printt_type<CONNECT_T("useles", double)>();

    std::cout << std::endl;

    printt<decltype(CONNECT("useles", double))>();
    printt<decltype(CONNECT("useles", double))::type>();
    printt_type<decltype(CONNECT("useles", double))>();
    


    GET_CONST_CONNECT(useles, "useles", double);


    printt<decltype(useles)>();
    printt<decltype(useles)::type>();
    printt_type<decltype(useles)>();
    printt_type2(useles);

    




    // std::cout << typeid(decltype(CONNECT("useles", double))::type).name() << std::endl;

    // printt(CONNECT("useles", double));

    
    
    // printt(GET_CONST_CONNECT(vakant, "useles", double));

    // printt<CONNECT_T("useles", double)::type>();




    // CONNECT("useles", double).type;
    // std::cout << typeid(T).name() << std::endl;


    // printt(CONNECT_T("useles", double));

    // printt(useles);





}









void test1(){

    printt<tag_type_t<"Id_A">>();
    printt<tag_type_t<"Id_B">>();
    printt<tag_type_t<"Id_C">>();

}



int main(){
    // test1();
    test2();

    return 0;
}