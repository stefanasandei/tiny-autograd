//
// Created by Stefan on 10/1/2023.
//

#include "value.h"

#include <iostream>

namespace TinyAutograd {

    Value::Value(T data) {
        Data = data;
        Gradient = 0.0;
        BackwardFn = []{};
    }

    Value::Value(T data, const std::vector<const Value*>& children, const std::string& op) {
        Data = data;
        Gradient = 0.0;
        BackwardFn = []{};

        m_Previous = children;
        m_Operation = op;
    }

    Value Value::operator+(const Value& other) const {
        std::vector<const Value*> children;
        children.push_back(this);
        children.push_back(&other);

        auto out = Value(Data + other.Data, children, "+");

        out.SetBackwardFn([&] {
            this->Gradient += 1.0f * out.Gradient;
            out.Gradient += 1.0f * out.Gradient;
        });

        return out;
    }

    Value Value::operator*(const Value &other) const {
        std::vector<const Value*> children;
        children.push_back(this);
        children.push_back(&other);

        auto out = Value(Data * other.Data, children, "*");

        out.SetBackwardFn( [&] {
            this->Gradient += other.Data * out.Gradient;
            other.Gradient += this->Data * out.Gradient;
        });

        return out;
    }

    Value Value::Tanh() const {
        std::vector<const Value*> children;
        children.push_back(this);

        T x = Data;
        float t = (std::exp(2.0f*x) - 1.0f) / (std::exp(2.0f*x) + 1.0f);
        auto out = Value(t, children, "tanh");

        out.SetBackwardFn([&, t] {
            this->Gradient += (1 - t*t) * out.Gradient;
        });

        return out;
    }

    Value Value::Pow(float exp) const {
        std::vector<const Value*> children;
        children.push_back(this);

        auto out = Value(std::pow(Data, exp), children, "pow");

        out.SetBackwardFn([&] {
            this->Gradient += exp * std::pow(Data, exp-1) * out.Gradient;
        });

        return out;
    }

    Value Value::Exp() const {
        std::vector<const Value*> children;
        children.push_back(this);

        auto out = Value(std::exp(Data), children, "exp");

        out.SetBackwardFn([&] {
            this->Gradient += out.Data * out.Gradient;
        });

        return out;
    }

    void Value::Backward() {
        std::vector<const Value*> topo;
        std::set<const Value*> visited;

        std::function<void(Value*)> buildTopo = [&] (const Value* v) {
            if(!visited.contains(v)) {
                visited.insert(v);
                for(auto child : v->GetPrevious()) {
                    buildTopo(const_cast<Value*>(child));
                }
                topo.emplace_back(v);
            }
        };

        buildTopo(this);

        Gradient = 1.0f;
        for(auto it = topo.rbegin(); it != topo.rend(); ++it) {
            (*it)->BackwardFn();
        }
    }

    Value Value::operator+(const float &other) const {
        return *this + Value(other);
    }

    Value Value::operator*(const float &other) const {
        return *this * Value(other);
    }

    Value Value::operator-(const Value &other) const {
        return *this + (other * -1);
    }

    Value Value::operator-(const float &other) const {
        return *this + (Value(other) * -1);
    }

    Value Value::operator/(const Value &other) const {
        return *this * (other.Pow(-1));
    }

    Value Value::operator/(const float &other) const {
        return *this * (Value(other).Pow(-1));
    }

    std::partial_ordering Value::operator<=>(const Value& other) const {
        return Data <=> other.Data;
    }

    std::ostream &operator<<(std::ostream &out, const Value& v) {
        out << v.Data;
        return out;
    }

}
