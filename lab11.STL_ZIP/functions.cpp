#include "functions.h"



namespace stl {


    template<typename InputIt, typename UnaryPredicate>
    bool all_of(InputIt first, InputIt last, UnaryPredicate p) {
        last--;
        while (first != last) {
            if (!p(*first)) {
                return false;
            }
            first++;
        }
        return true;
    }


    template<typename InputIt, typename UnaryPredicate>
    bool any_of(InputIt first, InputIt last, UnaryPredicate p) {
        last--;
        while (first != last) {
            if (p(*first)) {
                return true;
            }
            first++;
        }
        return false;
    }


    template<typename InputIt, typename UnaryPredicate>
    bool none_of(InputIt first, InputIt last, UnaryPredicate p) {
        last--;
        while (first != last) {
            if (p(*first)) {
                return false;
            }
            first++;
        }
        return true;
    }

    template<typename InputIt, typename UnaryPredicate>
    bool one_of(InputIt first, InputIt last, UnaryPredicate p) {
        int cnt = 0;
        last--;
        while (first != last) {
            if (p(*first)) {
                cnt++;
            }
            first++;
        }
        if (cnt == 1) {
            return true;
        } else {
            return false;
        }
    }

    template<typename InputIt>
    bool is_sorted(InputIt first, InputIt last) {
        last--;
        while (first != last) {
            if (!(*first <= *(first+1))) {
                return false;
            }
            first++;
        }
        return true;
    }

    template<typename InputIt, typename UnaryPredicate>
    bool is_partitioned(InputIt first, InputIt last, UnaryPredicate p){

        bool flag = false;
        if (first==last){
            return true;
        }
        last--;
        while (first<=last){
            if (!p(*first)){
                flag=true;
            }
            if (flag and p(*first)){
                return false;
            }
            first++;
        }
        return true;
    }


    template<typename InputIt, typename Value>
    InputIt find_not(InputIt first, InputIt last, Value& val){
        last--;
        while(first!=last){
            if (*first!=val){
                return first;
            }
            first++;
        }
        return last;
    }

    template<typename InputIt, typename Value>
    InputIt find_backward (InputIt first, InputIt last, Value &val){
        last--;
        while(last>=first){
            if (*last==val){
                return last;
            }
            last--;
        }
        return first;
    }

    template<typename InputIt>
    bool is_palindrome(InputIt first, InputIt last){
        last--;
        while ((first)!=(last)){
            if (*first!=*last){
                return false;
            }
            first++;
            last--;
        }
        return true;
    }



    template<typename T>
    class XRange{
        private:
        T x_start_;
        T x_end_;
        T x_step_;
    public:
        XRange(T start, T end, T step)
        :x_start_(start),
        x_end_(end),
        x_step_(step)
        {}

        class Iterator: public std::iterator<std::input_iterator_tag, T>{
        private:
            T it;
            T end_;
            T step_;
        public:
            Iterator(T val, T end, T step)
            :it(val),
            end_(end),
            step_(step)
            {}

            Iterator& operator++() {
                if (it+step_ <= end_) {
                    it = it+step_;
                }
                else{
                    it=end_;
                }
                return *this;
            }

            bool operator==(Iterator& i) const {
                return it == i.it;
            }
            bool operator!=(Iterator& i) const{
                return it != i.it;
            }
            T operator*() {
                return it;
            }
        };

        Iterator begin() const{
            return Iterator(x_start_, x_end_, x_step_);
        }
        Iterator end() const{
            return Iterator(x_end_, x_end_, x_step_);
        }

    };

    template<typename T>
    XRange<T> xrange(T start, T end, T step){
        return XRange<T>(start, end, step);
    }
    template<typename T>
    XRange<T> xrange(T start, T end){
        return XRange<T>(start,end, 1);
    }
    template<typename T>
    XRange<T> xrange(T end){
        return XRange<T>(0, end, 1);
    }


    template<typename Container1, typename Container2>
    class Zip {
    private:
        using It1 = typename Container1::iterator;
        using It2 = typename Container2::iterator;
        It1 it1_begin;
        It2 it2_begin;
        It1 it1_end;
        It2 it2_end;

    public:
        Zip(Container1 &c1, Container2 &c2)
                : it1_begin(c1.begin()),
                  it2_begin(c2.begin()),
                  it1_end(c1.end()),
                  it2_end(c2.end())
                  {}
        class Iterator {
        private:
            using Val1 = typename Container1::value_type;
            using Val2 = typename Container2::value_type;
            std::pair<Val1, Val2> double_iterator_;
            It1 iterator_1;
            It2 iterator_2;
            It1 end1_;
            It2 end2_;

        public:
            Iterator(It1 val1, It2 val2, It1 end1, It2 end2)
            :iterator_1(val1),
            iterator_2(val2),
            end1_(end1),
            end2_(end2),
            double_iterator_(*val1, *val2)
            {}
            Iterator& operator++(){
                ++iterator_1;
                ++iterator_2;
                if (iterator_1 == end1_ or iterator_2 == end2_){
                    iterator_1 = end1_;
                    iterator_2 = end2_;
                }

                else{
                    double_iterator_.first = *iterator_1;
                    double_iterator_.second = *iterator_2;
                }


                return *this;
            }
            bool operator==(const Iterator& i)const {
                return (iterator_1==i.iterator_1 and iterator_2==i.iterator_2);
            }
            bool operator!=(const Iterator& i) const {
                return (iterator_1 != i.iterator_1 or iterator_2 != i.iterator_2);
            }
            std::pair<Val1, Val2>& operator*(){
                return double_iterator_;
            }

        };

        Iterator begin() {
            return Iterator(it1_begin, it2_begin, it1_end, it2_end);
        }

        Iterator end() {
            return Iterator(it1_end, it2_end, it1_end, it2_end);
        }

    };
    template<typename Container1, typename Container2>
    Zip<Container1,Container2> zip(Container1& c1, Container2& c2){
        return Zip<Container1, Container2>(c1, c2);
    }


}