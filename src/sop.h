#pragma once
#include "utils/arg.hpp"
#include <memory>
#include <vector>

class SOPImpl {
public:
    enum class VariableState
    {
        TRUE,
        COMPLEMENT,
        IRRELEVANCE
    };

    using Term = std::vector<VariableState>;

private:
    CLASS_ARG(int32_t, vars_num);
    CLASS_ARG(int32_t, terms_num);
    CLASS_ARG(std::vector<Term>, terms);
};

class SOP {
    SHARED_CLASS_ARG(SOP, impl);

public:
    FORWARDED_METHOD(vars_num)
    FORWARDED_METHOD(terms_num)
    FORWARDED_METHOD(terms)
};

MAKE_SHARED_CLASS(SOP)