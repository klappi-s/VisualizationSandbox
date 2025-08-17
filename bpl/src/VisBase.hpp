#include"manager.h"

template<typename pc_T, typename fc_T>
VisBase<pc_T, fc_T>::VisBase() {
    std::cout << "VisBase default constructor" << std::endl;
    if (!VisBase<pc_T, fc_T>::pc) {
        VisBase<pc_T, fc_T>::pc = std::make_unique<std::vector<std::shared_ptr<pc_T>>>();
    }
    if (!VisBase<pc_T, fc_T>::fc) {
        VisBase<pc_T, fc_T>::fc = std::make_unique<std::vector<std::shared_ptr<fc_T>>>();
    }
}


