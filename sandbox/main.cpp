//
// Created by Stefan on 10/1/2023.
//

#include "engine/tiny_autograd.h"

#include <iostream>

using namespace TinyAutograd;

int main() {
    auto a = Value(2.0);
    auto b = Value(-3.0);
    auto c = Value(10.0);

    auto d = a * b + c;

    std::cout << a * b + c << '\n';

    Visualize(d);

    return 0;
}
