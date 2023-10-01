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

    std::string GetLabel(const Value& v) {
        if(v.Label.empty()) {
            return std::format("data {} | grad {}", v.Data, v.Gradient);
        }
        return std::format("{} | data {} | grad {}", v.Label, v.Data, v.Gradient);
    }

    std::string GetID(const Value& v) {
        if(v.Label.empty()) {
            return std::format("\tnode{}", std::hash<float>{}(v.Data));
        }
        return std::format("node{}", std::hash<std::string>{}(v.Label));
    }

    std::pair<std::set<const Value*>, std::vector<std::pair<const Value*, const Value*>>> Trace(const Value* leaf) {
        std::set<const Value*> nodes;
        std::vector<std::pair<const Value *, const Value *>> edges;

        std::function<void(const Value*)> build = [&] (const Value* node) {
            nodes.insert(node);
            for(auto child : node->GetPrevious()) {
                if(child->Gradient < -100) continue;
                edges.emplace_back(child, node);

                if(!nodes.contains(child))
                    build(child);
            }
        };

        build(leaf);

        return { nodes, edges };
    }

    void Visualize(const Value& leafNode) {
        auto dot = GraphvizTemplate;

        auto [nodes, edges] = Trace(&leafNode);

        for(const auto& node : nodes) {
            auto uid = GetID(*node);
            dot += std::format("\t{} [label=\"{}\", shape=record];\n", uid, GetLabel(*node));

            if(!node->GetOperation().empty()) {
                dot += std::format("\t{}{} [label=\"{}\", shape=ellipse];\n", uid, int(node->GetOperation()[0]), node->GetOperation());
                dot += std::format("\t{}{} -> {};\n", uid, int(node->GetOperation()[0]), uid);
            }
        }

        for(const auto& [n1, n2] : edges) {
            auto uid1 = GetID(*n1);
            auto uid2 = GetID(*n2);
            dot += std::format("\t{} [label=\"{}\", shape=record];\n", uid1, GetLabel(*n1));
            dot += std::format("\t{} -> {}{};\n", uid1, uid2, int(n2->GetOperation()[0]));
        }

        dot += "}\n";

        std::ofstream out("./res/viz.dot");
        out.write(dot.c_str(), dot.size());
        out.close();

        system(" dot -Tpng ./res/viz.dot -o ./res/viz.png");
    }

}
