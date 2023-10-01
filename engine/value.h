//
// Created by Stefan on 10/1/2023.
//

#pragma once

#include <ostream>
#include <string>
#include <set>

namespace TinyAutograd {

    using T = float;

    class Value {
    public:
        explicit Value(T data);
        Value(T data, const std::set<Value>& children, const std::string& op);

        Value operator+(const Value& other) const;
        Value operator*(const Value& other) const;

        std::partial_ordering operator<=>(const Value& other) const;

        friend std::ostream &operator<< (std::ostream &out,
                                           const Value& v);

        [[nodiscard]] std::set<Value> GetPrevious() const { return m_Previous; }
        [[nodiscard]] std::string GetOperation() const { return m_Operation; }

    public:
        T Data;

    private:
        std::set<Value> m_Previous;
        std::string m_Operation;
    };

}
