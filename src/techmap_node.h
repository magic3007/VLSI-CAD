#pragma once
#include "utils/arg.hpp"
#include <functional>
#include <memory>
#include <vector>

enum class CellType
{
    SAME,
    INV,
    NAND2,
    NOR2,
    AOI21,
    AOI12,
    AOI22,
    ONE,
    ZERO,
    UNDEFINED
};

inline const char *ToString(CellType cell_type) {
    switch (cell_type) {
        case CellType::SAME:
            return "SAME";
        case CellType::INV:
            return "INV";
        case CellType::NAND2:
            return "NAND2";
        case CellType::NOR2:
            return "NOR2";
        case CellType::AOI22:
            return "AOI22";
        case CellType::AOI12:
            return "AOI12";
        case CellType::AOI21:
            return "AOI21";
        case CellType::ONE:
            return "ONE";
        case CellType::ZERO:
            return "ZERO";
        case CellType::UNDEFINED:
            return "[UNKNOWN CELL TYPE]";
    }
}


class NodeImpl;

class DpStateImpl;

class DpState {
public:
    DpState() : impl_(nullptr) {}

    std::string &         name();
    long double &         min_cell_cnt();
    int64_t &             max_depth();
    std::vector<DpState> &sons();
    CellType &            cell_type();
    bool &                is_leaf();
    bool &                visited();
    [[nodiscard]] bool    defined() const { return impl_ != nullptr; }
    void                  update();

private:
    SHARED_CLASS_ARG(DpState, impl);
};

class DpStateImpl {

public:
    void update();

private:
    CLASS_ARG(std::string, name);
    CLASS_ARG(bool, is_leaf)       = false;
    CLASS_ARG(CellType, cell_type) = CellType::UNDEFINED;
    CLASS_ARG(std::vector<DpState>, sons){};
    CLASS_ARG(long double, min_cell_cnt) = std::numeric_limits<long double>::max();
    CLASS_ARG(int64_t, max_depth)        = std::numeric_limits<int64_t>::max();
    CLASS_ARG(bool, visited)             = false;
};

class Node {
public:
    using IndexType  = std::int32_t;
    using BinaryFunc = std::function<bool(bool, bool)>;

private:
    SHARED_CLASS_ARG(Node, impl);

public:
    Node() : impl_(nullptr) {}

    std::string &           name();
    std::vector<Node> &     sources();
    [[nodiscard]] IndexType num_sources() const;
    bool &                  is_model();
    bool &                  is_leaf();
    IndexType &             num_fan_out();
    BinaryFunc &            ori_func();
    DpState &               dp_state(bool is_inv);
    IndexType &             cover_a_state();
    IndexType &             cover_b_state();
    IndexType &             cover_out_state();
};

class NodeImpl {
    using IndexType  = Node::IndexType;
    using BinaryFunc = Node::BinaryFunc;

private:
    CLASS_ARG(std::string, name);
    CLASS_ARG(std::vector<Node>, sources){};
    CLASS_ARG(bool, is_model)         = false;
    CLASS_ARG(bool, is_leaf)          = false;
    CLASS_ARG(IndexType, num_fan_out) = 0;
    CLASS_ARG(IndexType, cover_a_state);
    CLASS_ARG(IndexType, cover_b_state);
    CLASS_ARG(IndexType, cover_out_state);
    CLASS_ARG(BinaryFunc, ori_func);
    // dynamic programming relative variables
    CLASS_ARG(DpState, ori_state);
    CLASS_ARG(DpState, inv_state);
};

MAKE_SHARED_CLASS(DpState)
MAKE_SHARED_CLASS(Node)