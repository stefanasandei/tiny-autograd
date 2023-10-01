#pragma once

#include <ostream>

namespace TinyAutograd {

    using T = float;

    class Value {
    public:
        explicit Value(T data);

        Value operator+(Value& other) const;
        Value operator*(Value& other) const;

        friend std::ostream &operator<< (std::ostream &out,
                                           Value v);
    public:
        T Data;
    };

}
