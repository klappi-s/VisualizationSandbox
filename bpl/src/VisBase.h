#pragma once

class VisBase{
std::string vis_ID;
};



// template<typename T, unsigned Dim, unsigned VDim>
// class VisSubRegistry
// : public VisBase
// {
//   public:
//     VisSubRegistry();

//     using sf_c_t = Field<std::array<T,VDim>,Dim>;

//     inline static std::unique_ptr<std::vector<vf_c_t*>> vf_c;
    
    
//     //     inline static std::unique_ptr<std::vector<ParticleBase<T,Dim>*>> pb_c; 
//     //     inline static std::unique_ptr<std::vector<Field<T,Dim>*>> sf_c;
//     private:
//     VisRegistry<T, Dim, VDim-1> subRegistry;

// };

// // template<typename T, unsigned Dim, unsigned VDim>
// template<typename T, unsigned Dim>
// class VisSubRegistry<T, Dim, 1>
// : public VisBase
// {
//   public:
//     VisSubRegistry();

//     using vf_c_t = Field<std::array<T,VDim>,Dim>;

//     inline static std::unique_ptr<std::vector<vf_c_t*>> vf_c;
    
    
// };



// template<typename T, unsigned Dim, unsigned VDim = 3, bool Vbool>
template<typename T, unsigned Dim, unsigned VDim = 3>
class VisRegistry
: public VisBase
{
  public:
    VisRegistry();

    using pb_c_t = ParticleBase<T,Dim>;
    using sf_c_t = Field<T,Dim>;
    // using vf_c_t = Field<std::array<T,VDim>,Dim>;


    inline static std::unique_ptr<std::vector<pb_c_t*>> pb_c; 
    inline static std::unique_ptr<std::vector<sf_c_t*>> sf_c; 
    // inline static std::unique_ptr<std::array<vf_c_t*>> vf_c;
    
    
    //     inline static std::unique_ptr<std::vector<ParticleBase<T,Dim>*>> pb_c; 
    //     inline static std::unique_ptr<std::vector<Field<T,Dim>*>> sf_c;
    private:
    // VisRegistry<T, Dim-1, VDim> subD;
    // VisSubRegistry<T, Dim, VDim> subV;

};






#include"VisBase.hpp"


/* only make sense with higher handler object but i think that not worth */
// inline static std::unique_ptr<std::vector<std::unique_ptr<pc_T>>> pb_c; 
// inline static std::unique_ptr<std::vector<std::unique_ptr<fc_T>>> sf_c;