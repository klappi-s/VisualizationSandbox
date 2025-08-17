#pragma once
// #include "manager.hpp"
/* circular inclusion not hindered by pragma once???ö. */


template<typename pc_T, typename fc_T>
class VisBase {
  public:

    /* implemtn member function for access is preferred.. */
    // friend class BaseManagerVis;
    // friend class Field<T>;
    // friend class ParticleBase<T>;

    VisBase();
    
  // private:
    inline static std::unique_ptr<std::vector<std::shared_ptr<pc_T>>> pc = nullptr;
    inline static std::unique_ptr<std::vector<std::shared_ptr<fc_T>>> fc = nullptr;

    std::vector<std::shared_ptr<pc_T>> pc_v;
    std::vector<std::shared_ptr<fc_T>> fc_v;


};

#include"VisBase.hpp"