//
// Created by gavinandre on 3/1/19.
//

class simple_bean {
public:
    int integer;

    simple_bean() = default;

    explicit simple_bean(const int &integer) {
        this->integer = integer;
    }
};