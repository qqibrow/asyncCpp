//
// Created by Lu Niu on 8/30/15.
//

#ifndef PROJECTTEMPLATE_SERIES_H
#define PROJECTTEMPLATE_SERIES_H

#include <iostream>
#include <functional>
#include "arg_pack.h"
#include "lambda_traits.h"

/*
 * traits to extract 1st function parameter from template shape like std::function<void(R)>
 */

namespace workflow {
    template <typename T>
    struct Trait;

    template <typename R>
    struct Trait<std::function<void(R)>> {
        typedef R ARG_TYPE;

    };

    static_assert(std::is_same<Trait<std::function<void(int)>>::ARG_TYPE, int>::value, "int and int is same type");
    static_assert(!std::is_same<Trait<std::function<void(int)>>::ARG_TYPE, std::string>::value, "int and string is not");


    struct DoTask {
        template<typename T>
        void operator()(T func) {
            typedef typename SO::lambda_traits<decltype(func)>::template arg<0>::type TTT;
            typedef typename Trait<TTT>::ARG_TYPE RETURN_TYPE;
            auto f = [](RETURN_TYPE a) {
                std::cout << a;
            };
            func(f);
        }
    };

    template<typename... Ts>
    void series(Ts&&... args) {
        SO::for_each_in_arg_pack(DoTask(), std::forward<Ts>(args)...);
    }
}

#endif //PROJECTTEMPLATE_SERIES_H
