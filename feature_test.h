//
// Created by zhangyang on 5/7/2023.
//

#ifndef DIG_SOME_FEATURE_TEST_H
#define DIG_SOME_FEATURE_TEST_H

#import "tuple"
#import "iostream"
#import "exception"
#import "utils.h"


class feature_test{
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

void my_main(){
    print(1);
    feature_test A;
    A.go();
    print(A.p);
}

#endif //DIG_SOME_FEATURE_TEST_H
