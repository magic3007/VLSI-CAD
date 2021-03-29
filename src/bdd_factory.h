#include "bdd.h"
#include "sop.h"
#include <vector>

class BddFactory {
public:
   static void genFromSop(SOP sop, const std::vector<int32_t> &vars_order, std::vector<BDDNode> &bdds_array);
};

