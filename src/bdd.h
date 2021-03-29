#pragma once
#include "utils/arg.hpp"
#include <cstdint>
#include <memory>
#include <vector>

class BDDNodeImpl;

class BDDNode {
private:
    SHARED_CLASS_ARG(BDDNode, impl);

public:
    BDDNode() : impl_(nullptr) {}
    bool &   is_leaf();
    bool &   leaf_value();
    int32_t &var_id();
    BDDNode &true_son();
    BDDNode &false_son();
    bool     defined();
    bool &   is_visited();
};

class BDDNodeImpl {
    CLASS_ARG(bool, is_leaf){};
    CLASS_ARG(bool, leaf_value){};
    CLASS_ARG(int32_t, var_id){};
    CLASS_ARG(bool, is_visited){};
    CLASS_ARG(BDDNode, true_son);
    CLASS_ARG(BDDNode, false_son);
};


MAKE_SHARED_CLASS(BDDNode)

struct BDDOperator {
    static BDDNode OR(std::vector<BDDNode> &bdds, const std::vector<int32_t> &vars_rank);
};