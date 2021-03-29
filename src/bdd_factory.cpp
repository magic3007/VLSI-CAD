#include "bdd_factory.h"
#include "glog/logging.h"

void BddFactory::genFromSop(SOP sop, const std::vector<int32_t> &vars_order,
                            std::vector<BDDNode> &bdds_array) {
    using VariableState = SOPImpl::VariableState;
    bdds_array.clear();
    bdds_array.reserve(sop.terms_num());
    for (const auto &term : sop.terms()) {
        auto root               = detail::makeBDDNode();
        root.is_leaf()          = true;
        root.leaf_value()       = true;
        auto false_leaf         = detail::makeBDDNode();
        false_leaf.is_leaf()    = true;
        false_leaf.leaf_value() = false;
        for (auto iter = vars_order.rbegin(); iter != vars_order.rend(); iter++) {
            int32_t var_id = *iter;
            if (term.at(var_id) == VariableState::IRRELEVANCE) {
                continue;
            } else {
                auto temp      = detail::makeBDDNode();
                temp.var_id()  = var_id;
                temp.is_leaf() = false;
                if (term.at(var_id) == VariableState::TRUE) {
                    temp.true_son()  = root;
                    temp.false_son() = false_leaf;
                } else {
                    temp.false_son() = root;
                    temp.true_son()  = false_leaf;
                }
                root = temp;
            }
        }
        LOG_IF(WARNING, root.is_leaf()) << "There is a term with all irrelevant variables.";
        bdds_array.emplace_back(root);
    }
}
