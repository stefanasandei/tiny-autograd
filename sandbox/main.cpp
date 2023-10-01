//
// Created by Stefan on 10/1/2023.
//

#include "engine/tiny_autograd.h"

#include <iostream>

using namespace TinyAutograd;

int main() {
    auto x1 = Value(2.0); x1.Label = "x1";
    auto x2 = Value(0.0); x2.Label = "x2";

    auto w1 = Value(-3.0); w1.Label = "w1";
    auto w2 = Value(1.0); w2.Label = "w2";

    auto b = Value(6.88); b.Label = "b";

    auto x1w1 = x1 * w1; x1w1.Label = "x1*w1";
    auto x2w2 = x2 * w2; x2w2.Label = "x2*w2";
    auto x1w1x2w2 = x1w1 + x2w2; x1w1x2w2.Label = "x1*w1+x2*w2";
    auto n = x1w1x2w2 + b; n.Label = "n";

    auto o = n.Tanh();
    o.Backward();

    std::cout << o << '\n';

    Visualize(o);

    return 0;
}
