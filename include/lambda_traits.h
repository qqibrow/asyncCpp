/*
 *  lambda_traits is used to extract parameter type and return type of a lambda. code copy from:
 *  http://stackoverflow.com/questions/7943525/is-it-possible-to-figure-out-the-parameter-type-and-return-type-of-a-lambda/7943765#7943765
 */
#ifndef PROJECTTEMPLATE_LAMBDA_TRAITS_H
#define PROJECTTEMPLATE_LAMBDA_TRAITS_H

#include <functional>

namespace SO {
    template <typename T>
    struct lambda_traits
            : public lambda_traits<decltype(&T::operator())>
    {};
// For generic types, directly use the result of the signature of its 'operator()'

    template <typename ClassType, typename ReturnType, typename... Args>
    struct lambda_traits<ReturnType(ClassType::*)(Args...) const>
// we specialize for pointers to member function
    {
        enum { arity = sizeof...(Args) };
        // arity is the number of arguments.

        typedef ReturnType result_type;

        template <size_t i>
        struct arg
        {
            typedef typename std::tuple_element<i, std::tuple<Args...>>::type type;
            // the i-th argument is equivalent to the i-th tuple element of a tuple
            // composed of those arguments.
        };
    };
}

#endif //PROJECTTEMPLATE_LAMBDA_TRAITS_H
