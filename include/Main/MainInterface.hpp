//
// Created by pyrowildx on 06/08/2023.
//

#ifndef MAIN_HPP
#define MAIN_HPP

#include <type_traits>
#include "Utils/Utils.hpp"

template<typename Derived>
class MainInterface {

public:
    MainInterface() = delete;

    static inline void Run() {
        static_assert(std::is_base_of<MainInterface, Derived>::value,
                      "MainInterface : RunImpl() not Implemented !");
        Derived::RunImpl();
    }

private:

};

#endif
