#include "bdd.h"
#include "glog/logging.h"
#include <algorithm>

bool &   BDDNode::is_leaf() { return impl_->is_leaf(); }
bool &   BDDNode::leaf_value() { return impl_->leaf_value(); }
int32_t &BDDNode::var_id() { return impl_->var_id(); }
BDDNode &BDDNode::true_son() { return impl_->true_son(); }
BDDNode &BDDNode::false_son() { return impl_->false_son(); }
bool &   BDDNode::is_visited() { return impl_->is_visited(); }
bool     BDDNode::defined() { return impl_ == nullptr; }

BDDNode BDDOperator::OR(std::vector<BDDNode> &bdds, const std::vector<int32_t> &vars_rank) {
    CHECK(!bdds.empty());
    // termination condition
    for (auto iter = bdds.begin(); iter != bdds.end();) {
        auto bdd = *iter;
        if (bdd.is_leaf()) {
            if (bdd.leaf_value()) { return bdd; }
            iter = bdds.erase(iter);
        } else {
            iter++;
        }
    }
    if (bdds.empty()) {
        // all the BDD nodes rea FALSE leaves.
        auto false_leaf         = detail::makeBDDNode();
        false_leaf.is_leaf()    = true;
        false_leaf.leaf_value() = false;
        return false_leaf;
    }
    int32_t var_id = std::numeric_limits<int32_t>::max();
    for (auto &bdd : bdds) {
        CHECK(!bdd.is_leaf());
        if (var_id == std::numeric_limits<int32_t>::max() ||
            vars_rank.at(var_id) > vars_rank.at(bdd.var_id())) {
            var_id = bdd.var_id();
        }
    }
    CHECK(var_id != std::numeric_limits<int32_t>::max());
    auto true_selector = [&var_id](BDDNode node) {
        return node.var_id() == var_id ? node.true_son() : node;
    };
    auto false_selector = [&var_id](BDDNode node) {
        return node.var_id() == var_id ? node.false_son() : node;
    };
    std::vector<BDDNode> true_bdds(bdds.size()), false_bdds(bdds.size());
    std::transform(bdds.begin(), bdds.end(), true_bdds.begin(), true_selector);
    std::transform(bdds.begin(), bdds.end(), false_bdds.begin(), false_selector);
    auto true_son    = OR(true_bdds, vars_rank);
    auto false_son   = OR(false_bdds, vars_rank);
    auto root        = detail::makeBDDNode();
    root.is_leaf()   = false;
    root.var_id()    = var_id;
    root.true_son()  = true_son;
    root.false_son() = false_son;
    return root;
}
