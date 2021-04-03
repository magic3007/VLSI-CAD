#include "bdd.h"
#include "bdd_factory.h"
#include "glog/logging.h"
#include "sop.h"
#include "sop_factory.h"
#include "utils/base.hpp"
#include "utils/stopwatch.hpp"
#include <cstdio>
#include <cxxabi.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <stdexcept>

bool travelBDD(BDDNode p) {
    if (p.is_leaf()) { return p.leaf_value(); }
    if (p.is_visited()) return true;
    p.is_visited() = true;
    if (!travelBDD(p.true_son())) return false;
    if (!travelBDD(p.false_son())) return false;
    return true;
}

bool groundTruth(SOP sop) {
    using VariableState    = SOPImpl::VariableState;
    int32_t vars_num       = sop.vars_num();
    auto    get_and_result = [&vars_num](std::vector<VariableState> &term, int32_t state) {
        auto rv = true;
        for (int32_t i = 0; i < vars_num; i++) {
            if (term.at(i) == VariableState::TRUE) {
                rv &= (state >> i) & 1;
            } else if (term.at(i) == VariableState::COMPLEMENT) {
                rv &= !((state >> i) & 1);
            }
            if (!rv) break;
        }
        return rv;
    };
    for (int32_t state = 0; state < (1 << vars_num); state++) {
        bool rv = false;
        for (auto &term : sop.terms()) {
            rv |= get_and_result(term, state);
            if (rv) break;
        }
        if (!rv) return false;
    }
    return true;
}

int main(int argc, char *argv[]) {
    google::InitGoogleLogging(argv[0]);
    FLAGS_logtostderr = true;
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input cube file path>\n", argv[0]);
        exit(-1);
    }
    char *input_path = argv[1];
    auto  stopwatch  = stopwatch::Stopwatch();
    stopwatch.start();
    std::ifstream in_file;
    in_file.open(input_path);
    DEFER({ in_file.close(); });
    SOP sop = SOPFactory::parseFromFile(in_file);
    LOG(INFO) << "# of vars: " << sop.vars_num() << "; # of terms: " << sop.terms_num();

    //    bool ground_truth = groundTruth(sop);
    //    LOG(INFO) << "ground truth is: " << std::boolalpha << (ground_truth ? "Yes" : "No");

    std::vector<int32_t> vars_order(sop.vars_num());
    std::iota(vars_order.begin(), vars_order.end(), 0);

    std::vector<int32_t> vars_rank(sop.vars_num());
    for (int32_t i = 0; i < sop.vars_num(); i++) { vars_rank[vars_order.at(i)] = i; }

    std::vector<BDDNode> bdds_array;
    BddFactory::genFromSop(sop, vars_order, bdds_array);
    auto root = BDDOperator::OR(bdds_array, vars_rank);

    bool result = travelBDD(root);
    LOG(INFO) << "🤔 Whether is this boolean function a tautology? " << (result ? "Yes!" : "No!");
    LOG(INFO) << std::setw(10)
              << "Total Elapsed time: " << stopwatch.elapsed<stopwatch::microseconds>() / 1000.0
              << " ms.";
    //    CHECK(result == ground_truth);
    return 0;
}