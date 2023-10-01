//
// Created by Stefan on 10/1/2023.
//

#include "value.h"

namespace TinyAutograd {

    Value::Value(T data) {
        Data = data;
    }

    Value::Value(T data, const std::set<Value>& children, const std::string& op) {
        Data = data;

        m_Previous = children;
        m_Operation = op;
    }

    Value Value::operator+(const Value& other) const {
        auto out = Value(Data + other.Data, { *this, other }, "+");
        return out;
    }

    Value Value::operator*(const Value &other) const {
        auto out = Value(Data * other.Data, { *this, other }, "*");
        return out;
    }

    std::partial_ordering Value::operator<=>(const Value& other) const {
        return Data <=> other.Data;
    }

    std::ostream &operator<<(std::ostream &out, const Value& v) {
        out << v.Data;
        return out;
    }

}
