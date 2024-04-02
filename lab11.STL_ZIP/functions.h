#include <iostream>
#include <vector>
#include <string>
#include <algorithm>





namespace stl {


    template<typename InputIt, typename UnaryPredicate>
    bool any_of(InputIt first, InputIt last, UnaryPredicate p);


    template<typename InputIt, typename UnaryPredicate>
    bool all_of(InputIt first, InputIt last, UnaryPredicate p);

    template<typename InputIt>
    bool is_sorted(InputIt first, InputIt last);

    template<typename InputIt, typename UnaryPredicate>
    bool one_of(InputIt first, InputIt last, UnaryPredicate p);

    template<typename InputIt, typename UnaryPredicate>
    bool none_of(InputIt first, InputIt last, UnaryPredicate p);

    template<typename InputIt, typename UnaryPredicate>
    bool is_partitioned(InputIt first, InputIt last, UnaryPredicate p);

    template<typename InputIt, typename Value>
    InputIt find_not(InputIt first, InputIt last, Value& val);

    template<typename InputIt, typename Value>
    InputIt find_backward (InputIt first, InputIt last, Value &val);

    template<typename InputIt>
    bool is_palindrome(InputIt first, InputIt last);
}