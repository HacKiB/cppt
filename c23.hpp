//
// Created by Admin on 2023/3/16.
//

#ifndef DIG_SOME_C23_HPP
#define DIG_SOME_C23_HPP

#import "tuple"
#import "iostream"
#import "exception"

using namespace std;


class c23{
public:
    int p{12222};
    void go(){
        tuple t(1,"bbbb",3);
        cout << get<1>(t) << endl;
//        for (auto i : t){
//            cout << i ;
//        }

        int a[3]={1,3};
        auto [x,y,z] = a;


        int a1[3] = {5,6,7};
        auto [x1,x2,x3] = a1;

        cout << x << " " << y << " " << z << endl;
        return ;
    }
};

#endif //DIG_SOME_C23_HPP

