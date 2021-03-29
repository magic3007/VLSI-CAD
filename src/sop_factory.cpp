#include "sop_factory.h"
#include "glog/logging.h"
#include <sstream>

SOP SOPFactory::parseFromFile(std::istream &is) {
    auto sop        = detail::makeSOP();
    sop.vars_num()  = 0;
    sop.terms_num() = 0;
    std::string line;
    std::getline(is, line);
    while (std::getline(is, line)) {
        sop.terms().emplace_back();
        auto &             term_vec = sop.terms().back();
        int32_t            i        = 0;
        std::istringstream iss(line);
        std::string        state;
        while ((iss >> state)) {
            if (state == "01") {
                term_vec.push_back(SOPImpl::VariableState::TRUE);
            } else if (state == "10") {
                term_vec.push_back(SOPImpl::VariableState::COMPLEMENT);
            } else if (state == "11") {
                term_vec.push_back(SOPImpl::VariableState::IRRELEVANCE);
            } else {
                LOG(ERROR) << "Unsupported variable state in the input file: " << state;
            }
            i++;
        }
        if (sop.vars_num() == 0) { sop.vars_num() = i; }
        sop.terms_num()++;
    }
    return sop;
}
