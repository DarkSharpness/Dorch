#pragma once
#include "basic/tensor.h"
#include "utility/shared.h"
#include <cstddef>
#include <optional>
#include <vector>

namespace dark {

using TensorList = std::vector<Tensor>;

struct Node {
public:
    Node(const Node &)                     = delete;
    Node(Node &&)                          = delete;
    auto operator=(const Node &) -> Node & = delete;
    auto operator=(Node &&) -> Node &      = delete;

    virtual auto forward(TensorList) -> Tensor  = 0;
    virtual auto backward(Tensor) -> TensorList = 0;

    struct GraphStorage {
        std::size_t dependencies {};
        std::optional<Tensor> grad;
        void acumulate(Tensor);
    };

    auto get_graph() -> GraphStorage & { return this->_M_graph; }
    auto get_graph() const -> const GraphStorage & { return this->_M_graph; }

    auto get_from() -> std::vector<shared_ptr<Node>> & { return this->_M_from; }
    auto get_from() const -> const std::vector<shared_ptr<Node>> & { return this->_M_from; }

private:
    std::size_t _M_ref_count = 0;
    GraphStorage _M_graph;
    std::vector<shared_ptr<Node>> _M_from;
};

} // namespace dark
