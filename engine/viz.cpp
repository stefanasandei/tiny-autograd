//
// Created by Stefan on 10/1/2023.
//

#include "engine/viz.h"

#include <functional>
#include <string>
#include <fstream>
#include <utility>
#include <format>

namespace TinyAutograd {

    const std::string GraphvizTemplate = R"(
digraph G {
    rankdir=LR;
    node [shape=box, fontsize=12];

)";

    std::pair<std::set<Value>, std::set<std::pair<Value, Value>>> Trace(const Value& leaf) {
        std::set<Value> nodes;
        std::set<std::pair<Value, Value>> edges;

        std::function<void(const Value&)> build = [&] (const Value& node) {
            if(!nodes.contains(node)) {
                nodes.insert(node);
                for(const auto& child : node.GetPrevious()) {
                    edges.insert({child, node});
                    build(child);
                }
            }
        };

        build(leaf);
        return { nodes, edges };
    }

    void Visualize(const Value& leafNode) {
        auto dot = GraphvizTemplate;

        auto [nodes, edges] = Trace(leafNode);

        for(const auto& node : nodes) {
            auto uid = std::format("\tnode{}", std::hash<float>{}(node.Data));
            dot += std::format("\t{} [label=\"{}\"];\n", uid, node.Data);

            if(!node.GetOperation().empty()) {
                dot += std::format("\t{}{} [label=\"{}\", shape=ellipse];\n", uid, int(node.GetOperation()[0]), node.GetOperation());
                dot += std::format("\t{}{} -> {};\n", uid, int(node.GetOperation()[0]), uid);
            }
        }

        for(const auto& [n1, n2] : edges) {
            auto uid1 = std::format("\tnode{}", std::hash<float>{}(n1.Data));
            auto uid2 = std::format("\tnode{}", std::hash<float>{}(n2.Data));

            dot += std::format("\t{} -> {}{};\n", uid1, uid2, int(n2.GetOperation()[0]));
        }

        dot += "}\n";

        std::ofstream out("./res/viz.dot");
        out.write(dot.c_str(), dot.size());
        out.close();

        system(" dot -Tpng ./res/viz.dot -o ./res/viz.png");
    }

}
