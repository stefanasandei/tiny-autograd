//
// Created by Stefan on 10/1/2023.
//

#pragma once

#include <functional>
#include <ostream>
#include <string>
#include <set>

namespace TinyAutograd {

    using T = float;

    class Value {
    public:
        Value(T data);
        Value(T data, const std::vector<const Value*>& children, const std::string& op);

        Value operator+(const Value& other) const;
        Value operator+(const float& other) const;
        Value operator*(const Value& other) const;
        Value operator*(const float& other) const;
        Value operator-(const Value& other) const;
        Value operator-(const float& other) const;
        Value operator/(const Value& other) const;
        Value operator/(const float& other) const;

        Value Tanh() const;
        Value Pow(float exp) const;
        Value Exp() const;

        std::partial_ordering operator<=>(const Value& other) const;

        friend std::ostream &operator<< (std::ostream &out,
                                           const Value& v);

        void Backward();

        [[nodiscard]] std::vector<const Value*> GetPrevious() const { return m_Previous; }
        [[nodiscard]] std::string GetOperation() const { return m_Operation; }

        void SetBackwardFn(const std::function<void()>& fn) { BackwardFn = fn; }

    public:
        mutable T Data;
        mutable float Gradient;
        std::string Label;

    private:
        std::function<void()> BackwardFn;
        std::vector<const Value*> m_Previous;
        std::string m_Operation;
    };

}
