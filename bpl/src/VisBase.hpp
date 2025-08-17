#include"bpl.h"

template<typename pc_T, typename fc_T>
VisManager<pc_T, fc_T>::VisManager() {
    std::cout << "VisBase default constructor" << std::endl;
    
    
    if (!VisManager<pc_T, fc_T>::pc) {
        VisManager<pc_T, fc_T>::pc = &pc_v;
        // VisManager<pc_T, fc_T>::pc = std::make_shared<std::vector<pc_T*>>();
        // VisManager<pc_T, fc_T>::pc = std::make_unique<std::vector<pc_T*>>();
        
    }
    
    
    if (!VisManager<pc_T, fc_T>::fc) {
        // VisManager<pc_T, fc_T>::fc = &fc_v;
        // VisManager<pc_T, fc_T>::fc = std::make_shared<std::vector<fc_T*>>();
        VisManager<pc_T, fc_T>::fc = std::make_unique<std::vector<fc_T*>>();
    }
}


