#include "functions.cpp"

bool Pred(int x){
    return x%2==0;
}

class big_int {
public:
    int value[68];
    size_t size;



    big_int &operator=(int a) {
        size = a;
        return *this;
    }

    big_int &operator+=(big_int &a) {
        size += a.size;
        return *this;
    }
    bool operator!=(big_int &a) const {
        if (size != a.size) {
            return true;
        }
        return false;
    }

    big_int& operator++(){
        size++;
        return *this;
    }

    big_int operator++(int){
        big_int tmp(*this);
        operator++();
        return tmp;
    }

    big_int& operator--(){
        size--;
        return *this;
    }

    big_int operator--(int){
        big_int tmp(*this);
        operator--();
        return tmp;
    }


    bool operator<(const big_int& a) const {
        return size < a.size;
    }

    bool operator>(big_int &a) const {
        if (size > a.size) {
            return true;
        }
        return false;
    }

    bool operator>(const big_int& other) const {
        return other < *this;
    }

    bool operator<=(const big_int& a) const {
        return !(size > a.size);
    }

    bool operator>=(const big_int& a) const {
        return !(size < a.size);
    }

    bool operator==(const big_int& a) const {
        return size == a.size;
    }

    bool operator!=(const big_int& a) const {
        return !(size == a.size);
    }


    big_int() = default;

    big_int(const big_int &a) {
        size = a.size;
    }

    big_int(int f) {
        size = f;
    }
};

class cat {
public:
    int age;
    int tail_lenght;

    cat(int a_,  int len) {
        age = a_;
        tail_lenght = len;
    }
};

using namespace stl;

int main() {


// 1 3 5
    for(auto i : xrange(1, 6, 2)) {
        std::cout << i <<  " ";
    }


/*    auto x = xrange(5);
    std::vector<int> v{x.begin(), x.end()};
    for (auto i:v){
        std::cout<<i;
    }*/
    /*for(auto i : xrange(100)) {
        std::cout << i <<  " ";
    }*/
/*    //std::cout<<Pred(2);
    std::initializer_list<int> b = {1, 2, 3, 4, 5};
    std::vector<char> v = {'a', 'b', 'c', 'd'};
*//*
1 a
2 b
3 c
4 d

 *//*

    for(auto value : zip(b, v)) {
        std::cout << value.first << " " << value.second << std::endl;
    }*/

    return 0;
}
