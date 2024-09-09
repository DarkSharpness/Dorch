#pragma once
#include "backward/node.h"
#include "basic/tensor.h"
#include "utility/shared.h"
#include <queue>

namespace dark {

struct BackwardEngine {
public:
    static auto operator()(shared_ptr<Node> root) -> void {
        _M_handle_dependency(root);
        _M_back_propagation(root);
    }

private:
    static auto _M_handle_dependency(shared_ptr<Node> node) -> void {
        std::queue<Node *> q;
        q.push(node.get());
        while (!q.empty()) {
            const auto current = q.front();
            q.pop();
            for (auto &from : current->get_from()) {
                from->get_graph().dependencies++;
                q.push(from.get());
            }
        }
    }

    static auto _M_back_propagation(shared_ptr<Node> node) -> void {
        std::queue<Node *> q;
        q.push(node.get());
        while (!q.empty()) {
            const auto current = q.front();
            q.pop();
            // Calculate the gradient
            auto &graph = current->get_graph();
            auto inputs = current->backward(graph.grad.value());
            for (size_t i = 0; i < current->get_from().size(); i++) {
                auto *from  = current->get_from()[i].get();
                auto &graph = from->get_graph();
                graph.acumulate(inputs[i]);
                if (--graph.dependencies == 0)
                    q.push(from);
            }
        }
    }
};

} // namespace dark
