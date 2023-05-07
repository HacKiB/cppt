//
// Created by zhangyang on 5/7/2023.
//

#ifndef DIG_SOME_UTILS_H
#define DIG_SOME_UTILS_H

#include <iostream>
using namespace std;

#define loop for(;;)

template <typename A>
void print(const A x){
    std::cout << x << std::endl;
}

template<typename A,typename ...B>
void print(A x1,B... x2_xn){
    std::cout << x1 << " " ;
    print(x2_xn...);
}

#endif //DIG_SOME_UTILS_H

