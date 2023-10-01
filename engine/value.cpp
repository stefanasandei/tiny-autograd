#include "value.h"

namespace TinyAutograd {

    Value::Value(T data) {
        Data = data;
    }

    Value Value::operator+(Value &other) const {
        auto out = Value(Data + other.Data);
        return out;
    }

    Value Value::operator*(Value &other) const {
        auto out = Value(Data * other.Data);
        return out;
    }

    std::ostream &operator<<(std::ostream &out, Value v) {
        out << v.Data;
        return out;
    }

}
