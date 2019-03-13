//
// Created by gavinandre on 3/1/19.
//

#include <vector>

class complex_bean {
public:
    int integer;
    std::vector<float> list;

    complex_bean() = default;

    complex_bean(const int &integer, const std::vector<float> &list) {
        this->integer = integer;
        this->list = list;
    }
};