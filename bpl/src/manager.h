#pragma once

#include"bpl.h"


class BaseManager{
  public:
  
    // BaseManager()          = default;
    // virtual ~BaseManager() = default;

    void run(){
      std::cout << "start run" << std::endl;
      int nt = 10;
      for (int it = 0; it < nt; it++) {
          this->pre_step();
                  this->advance();
                  this->post_step();
      }
           std::cout << "end run" << std::endl;
    }
    void pre_run(){
      std::cout << "prerun" << std::endl;
    }

    /* even if virtual these must not only be declared but initialized */
    virtual void pre_step() {}
    virtual void post_step() {}
    virtual void advance() {}

   

};


class BaseManagerVis : public BaseManager {
  public:
    // BaseManagerVis();

    // BaseManagerVis()          = default;

    /* run was not intended to be overwritten but we do it anyway ... so no keyword override */
    
    // void run();
    // void pre_run();
    
    void pre_step();
    void post_step();
    void advance();

//   private:
    vb_t vb;
    


};



void BaseManagerVis::pre_step(){
    // set some internal state
}

void BaseManagerVis::post_step(){
    // set some internal state
}

void BaseManagerVis::advance(){
    // set some internal state
}
