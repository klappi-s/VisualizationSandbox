#pragma once
// #include "manager.hpp"
/* circular inclusion not hindered by pragma once???ö. */




// template<typename pc_T, typename fc_T>
// ->
// class VisManager : public VisBase<pc_T, fc_T> {

class VisBase{
  double member;
};


template<typename pc_T, typename fc_T>
class VisManager 
: public VisBase
{
  public:
    VisManager();



    std::vector<pc_T*> pc_v;
    // std::vector<fc_T*> fc_v;


    inline static std::vector<pc_T*>* pc; 
    // inline static std::shared_ptr<std::vector<pc_T*>> pc; 
    // inline static std::unique_ptr<std::vector<pc_T*>> pc; 
    
    
    // inline static std::vector<fc_T*>* fc;
    // inline static std::shared_ptr<std::vector<fc_T*>> fc;
    inline static std::unique_ptr<std::vector<fc_T*>> fc;

};

#include"VisBase.hpp"



// friend class BaseManagerVis;
// friend class Field<T>;
// friend class ParticleBase<T>;

    